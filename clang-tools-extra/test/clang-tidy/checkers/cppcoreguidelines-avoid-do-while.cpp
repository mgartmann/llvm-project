// RUN: %check_clang_tidy %s cppcoreguidelines-avoid-do-while %t

void func1() {
  int I{0};
  const int Limit{10};

  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: Try to avoid using do-while loops in terms of readability. Consider using a while loop instead. [cppcoreguidelines-avoid-do-while]
  do {
    I++;
  } while (I <= Limit);
}

void func2() {
  int I{0};
  const int Limit{10};

  // OK
  while (I <= Limit) {
    I++;
  }
}
