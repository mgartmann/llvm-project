//===--- StdStreamObjectsOutsideMainCheck.cpp - clang-tidy ------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "StdStreamObjectsOutsideMainCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace misc {

void StdStreamObjectsOutsideMainCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(
      declRefExpr(to(namedDecl(hasAnyName("cin", "wcin", "cout", "wcout", "cerr", "wcerr", "clog", "wclog")))).bind("match"), this);
}

void StdStreamObjectsOutsideMainCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *MatchedDecl = Result.Nodes.getNodeAs<DeclRefExpr>("match");

  const bool isInMain =
      StdStreamObjectsOutsideMainCheck::isInsideMainFunction(Result, DynTypedNode::create(*MatchedDecl));

  if (isInMain) {
    return;
  }

  diag(MatchedDecl->getLocation(),
       "predefined standard stream objects should not be used outside the main function");
}

bool StdStreamObjectsOutsideMainCheck::isInsideMainFunction(const MatchFinder::MatchResult &Result,
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
        return StdStreamObjectsOutsideMainCheck::isInsideMainFunction(Result, Parent);
      });
}

} // namespace misc
} // namespace tidy
} // namespace clang