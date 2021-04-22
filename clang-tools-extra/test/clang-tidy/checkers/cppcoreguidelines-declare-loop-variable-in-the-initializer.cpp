// RUN: %check_clang_tidy %s cppcoreguidelines-declare-loop-variable-in-the-initializer %t

const int Limit{10};

void func1() {
  // CHECK-MESSAGES: :[[@LINE+2]]:7: warning: Variable 'A' is only modified in a for statement and not used elsewhere. Consider declaring it inside the for statement. [cppcoreguidelines-declare-loop-variable-in-the-initializer]
  // CHECK-MESSAGES: :11:5: note: Variable gets modified here
  int A{0};

  for (int I{0}; I < Limit; I++) {
    A = 3;
  }
}

void func2() {
  // CHECK-MESSAGES: :[[@LINE+2]]:7: warning: Variable 'A' is only modified in a for statement and not used elsewhere. Consider declaring it inside the for statement. [cppcoreguidelines-declare-loop-variable-in-the-initializer]
  // CHECK-MESSAGES: :21:5: note: Variable gets modified here
  int A{0};

  for (int I{0}; I < Limit; I++) {
    A += 3;
  }
}

void func3() {
  // CHECK-MESSAGES: :[[@LINE+2]]:7: warning: Variable 'I' is only modified in a for statement and not used elsewhere. Consider declaring it inside the for statement. [cppcoreguidelines-declare-loop-variable-in-the-initializer]
  // CHECK-MESSAGES: :30:7: note: Variable gets modified here
  int I{0};

  for (I = 1; I < Limit; I++) {
  }
}

void func4() {
  // CHECK-MESSAGES: :[[@LINE+2]]:7: warning: Variable 'I' is only modified in a for statement and not used elsewhere. Consider declaring it inside the for statement. [cppcoreguidelines-declare-loop-variable-in-the-initializer]
  // CHECK-MESSAGES: :38:20: note: Variable gets modified here
  int I{0};

  for (; I < Limit; I++) {
  }
}

void func5() {
  // CHECK-MESSAGES: :[[@LINE+2]]:7: warning: Variable 'I' is only modified in a for statement and not used elsewhere. Consider declaring it inside the for statement. [cppcoreguidelines-declare-loop-variable-in-the-initializer]
  // CHECK-MESSAGES: :46:33: note: Variable gets modified here
  int I{0};

  for (int Unused{0}; I < Limit; I++) {
  }
}

void func6() {
  // OK
  int A{0};

  for (int I{0}; I < Limit; I++) {
    const int B{A};
  }
}
