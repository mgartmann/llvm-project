// RUN: %check_clang_tidy %s cppcoreguidelines-declare-loop-variable-in-the-initializer %t

void forLoopFunction() {
  const int Limit{10};
  int I{0};

  // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: Prefer to declare a loop variable in the initializer part of a for-statement [cppcoreguidelines-declare-loop-variable-in-the-initializer]
  for (; I < Limit; I++) {
  }

  // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: Prefer to declare a loop variable in the initializer part of a for-statement [cppcoreguidelines-declare-loop-variable-in-the-initializer]
  for (I = 0; I < Limit; I++) {
  }

  for (int I{0}; I < Limit; I++) {
  }
}
