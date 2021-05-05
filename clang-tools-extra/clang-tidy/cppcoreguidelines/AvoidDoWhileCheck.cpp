//===--- AvoidDoWhileCheck.cpp - clang-tidy -------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidDoWhileCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace cppcoreguidelines {

void AvoidDoWhileCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(doStmt().bind("doStmt"), this);
}

void AvoidDoWhileCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *MatchedDoStmt = Result.Nodes.getNodeAs<DoStmt>("doStmt");

  diag(MatchedDoStmt->getDoLoc(),
       "Try to avoid using do-while loops in terms of readability. Consider "
       "using a while loop instead.");
}

} // namespace cppcoreguidelines
} // namespace tidy
} // namespace clang
