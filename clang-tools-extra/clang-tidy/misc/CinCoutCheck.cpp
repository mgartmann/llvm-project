//===--- CinCoutCheck.cpp - clang-tidy ------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "CinCoutCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace misc {

void CinCoutCheck::registerMatchers(MatchFinder *Finder) {
  //TODO: declRefExpr(to(namedDecl(hasAnyName("cout"))), to(namedDecl(hasAnyName("cin"))))
  Finder->addMatcher(
      declRefExpr(to(namedDecl(hasAnyName("cout")))).bind("match"), this);
  Finder->addMatcher(
      declRefExpr(to(namedDecl(hasAnyName("cin")))).bind("match"), this);
}

void CinCoutCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *MatchedDecl = Result.Nodes.getNodeAs<DeclRefExpr>("match");

  const bool isInMain =
      CinCoutCheck::isInsideMainFunction(Result, DynTypedNode::create(*MatchedDecl));

  if (isInMain) {
    return;
  }

  diag(MatchedDecl->getLocation(),
       "cin/cout should only be used inside main function");
}

bool CinCoutCheck::isInsideMainFunction(const MatchFinder::MatchResult &Result,
                                        const DynTypedNode &Node) {
  const auto *AsFunctionDecl = Node.get<FunctionDecl>();

  if (AsFunctionDecl) {
    if (AsFunctionDecl->getIdentifier() &&
        AsFunctionDecl->getName().equals("main")) {
      return true;
    }
  }

  return llvm::any_of(
      Result.Context->getParents(Node), [&Result](const DynTypedNode &Parent) {
        return CinCoutCheck::isInsideMainFunction(Result, Parent);
      });
}

} // namespace misc
} // namespace tidy
} // namespace clang