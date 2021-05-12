//===--- AvoidStdIoOutsideMainCheck.h - clang-tidy --------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISC_AVOIDSTDIOOUTSIDEMAINCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISC_AVOIDSTDIOOUTSIDEMAINCHECK_H

#include "../ClangTidyCheck.h"
#include <string>

namespace clang {
namespace tidy {
namespace misc {

/// Finds predefined standard stream objects like ``cin``, ``wcin``, ``cout``,
/// ``wcout``, ``cerr`` or ``wcerr``, that are used outside the ``main``
/// function. It also finds uses of ``cstdio`` and ``stdio.h`` functions like
/// ``printf()`` outside the ``main`` function.
///
/// For the user-facing documentation and examples see:
/// http://clang.llvm.org/extra/clang-tidy/checks/misc-avoid-std-io-outside-main.html
class AvoidStdIoOutsideMainCheck : public ClangTidyCheck {
public:
  AvoidStdIoOutsideMainCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.CPlusPlus;
  }

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

private:
  const std::vector<std::string> StdIOStreams = {"cin",   "wcin", "cout",
                                                 "wcout", "cerr", "wcerr"};
  const std::vector<std::string> CLikeIOFunctions = {
      "printf", "vprintf", "puts", "putchar", "scanf", "getchar", "gets"};
};

} // namespace misc
} // namespace tidy
} // namespace clang

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISC_AVOIDSTDIOOUTSIDEMAINCHECK_H
