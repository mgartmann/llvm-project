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

void func3() {
#define MACRO \
  do {        \
    ;         \
  } while (true)

  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: Try to avoid using do-while loops in terms of readability. Consider using a while loop instead. [cppcoreguidelines-avoid-do-while]
  MACRO;

#undef MACRO
}

void func4() {
#define MACRO \
  do {        \
    ;         \
  } while (1)

  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: Try to avoid using do-while loops in terms of readability. Consider using a while loop instead. [cppcoreguidelines-avoid-do-while]
  MACRO;

#undef MACRO
}

void func5() {
#define MACRO \
  do {        \
    ;         \
  } while (false)

  // OK
  MACRO;

#undef MACRO
}

void func6() {
#define MACRO \
  do {        \
    ;         \
  } while (0)

  // OK
  MACRO;

#undef MACRO
}

void func7() {
#define MACRO \
  do {        \
    ;         \
  } while (nullptr)

  // OK
  MACRO;

#undef MACRO
}

void func8() {
#define MACRO \
  do {        \
    ;         \
  } while (__null)

  // OK
  MACRO;

#undef MACRO
}
