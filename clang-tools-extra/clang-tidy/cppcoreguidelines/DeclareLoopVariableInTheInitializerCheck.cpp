//===--- DeclareLoopVariableInTheInitializerCheck.cpp - clang-tidy --------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "DeclareLoopVariableInTheInitializerCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace cppcoreguidelines {

class OutsideForStmtVisitor
    : public RecursiveASTVisitor<OutsideForStmtVisitor> {
  friend class RecursiveASTVisitor<OutsideForStmtVisitor>;

public:
  OutsideForStmtVisitor(const ForStmt *ForStmt, const VarDecl *VarDecl,
                        const MatchFinder::MatchResult &Result)
      : MatchedForStmt(ForStmt), MatchedDecl(VarDecl), MatchedResult(Result),
        IsOutsideMatchedForStmt(false) {}

  bool hasDeclRefExpOutside(const TranslationUnitDecl *TransUnit) {
    TraverseDecl(const_cast<TranslationUnitDecl *>(TransUnit));
    return IsOutsideMatchedForStmt;
  }

private:
  bool VisitDeclRefExpr(DeclRefExpr *D) {
    if (const VarDecl *To = dyn_cast<VarDecl>(D->getDecl())) {
      if (To == MatchedDecl &&
          !IsInsideMatchedForStmt(MatchedResult, DynTypedNode::create(*D))) {
        IsOutsideMatchedForStmt = true;
        return false;
      }
    }
    return true;
  }

  bool IsInsideMatchedForStmt(const MatchFinder::MatchResult &Result,
                              const DynTypedNode &Node) {
    const auto *PossibleForStmt = Node.get<ForStmt>();

    if (PossibleForStmt && PossibleForStmt == MatchedForStmt) {
      return true;
    }

    return llvm::any_of(Result.Context->getParents(Node),
                        [&](const DynTypedNode &Parent) {
                          return IsInsideMatchedForStmt(Result, Parent);
                        });
  }

  const ForStmt *MatchedForStmt;
  const VarDecl *MatchedDecl;
  const MatchFinder::MatchResult &MatchedResult;
  bool IsOutsideMatchedForStmt;
};

void DeclareLoopVariableInTheInitializerCheck::registerMatchers(
    MatchFinder *Finder) {
  Finder->addMatcher(
      declRefExpr(
          hasAncestor(forStmt().bind("ForStmt")),
          anyOf(hasAncestor(unaryOperator().bind("Operator")),
                hasAncestor(
                    binaryOperator(isAssignmentOperator()).bind("Operator"))),
          to(varDecl(unless(hasAncestor(forStmt(equalsBoundNode("ForStmt")))))
                 .bind("VarDecl"))),
      this);
}

void DeclareLoopVariableInTheInitializerCheck::check(
    const MatchFinder::MatchResult &Result) {
  const auto *MatchedForStmt = Result.Nodes.getNodeAs<ForStmt>("ForStmt");
  const auto *MatchedExprOperator = Result.Nodes.getNodeAs<Expr>("Operator");
  const auto *MatchedVarDecl = Result.Nodes.getNodeAs<VarDecl>("VarDecl");
  const auto TransUnit = Result.Context->getTranslationUnitDecl();

  if (OutsideForStmtVisitor(MatchedForStmt, MatchedVarDecl, Result)
          .hasDeclRefExpOutside(TransUnit)) {
    return;
  }

  diag(MatchedVarDecl->getLocation(),
       "Variable %0 is only modified in a for statement and not used "
       "elsewhere. Consider declaring it inside the for statement.")
      << MatchedVarDecl;
  diag(MatchedExprOperator->getBeginLoc(), "Variable gets modified here",
       DiagnosticIDs::Note);
}

} // namespace cppcoreguidelines
} // namespace tidy
} // namespace clang
