//===--- AvoidStdIoOutsideMainCheck.cpp - clang-tidy
//-----------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidStdIoOutsideMainCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace misc {

void AvoidStdIoOutsideMainCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(
      declRefExpr(to(varDecl(hasAnyName("cin", "wcin", "cout", "wcout", "cerr",
                                        "wcerr"),
                             isInStdNamespace())),
                  unless(forFunction(isMain())))
          .bind("StdStreamObject"),
      this);

  Finder->addMatcher(declRefExpr(hasDeclaration(functionDecl(hasAnyName(
                                     "printf", "vprintf", "puts", "putchar",
                                     "scanf", "getchar", "gets"))),
                                 unless(forFunction(isMain())))
                         .bind("CLibFunction"),
                     this);

  /// Matcher for indirect stdio uses:
  /// \code
  ///   auto Print = &puts;
  ///   Print("This is using stdio");
  /// \endcode
  Finder->addMatcher(
      declRefExpr(
          hasDeclaration(varDecl(hasDescendant(declRefExpr(hasDeclaration(
              functionDecl(hasAnyName("printf", "vprintf", "puts", "putchar",
                                      "scanf", "getchar", "gets"))))))),
          unless(forFunction(isMain())))
          .bind("CLibFunction"),
      this);
}

void AvoidStdIoOutsideMainCheck::check(const MatchFinder::MatchResult &Result) {

  if (const auto *MatchedStreamObj =
          Result.Nodes.getNodeAs<DeclRefExpr>("StdStreamObject")) {
    diag(MatchedStreamObj->getLocation(),
         "predefined standard stream objects should "
         "not be used outside the main function");
    return;
  }

  if (const auto *MatchedCLibFunc =
          Result.Nodes.getNodeAs<DeclRefExpr>("CLibFunction")) {
    diag(MatchedCLibFunc->getLocation(),
         "cstdio functions should not be used outside the main function");
    return;
  }
}

} // namespace misc
} // namespace tidy
} // namespace clang
