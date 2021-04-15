//===--- AvoidNonConstGlobalVariablesCheck.cpp - clang-tidy ---------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidNonConstGlobalVariablesCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Lex/Lexer.h"
#include <iostream>

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace cppcoreguidelines {

namespace {
AST_MATCHER(VarDecl, isLocalVarDecl) { return Node.isLocalVarDecl(); }
} // namespace

void AvoidNonConstGlobalVariablesCheck::registerMatchers(MatchFinder *Finder) {
  auto GlobalVariable = varDecl(
      hasGlobalStorage(),
      unless(anyOf(
          isLocalVarDecl(), isConstexpr(), hasType(isConstQualified()),
          hasType(referenceType())))); // References can't be changed, only the
                                       // data they reference can be changed.

  auto GlobalReferenceToNonConst =
      varDecl(hasGlobalStorage(), hasType(referenceType()),
              unless(hasType(references(qualType(isConstQualified())))));

  auto GlobalPointerToNonConst =
      varDecl(hasGlobalStorage(),
              hasType(pointerType(pointee(unless(isConstQualified())))));

  Finder->addMatcher(GlobalVariable.bind("non-const_variable"), this);
  Finder->addMatcher(GlobalReferenceToNonConst.bind("indirection_to_non-const"),
                     this);
  Finder->addMatcher(GlobalPointerToNonConst.bind("indirection_to_non-const"),
                     this);
}

void AvoidNonConstGlobalVariablesCheck::check(
    const MatchFinder::MatchResult &Result) {

  if (const auto *Variable =
          Result.Nodes.getNodeAs<VarDecl>("non-const_variable")) {

    auto TypeBeginLoc = Variable->getBeginLoc();
    auto TypeEndLoc = Variable->getBeginLoc().getLocWithOffset(
        Variable->getType().getAsString().length());

    auto ReplacementRange =
        CharSourceRange::getCharRange(TypeBeginLoc, TypeEndLoc);

    auto Replacement = getConstTypeAsString(Result, Variable);

    diag(Variable->getLocation(), "variable %0 is non-const and globally "
                                  "accessible, consider making it const")
        << Variable
        << FixItHint::CreateReplacement(ReplacementRange, Replacement);

    // Don't return early, a non-const variable may also be a pointer or
    // reference to non-const data.
  }

  if (const auto *VD =
          Result.Nodes.getNodeAs<VarDecl>("indirection_to_non-const")) {
    diag(VD->getLocation(),
         "variable %0 provides global access to a non-const object; consider "
         "making the %select{referenced|pointed-to}1 data 'const'")
        << VD << VD->getType()->isPointerType() << FixItHint::CreateInsertion(VD->getBeginLoc(), "const ");
  }
}

std::string AvoidNonConstGlobalVariablesCheck::getConstTypeAsString(
    const MatchFinder::MatchResult &Result, const VarDecl *Variable) const {

  auto Type = Variable->getType();
  bool HasSpace = hasSpaceAfterType(Result, Variable, Type);
  Type.addConst();

  return Type.getAsString() + std::string(HasSpace ? "" : " ");
}

bool AvoidNonConstGlobalVariablesCheck::hasSpaceAfterType(
    const MatchFinder::MatchResult &Result, const VarDecl *Variable,
    const QualType &OldType) const {

  StringRef VariableText = Lexer::getSourceText(
      CharSourceRange::getTokenRange(Variable->getSourceRange()),
      *Result.SourceManager, getLangOpts());

  if (OldType.getAsString().find("class") == 0) {
    return true;
  }

  return VariableText.str().at(OldType.getAsString().length()) == ' ';
}

} // namespace cppcoreguidelines
} // namespace tidy
} // namespace clang
