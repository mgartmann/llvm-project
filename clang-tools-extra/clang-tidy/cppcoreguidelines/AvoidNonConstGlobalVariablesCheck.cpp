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

    auto ReplacementRange = generateReplacementRange(*Variable);
    auto Replacement = generateReplacementString(Result, *Variable);

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
        << VD << VD->getType()->isPointerType()
        << FixItHint::CreateInsertion(VD->getBeginLoc(), "const ");
  }
}

/// Makes the provided type constant and converts it to a string.
/// If the current type sticks to the variable name as in the example below, a
/// space is inserted:
//
/// \code
/// int *y = &x;
///     ^^
/// \endcode
///
/// \returns string representation of the constant type of \p Variable.
std::string AvoidNonConstGlobalVariablesCheck::generateReplacementString(
    const MatchFinder::MatchResult &Result, const VarDecl &Variable) const {

  auto Type = Variable.getType();
  bool HasSpace = hasSpaceAfterType(Result, Variable, printCleanedType(Type));
  Type.addConst();
  return printCleanedType(Type) + (HasSpace ? "" : " ");
}

bool AvoidNonConstGlobalVariablesCheck::hasSpaceAfterType(
    const MatchFinder::MatchResult &Result, const VarDecl &Variable,
    const std::string &NonConstType) const {

  StringRef VariableText = Lexer::getSourceText(
      CharSourceRange::getTokenRange(Variable.getSourceRange()),
      *Result.SourceManager, getLangOpts());

  return VariableText.str().at(NonConstType.length()) == ' ';
}

CharSourceRange AvoidNonConstGlobalVariablesCheck::generateReplacementRange(
    const VarDecl &Variable) const {

  auto TypeBeginLoc = Variable.getBeginLoc();

  auto TypeEndLoc = TypeBeginLoc.getLocWithOffset(
      printCleanedType(Variable.getType()).length());

  return CharSourceRange::getCharRange(TypeBeginLoc, TypeEndLoc);
}

/// Creates a string representation of \p Type and suppresses the \c class
/// keyword in a class instance's type. Also, on unnamed types, the tag location
/// and the \c unnamed keyword are removed from the type description.
/// If this would not be done, those keywords would be inserted into the source
/// code as part of the \c FixItHint replacement.
std::string AvoidNonConstGlobalVariablesCheck::printCleanedType(
    const QualType &Type) const {

  /// \c PrintingPolicy suppresses the "class" keyword in a class
  /// instance's type and to suppress locations of anonymous tags.
  PrintingPolicy PrintingPolicy{getLangOpts()};
  PrintingPolicy.AnonymousTagLocations = false;
  std::string PolicyCleanedType = Type.getAsString(PrintingPolicy);

  std::string StringToErase = " (unnamed)";
  int StartPositionToErase = PolicyCleanedType.find(StringToErase);

  if (StartPositionToErase == std::string::npos)
    return PolicyCleanedType;

  return PolicyCleanedType.erase(StartPositionToErase, StringToErase.length());
}

} // namespace cppcoreguidelines
} // namespace tidy
} // namespace clang
