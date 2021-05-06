//===--- VirtualBaseClassDestructorCheck.cpp - clang-tidy -----------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "VirtualBaseClassDestructorCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Lex/Lexer.h"
#include <string>

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace cppcoreguidelines {

void VirtualBaseClassDestructorCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(
      cxxRecordDecl(
          anyOf(isStruct(), isClass()), has(cxxMethodDecl(isVirtual())),
          unless(anyOf(
              has(cxxDestructorDecl(isPublic(), isVirtual())),
              has(cxxDestructorDecl(isProtected(), unless(isVirtual()))))))
          .bind("ProblematicClassOrStruct"),
      this);
}

void VirtualBaseClassDestructorCheck::check(
    const MatchFinder::MatchResult &Result) {

  const auto *MatchedClassOrStruct =
      Result.Nodes.getNodeAs<CXXRecordDecl>("ProblematicClassOrStruct");

  const auto Destructor = MatchedClassOrStruct->getDestructor();

  if (Destructor->getAccess() == AS_private) {
    diag(MatchedClassOrStruct->getLocation(),
         "destructor of %0 %1 is private and prevents using the type. Consider "
         "making it public and virtual or protected and non-virtual")
        << (MatchedClassOrStruct->isClass() ? "class" : "struct")
        << MatchedClassOrStruct;

    return;
  }

  // Implicit destructors are public and non-virtual for classes and structs.
  std::string TypeAndVirtuality = "public and non-virtual";
  FixItHint Fix;

  if (MatchedClassOrStruct->hasUserDeclaredDestructor()) {
    if (Destructor->getAccess() == AccessSpecifier::AS_public) {
      Fix = FixItHint::CreateInsertion(Destructor->getLocation(), "virtual ");
    } else if (Destructor->getAccess() == AS_protected) {
      TypeAndVirtuality = "protected and virtual";
      Fix = FixItHint::CreateRemoval(getVirtualKeywordRange(
          *Destructor, *Result.SourceManager, Result.Context->getLangOpts()));
    }
  } else {
    Fix = generateUserDeclaredConstructor(*MatchedClassOrStruct,
                                          *Result.SourceManager);
  }

  diag(MatchedClassOrStruct->getLocation(),
       "destructor of %0 %1 is %2. It should either be public and virtual or "
       "protected and non-virtual")
      << (MatchedClassOrStruct->isClass() ? "class" : "struct")
      << MatchedClassOrStruct << TypeAndVirtuality << Fix;
}

CharSourceRange VirtualBaseClassDestructorCheck::getVirtualKeywordRange(
    const CXXDestructorDecl &Destructor, const SourceManager &SM,
    const LangOptions &LangOpts) const {
  auto VirtualBeginLoc = Destructor.getBeginLoc();
  auto VirtualEndLoc = VirtualBeginLoc.getLocWithOffset(
      Lexer::MeasureTokenLength(VirtualBeginLoc, SM, LangOpts));

  /// Range ends with \c StartOfNextToken so that any whitespace after \c
  /// virtual is included.
  auto StartOfNextToken = Lexer::findNextToken(VirtualEndLoc, SM, LangOpts)
                              .getValue()
                              .getLocation();

  CharSourceRange Range = CharSourceRange{};
  Range.setBegin(VirtualBeginLoc);
  Range.setEnd(StartOfNextToken);
  return Range;
}

FixItHint VirtualBaseClassDestructorCheck::generateUserDeclaredConstructor(
    const CXXRecordDecl &StructOrClass,
    const SourceManager &SourceManager) const {
  auto DestructorString = std::string("");
  SourceLocation Loc;
  bool AppendLineBreak = false;

  auto ParentIndentation =
      SourceManager.getExpansionColumnNumber(StructOrClass.getBeginLoc()) - 1;

  auto AccessSpecDecl = getPublicASDecl(StructOrClass);

  if (!AccessSpecDecl) {
    if (StructOrClass.isClass()) {
      Loc = StructOrClass.getEndLoc();
      DestructorString.append("public:");
      AppendLineBreak = true;
    } else {
      Loc = StructOrClass.getBraceRange().getBegin().getLocWithOffset(1);
    }
  } else {
    Loc = AccessSpecDecl->getEndLoc().getLocWithOffset(1);
  }

  DestructorString.append("\n")
      .append(indent(ParentIndentation + IndentationWidth))
      .append("virtual ~")
      .append(StructOrClass.getName().str())
      .append("() = default;")
      .append(AppendLineBreak ? "\n" + indent(ParentIndentation) : "");

  return FixItHint::CreateInsertion(Loc, DestructorString);
}

AccessSpecDecl *VirtualBaseClassDestructorCheck::getPublicASDecl(
    const CXXRecordDecl &StructOrClass) const {
  for (DeclContext::specific_decl_iterator<AccessSpecDecl>
           AS{StructOrClass.decls_begin()},
       ASEnd{StructOrClass.decls_end()};
       AS != ASEnd; ++AS) {
    AccessSpecDecl *ASDecl = *AS;
    if (ASDecl->getAccess() == AccessSpecifier::AS_public)
      return ASDecl;
  }

  return nullptr;
}

std::string VirtualBaseClassDestructorCheck::indent(int Indentation) const {
  return std::string().append(Indentation, ' ');
}

} // namespace cppcoreguidelines
} // namespace tidy
} // namespace clang
