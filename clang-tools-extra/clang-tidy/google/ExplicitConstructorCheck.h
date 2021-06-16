//===--- ExplicitConstructorCheck.h - clang-tidy ----------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_GOOGLE_EXPLICITCONSTRUCTORCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_GOOGLE_EXPLICITCONSTRUCTORCHECK_H

#include "../ClangTidyCheck.h"
#include "../utils/OptionsUtils.h"

namespace clang {
namespace tidy {
namespace google {

/// Checks that all single-argument constructors are explicit.
///
/// See https://google.github.io/styleguide/cppguide.html#Explicit_Constructors
///
/// For the user-facing documentation see:
/// http://clang.llvm.org/extra/clang-tidy/checks/google-explicit-constructor.html
class ExplicitConstructorCheck : public ClangTidyCheck {
public:
  ExplicitConstructorCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context),
        IgnoredConstructors(utils::options::parseStringList(
            Options.get("IgnoredConstructors", ""))),
        IgnoredConversionOperators(utils::options::parseStringList(
            Options.get("IgnoredConversionOperators", ""))) {}
  void storeOptions(ClangTidyOptions::OptionMap &Opts) override;
  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.CPlusPlus;
  }
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

private:
  const std::vector<std::string> IgnoredConstructors;
  const std::vector<std::string> IgnoredConversionOperators;
};

} // namespace google
} // namespace tidy
} // namespace clang

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_GOOGLE_EXPLICITCONSTRUCTORCHECK_H
