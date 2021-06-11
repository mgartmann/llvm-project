//===--- AvoidInitDefaultConstructorsCheck.cpp - clang-tidy ---------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidInitDefaultConstructorsCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace cppcoreguidelines {

void AvoidInitDefaultConstructorsCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(
      cxxConstructorDecl(
          hasBody(compoundStmt(unless(hasAnySubstatement(anything())))),
          isDefaultConstructor(),
          unless(hasAnyConstructorInitializer(cxxCtorInitializer(
              withInitializer(hasDescendant(declRefExpr(to(parmVarDecl()))))))),
          hasAnyConstructorInitializer(
              forField(unless(hasInClassInitializer(anything())))))
          .bind("Constructor"),
      this);
}

void AvoidInitDefaultConstructorsCheck::check(
    const MatchFinder::MatchResult &Result) {
  const auto *MatchedConstructor =
      Result.Nodes.getNodeAs<CXXConstructorDecl>("Constructor");

  diag(MatchedConstructor->getLocation(),
       "Default constructors like %0 can be omitted. Use in-class member "
       "initializers instead")
      << MatchedConstructor;
}

} // namespace cppcoreguidelines
} // namespace tidy
} // namespace clang
