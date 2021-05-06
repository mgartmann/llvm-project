//===--- VirtualBaseClassDestructorCheck.h - clang-tidy ---------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_CPPCOREGUIDELINES_VIRTUALBASECLASSDESTRUCTORCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_CPPCOREGUIDELINES_VIRTUALBASECLASSDESTRUCTORCHECK_H

#include "../ClangTidyCheck.h"
#include <string>

namespace clang {
namespace tidy {
namespace cppcoreguidelines {

/// Finds base classes whose destructor is neither ``public`` and ``virtual``
/// nor ``protected`` and non-``virtual``.
/// For the user-facing documentation see:
/// http://clang.llvm.org/extra/clang-tidy/checks/cppcoreguidelines-virtual-base-class-destructor.html
class VirtualBaseClassDestructorCheck : public ClangTidyCheck {
  const unsigned IndentationWidth;
  CharSourceRange getVirtualKeywordRange(const CXXDestructorDecl &Destructor,
                                         const SourceManager &SM,
                                         const LangOptions &LangOpts) const;
  FixItHint
  generateUserDeclaredConstructor(const CXXRecordDecl &StructOrClass,
                                  const SourceManager &SourceManager) const;
  AccessSpecDecl *getPublicASDecl(const CXXRecordDecl &StructOrClass) const;
  std::string indent(int Indentation) const;

public:
  VirtualBaseClassDestructorCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context),
        IndentationWidth(Options.get("IndentationWidth", 4)) {}

  void storeOptions(ClangTidyOptions::OptionMap &Opts) override {
    Options.store(Opts, "IndentationWidth", IndentationWidth);
  }

  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.CPlusPlus;
  }

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace cppcoreguidelines
} // namespace tidy
} // namespace clang

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_CPPCOREGUIDELINES_VIRTUALBASECLASSDESTRUCTORCHECK_H
