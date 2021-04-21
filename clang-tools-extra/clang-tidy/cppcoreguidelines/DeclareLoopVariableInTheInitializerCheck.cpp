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

class IsOutsideForStmtVisitor
    : public RecursiveASTVisitor<IsOutsideForStmtVisitor> {
  friend class RecursiveASTVisitor<IsOutsideForStmtVisitor>;

public:
  IsOutsideForStmtVisitor(const ForStmt *ForStmt, const VarDecl *VarDecl,
                          const MatchFinder::MatchResult &Result)
      : ForLoop(ForStmt), Declaration(VarDecl), externResult(Result),
        IsOutsideForStmt(false) {}

  bool hasDeclRefExpOutside(const TranslationUnitDecl *TransUnit) {
    // Find out more about TraverseDecl
    TraverseDecl(const_cast<TranslationUnitDecl *>(TransUnit));
    return IsOutsideForStmt;
  }

private:
  bool VisitDeclRefExpr(DeclRefExpr *D) {
    if (const VarDecl *To = dyn_cast<VarDecl>(D->getDecl())) {
      if (To == Declaration) {
        IsOutsideForStmt =
            !IsInsideForLoop(externResult, DynTypedNode::create(*D));
        if (IsOutsideForStmt) {
          return false;
        }
      }
    }
    return true;
  }

  // IsInsideMatchedForLoop
  bool IsInsideForLoop(const MatchFinder::MatchResult &Result,
                       const DynTypedNode &Node) {
    const auto *AsForStmt = Node.get<ForStmt>();

    if (AsForStmt && AsForStmt == ForLoop) {
      return true;
    }

    return llvm::any_of(Result.Context->getParents(Node),
                        [&](const DynTypedNode &Parent) {
                          return IsInsideForLoop(Result, Parent);
                        });
  }

  const ForStmt *ForLoop;
  const VarDecl *Declaration;
  const MatchFinder::MatchResult &externResult;
  bool IsOutsideForStmt;
};

void DeclareLoopVariableInTheInitializerCheck::registerMatchers(
    MatchFinder *Finder) {
  Finder->addMatcher(
      declRefExpr(
          hasAncestor(forStmt().bind("ForStmt")), // is in for statement
          anyOf(hasAncestor(unaryOperator()),     // gets modified
                hasAncestor(
                    binaryOperator(hasOperatorName("=")))), // gets modified
          to(varDecl(unless(hasAncestor(forStmt(equalsBoundNode("ForStmt")))))
                 .bind("VarDecl"))) // is declared outside a for statement
          .bind("DeclRefExpr"),
      this);
}

void DeclareLoopVariableInTheInitializerCheck::check(
    const MatchFinder::MatchResult &Result) {
  const auto *MatchedForStmt = Result.Nodes.getNodeAs<ForStmt>("ForStmt");
  const auto *MatchedVarDecl = Result.Nodes.getNodeAs<VarDecl>("VarDecl");
  const auto *MatchedDeclRefExpr =
      Result.Nodes.getNodeAs<DeclRefExpr>("DeclRefExpr");

  // Maybe just the next Compound Stmt fo the VarDecl?
  const auto TransUnit = Result.Context->getTranslationUnitDecl();

  if (IsOutsideForStmtVisitor(MatchedForStmt, MatchedVarDecl, Result)
          .hasDeclRefExpOutside(TransUnit)) {
    return;
  }

  diag(MatchedForStmt->getBeginLoc(),
       "The variable %0 gets modified in this for loop statment, but is not "
       "declared in it. The Variable is also not needed outside the loop.")
      << MatchedDeclRefExpr->getDecl();
}

} // namespace cppcoreguidelines
} // namespace tidy
} // namespace clang
