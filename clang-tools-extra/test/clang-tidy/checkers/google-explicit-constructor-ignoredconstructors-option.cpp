// RUN: %check_clang_tidy -check-suffix=DEFAULT %s google-explicit-constructor %t

// RUN: %check_clang_tidy -check-suffix=IGNORED %s \
// RUN: google-explicit-constructor %t -- \
// RUN: -config='{CheckOptions: [ \
// RUN:   {key: google-explicit-constructor.IgnoredConstructors, value: "A;B"} \
// RUN: ]}'

struct A {
  A() {}
  A(int x, int y) {}

  explicit A(void *x) {}
  explicit A(void *x, void *y) {}

  explicit A(const A &a) {}
  // CHECK-MESSAGES-DEFAULT: :[[@LINE-1]]:12: warning: copy constructor should not be declared explicit [google-explicit-constructor]
  // CHECK-FIXES-DEFAULT: {{^  }}A(const A &a) {}
  // IGNORED: Warning disabled with ConstructorWhitelist=A

  A(int x1);
  // CHECK-MESSAGES-DEFAULT: :[[@LINE-1]]:3: warning: single-argument constructors must be marked explicit to avoid unintentional implicit conversions [google-explicit-constructor]
  // CHECK-FIXES-DEFAULT: {{^  }}explicit A(int x1);
  // IGNORED: Warning disabled with ConstructorWhitelist=A

  A(double x2, double y = 3.14) {}
  // CHECK-MESSAGES-DEFAULT: :[[@LINE-1]]:3: warning: constructors that are callable with a single argument must be marked explicit to avoid unintentional implicit conversions [google-explicit-constructor]
  // CHECK-FIXES-DEFAULT: {{^  }}explicit A(double x2, double y = 3.14) {}
  // IGNORED: Warning disabled with ConstructorWhitelist=A

  template <typename... T>
  A(T &&...args);
  // CHECK-MESSAGES-DEFAULT: :[[@LINE-1]]:3: warning: constructors that are callable with a single argument
  // CHECK-FIXES-DEFAULT: {{^  }}explicit A(T &&...args);
  // IGNORED: Warning disabled with ConstructorWhitelist=A

  operator bool() const { return true; }
  // CHECK-MESSAGES-DEFAULT: :[[@LINE-1]]:3: warning: 'operator bool' must be marked explicit to avoid unintentional implicit conversions [google-explicit-constructor]
  // CHECK-MESSAGES-IGNORED: :[[@LINE-2]]:3: warning: 'operator bool' must be marked explicit to avoid unintentional implicit conversions [google-explicit-constructor]
  // CHECK-FIXES: {{^  }}explicit operator bool() const { return true; }

  operator double() const;
  // CHECK-MESSAGES-DEFAULT: :[[@LINE-1]]:3: warning: 'operator double' must be marked explicit to avoid unintentional implicit conversions [google-explicit-constructor]
  // CHECK-MESSAGES-IGNORED: :[[@LINE-2]]:3: warning: 'operator double' must be marked explicit to avoid unintentional implicit conversions [google-explicit-constructor]
  // CHECK-FIXES: {{^  }}explicit operator double() const;
};

inline A::A(int x1) {}

struct B {
  B() {}
  B(int x, int y) {}

  explicit B(void *x) {}
  explicit B(void *x, void *y) {}

  explicit B(const B &b) {}
  // CHECK-MESSAGES-DEFAULT: :[[@LINE-1]]:12: warning: copy constructor should not be declared explicit [google-explicit-constructor]
  // CHECK-FIXES-DEFAULT: {{^  }}B(const B &b) {}
  // IGNORED: Warning disabled with ConstructorWhitelist=B

  B(int x1);
  // CHECK-MESSAGES-DEFAULT: :[[@LINE-1]]:3: warning: single-argument constructors must be marked explicit to avoid unintentional implicit conversions [google-explicit-constructor]
  // CHECK-FIXES-DEFAULT: {{^  }}explicit B(int x1);
  // IGNORED: Warning disabled with ConstructorWhitelist=B

  B(double x2, double y = 3.14) {}
  // CHECK-MESSAGES-DEFAULT: :[[@LINE-1]]:3: warning: constructors that are callable with a single argument must be marked explicit to avoid unintentional implicit conversions [google-explicit-constructor]
  // CHECK-FIXES-DEFAULT: {{^  }}explicit B(double x2, double y = 3.14) {}
  // IGNORED: Warning disabled with ConstructorWhitelist=B

  template <typename... T>
  B(T &&...args);
  // CHECK-MESSAGES-DEFAULT: :[[@LINE-1]]:3: warning: constructors that are callable with a single argument
  // CHECK-FIXES-DEFAULT: {{^  }}explicit B(T &&...args);
  // IGNORED: Warning disabled with ConstructorWhitelist=B

  operator bool() const { return true; }
  // CHECK-MESSAGES-DEFAULT: :[[@LINE-1]]:3: warning: 'operator bool' must be marked explicit to avoid unintentional implicit conversions [google-explicit-constructor]
  // CHECK-MESSAGES-IGNORED: :[[@LINE-2]]:3: warning: 'operator bool' must be marked explicit to avoid unintentional implicit conversions [google-explicit-constructor]
  // CHECK-FIXES: {{^  }}explicit operator bool() const { return true; }

  operator double() const;
  // CHECK-MESSAGES-DEFAULT: :[[@LINE-1]]:3: warning: 'operator double' must be marked explicit to avoid unintentional implicit conversions [google-explicit-constructor]
  // CHECK-MESSAGES-IGNORED: :[[@LINE-2]]:3: warning: 'operator double' must be marked explicit to avoid unintentional implicit conversions [google-explicit-constructor]
  // CHECK-FIXES: {{^  }}explicit operator double() const;
};

struct C {
  C() {}
  C(int x, int y) {}

  explicit C(void *x) {}
  explicit C(void *x, void *y) {}

  explicit C(const C &c) {}
  // CHECK-MESSAGES-DEFAULT: :[[@LINE-1]]:12: warning: copy constructor should not be declared explicit [google-explicit-constructor]
  // CHECK-MESSAGES-IGNORED: :[[@LINE-2]]:12: warning: copy constructor should not be declared explicit [google-explicit-constructor]
  // CHECK-FIXES: {{^  }}C(const C &c) {}

  C(int x1);
  // CHECK-MESSAGES-DEFAULT: :[[@LINE-1]]:3: warning: single-argument constructors must be marked explicit to avoid unintentional implicit conversions [google-explicit-constructor]
  // CHECK-MESSAGES-IGNORED: :[[@LINE-2]]:3: warning: single-argument constructors must be marked explicit to avoid unintentional implicit conversions [google-explicit-constructor]
  // CHECK-FIXES: {{^  }}explicit C(int x1);

  C(double x2, double y = 3.14) {}
  // CHECK-MESSAGES-DEFAULT: :[[@LINE-1]]:3: warning: constructors that are callable with a single argument must be marked explicit to avoid unintentional implicit conversions [google-explicit-constructor]
  // CHECK-MESSAGES-IGNORED: :[[@LINE-2]]:3: warning: constructors that are callable with a single argument must be marked explicit to avoid unintentional implicit conversions [google-explicit-constructor]
  // CHECK-FIXES: {{^  }}explicit C(double x2, double y = 3.14) {}

  template <typename... T>
  C(T &&...args);
  // CHECK-MESSAGES-DEFAULT: :[[@LINE-1]]:3: warning: constructors that are callable with a single argument
  // CHECK-MESSAGES-IGNORED: :[[@LINE-2]]:3: warning: constructors that are callable with a single argument
  // CHECK-FIXES: {{^  }}explicit C(T &&...args);

  operator bool() const { return true; }
  // CHECK-MESSAGES-DEFAULT: :[[@LINE-1]]:3: warning: 'operator bool' must be marked explicit to avoid unintentional implicit conversions [google-explicit-constructor]
  // CHECK-MESSAGES-IGNORED: :[[@LINE-2]]:3: warning: 'operator bool' must be marked explicit to avoid unintentional implicit conversions [google-explicit-constructor]
  // CHECK-FIXES: {{^  }}explicit operator bool() const { return true; }

  operator double() const;
  // CHECK-MESSAGES-DEFAULT: :[[@LINE-1]]:3: warning: 'operator double' must be marked explicit to avoid unintentional implicit conversions [google-explicit-constructor]
  // CHECK-MESSAGES-IGNORED: :[[@LINE-2]]:3: warning: 'operator double' must be marked explicit to avoid unintentional implicit conversions [google-explicit-constructor]
  // CHECK-FIXES: {{^  }}explicit operator double() const;
};
