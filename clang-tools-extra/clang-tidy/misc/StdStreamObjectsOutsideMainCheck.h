//===--- StdStreamObjectsOutsideMainCheck.h - clang-tidy --------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISC_STDSTREAMOBJECTSOUTSIDEMAINCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISC_STDSTREAMOBJECTSOUTSIDEMAINCHECK_H

#include "../ClangTidyCheck.h"

namespace clang {
namespace tidy {
namespace misc {

/// Diagnoses if a predefined standard stream object (`cin`, `wcin`, 
/// `cout`, `wcout`, `cerr`, `wcerr`, `clog` or `wclog`) is used 
/// outside the `main` function.
///
/// For the user-facing documentation and examples see:
/// http://clang.llvm.org/extra/clang-tidy/checks/misc-std-stream-objects-outside-main.html
class StdStreamObjectsOutsideMainCheck : public ClangTidyCheck {
public:
  StdStreamObjectsOutsideMainCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
  static bool
  isInsideMainFunction(const ast_matchers::MatchFinder::MatchResult &Result,
                       const DynTypedNode &Node);
};

} // namespace misc
} // namespace tidy
} // namespace clang

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISC_STDSTREAMOBJECTSOUTSIDEMAINCHECK_H