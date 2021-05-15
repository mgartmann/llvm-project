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
  ast_matchers::internal::Matcher<CXXRecordDecl> InheritsVirtualMethod =
      hasAnyBase(hasType(cxxRecordDecl(has(cxxMethodDecl(isVirtual())))));

  Finder->addMatcher(
      cxxRecordDecl(
          anyOf(isStruct(), isClass()),
          anyOf(has(cxxMethodDecl(isVirtual())), InheritsVirtualMethod),
          unless(anyOf(
              has(cxxDestructorDecl(isPublic(), isVirtual())),
              has(cxxDestructorDecl(isProtected(), unless(isVirtual()))))))
          .bind("ProblematicClassOrStruct"),
      this);
}

static CharSourceRange
getVirtualKeywordRange(const CXXDestructorDecl &Destructor,
                       const SourceManager &SM, const LangOptions &LangOpts) {
  SourceLocation VirtualBeginLoc = Destructor.getBeginLoc();
  SourceLocation VirtualEndLoc = VirtualBeginLoc.getLocWithOffset(
      Lexer::MeasureTokenLength(VirtualBeginLoc, SM, LangOpts));

  /// Range ends with \c StartOfNextToken so that any whitespace after \c
  /// virtual is included.
  SourceLocation StartOfNextToken =
      Lexer::findNextToken(VirtualEndLoc, SM, LangOpts)
          .getValue()
          .getLocation();

  CharSourceRange Range = CharSourceRange{};
  Range.setBegin(VirtualBeginLoc);
  Range.setEnd(StartOfNextToken);
  return Range;
}

static const AccessSpecDecl *
getPublicASDecl(const CXXRecordDecl &StructOrClass) {
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

static FixItHint
generateUserDeclaredDestructor(const CXXRecordDecl &StructOrClass,
                               const SourceManager &SourceManager) {
  std::string DestructorString;
  SourceLocation Loc;
  bool AppendLineBreak = false;

  const AccessSpecDecl *AccessSpecDecl = getPublicASDecl(StructOrClass);

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
      .append("virtual ~")
      .append(StructOrClass.getName().str())
      .append("() = default;")
      .append(AppendLineBreak ? "\n" : "");

  return FixItHint::CreateInsertion(Loc, DestructorString);
}

void VirtualBaseClassDestructorCheck::check(
    const MatchFinder::MatchResult &Result) {

  const auto *MatchedClassOrStruct =
      Result.Nodes.getNodeAs<CXXRecordDecl>("ProblematicClassOrStruct");

  const CXXDestructorDecl *Destructor = MatchedClassOrStruct->getDestructor();

  if (Destructor->getAccess() == AS_private) {
    diag(MatchedClassOrStruct->getLocation(),
         "destructor of %0 is private and prevents using the type. Consider "
         "making it public and virtual or protected and non-virtual")
        << MatchedClassOrStruct;

    return;
  }

  // Implicit destructors are public and non-virtual for classes and structs.
  bool ProtectedVirtual = false;
  FixItHint Fix;

  if (MatchedClassOrStruct->hasUserDeclaredDestructor()) {
    if (Destructor->getAccess() == AccessSpecifier::AS_public) {
      Fix = FixItHint::CreateInsertion(Destructor->getLocation(), "virtual ");
    } else if (Destructor->getAccess() == AS_protected) {
      ProtectedVirtual = true;
      Fix = FixItHint::CreateRemoval(getVirtualKeywordRange(
          *Destructor, *Result.SourceManager, Result.Context->getLangOpts()));
    }
  } else {
    Fix = generateUserDeclaredDestructor(*MatchedClassOrStruct,
                                         *Result.SourceManager);
  }

  diag(MatchedClassOrStruct->getLocation(),
       "destructor of %0 is %select{public and non-virtual|protected and "
       "virtual}1. It should either be public and virtual or protected and "
       "non-virtual")
      << MatchedClassOrStruct << ProtectedVirtual << Fix;
}

} // namespace cppcoreguidelines
} // namespace tidy
} // namespace clang
