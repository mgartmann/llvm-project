//===--- DeclareLoopVariableInTheInitializerCheck.cpp - clang-tidy --------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "DeclareLoopVariableInTheInitializerCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace cppcoreguidelines {

void DeclareLoopVariableInTheInitializerCheck::registerMatchers(
    MatchFinder *Finder) {
  Finder->addMatcher(forStmt(unless(has(declStmt()))).bind("forStmt"), this);
}

void DeclareLoopVariableInTheInitializerCheck::check(
    const MatchFinder::MatchResult &Result) {
  const auto *MatchedForStmt = Result.Nodes.getNodeAs<ForStmt>("forStmt");

  diag(MatchedForStmt->getBeginLoc(),
       "Prefer to declare a loop variable in the initializer part of a "
       "for-statement");
}

} // namespace cppcoreguidelines
} // namespace tidy
} // namespace clang
