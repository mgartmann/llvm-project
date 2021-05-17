//===--- AvoidDefaultConstructorWithOnlyInitializersCheck.cpp - clang-tidy
//-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidDefaultConstructorWithOnlyInitializersCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace cppcoreguidelines {

void AvoidDefaultConstructorWithOnlyInitializersCheck::registerMatchers(
    MatchFinder *Finder) {
  Finder->addMatcher(
      cxxConstructorDecl(
          hasBody(compoundStmt(unless(hasAnySubstatement(anything())))),
          isDefaultConstructor(), hasAnyConstructorInitializer(anything())).bind("Constructor"),
      this);
}

void AvoidDefaultConstructorWithOnlyInitializersCheck::check(
    const MatchFinder::MatchResult &Result) {
  const auto *MatchedConstructor =
      Result.Nodes.getNodeAs<CXXConstructorDecl>("Constructor");

  diag(MatchedConstructor->getLocation(),
       "The default constructor %0 does only initialize member variables.")
      << MatchedConstructor;
}

} // namespace cppcoreguidelines
} // namespace tidy
} // namespace clang
