// RUN: %check_clang_tidy -check-suffix=DEFAULT %s \
// RUN: google-explicit-constructor %t -- \
// RUN: -config='{CheckOptions: [ \
// RUN: ]}'

// RUN: %check_clang_tidy -check-suffix=IGNORED %s \
// RUN: google-explicit-constructor %t -- \
// RUN: -config='{CheckOptions: [ \
// RUN:   {key: google-explicit-constructor.IgnoredConversionOperators, value: "A::operator bool;B::operator double;B::operator A"} \
// RUN: ]}'

struct A {
  A() {}
  A(int x, int y) {}

  explicit A(void *x) {}
  explicit A(void *x, void *y) {}

  A(int x1);
  // CHECK-MESSAGES-DEFAULT: :[[@LINE-1]]:3: warning: single-argument constructors must be marked explicit to avoid unintentional implicit conversions [google-explicit-constructor]
  // CHECK-MESSAGES-IGNORED: :[[@LINE-2]]:3: warning: single-argument constructors must be marked explicit to avoid unintentional implicit conversions [google-explicit-constructor]
  // CHECK-FIXES: {{^  }}explicit A(int x1);

  operator bool() const { return true; }
  // CHECK-MESSAGES-DEFAULT: :[[@LINE-1]]:3: warning: 'operator bool' must be marked explicit to avoid unintentional implicit conversions [google-explicit-constructor]
  // CHECK-FIXES-DEFAULT: {{^  }}explicit operator bool() const { return true; }

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

  B(int x1);
  // CHECK-MESSAGES-DEFAULT: :[[@LINE-1]]:3: warning: single-argument constructors must be marked explicit to avoid unintentional implicit conversions [google-explicit-constructor]
  // CHECK-MESSAGES-IGNORED: :[[@LINE-2]]:3: warning: single-argument constructors must be marked explicit to avoid unintentional implicit conversions [google-explicit-constructor]
  // CHECK-FIXES: {{^  }}explicit B(int x1);

  operator bool() const { return true; }
  // CHECK-MESSAGES-DEFAULT: :[[@LINE-1]]:3: warning: 'operator bool' must be marked explicit to avoid unintentional implicit conversions [google-explicit-constructor]
  // CHECK-MESSAGES-IGNORED: :[[@LINE-2]]:3: warning: 'operator bool' must be marked explicit to avoid unintentional implicit conversions [google-explicit-constructor]
  // CHECK-FIXES: {{^  }}explicit operator bool() const { return true; }

  operator double() const;
  // CHECK-MESSAGES-DEFAULT: :[[@LINE-1]]:3: warning: 'operator double' must be marked explicit to avoid unintentional implicit conversions [google-explicit-constructor]
  // CHECK-FIXES-DEFAULT: {{^  }}explicit operator double() const;

  operator A() const;
  // CHECK-MESSAGES-DEFAULT: :[[@LINE-1]]:3: warning: 'operator A' must be marked explicit to avoid unintentional implicit conversions [google-explicit-constructor]
  // CHECK-FIXES-DEFAULT: {{^  }}explicit operator A() const;
};

struct C {
  C() {}
  C(int x, int y) {}

  explicit C(void *x) {}
  explicit C(void *x, void *y) {}

  C(int x1);
  // CHECK-MESSAGES-DEFAULT: :[[@LINE-1]]:3: warning: single-argument constructors must be marked explicit to avoid unintentional implicit conversions [google-explicit-constructor]
  // CHECK-MESSAGES-IGNORED: :[[@LINE-2]]:3: warning: single-argument constructors must be marked explicit to avoid unintentional implicit conversions [google-explicit-constructor]
  // CHECK-FIXES: {{^  }}explicit C(int x1);

  operator bool() const { return true; }
  // CHECK-MESSAGES-DEFAULT: :[[@LINE-1]]:3: warning: 'operator bool' must be marked explicit to avoid unintentional implicit conversions [google-explicit-constructor]
  // CHECK-MESSAGES-IGNORED: :[[@LINE-2]]:3: warning: 'operator bool' must be marked explicit to avoid unintentional implicit conversions [google-explicit-constructor]
  // CHECK-FIXES: {{^  }}explicit operator bool() const { return true; }

  operator double() const;
  // CHECK-MESSAGES-DEFAULT: :[[@LINE-1]]:3: warning: 'operator double' must be marked explicit to avoid unintentional implicit conversions [google-explicit-constructor]
  // CHECK-MESSAGES-IGNORED: :[[@LINE-2]]:3: warning: 'operator double' must be marked explicit to avoid unintentional implicit conversions [google-explicit-constructor]
  // CHECK-FIXES: {{^  }}explicit operator double() const;

  operator A() const;
  // CHECK-MESSAGES-DEFAULT: :[[@LINE-1]]:3: warning: 'operator A' must be marked explicit to avoid unintentional implicit conversions [google-explicit-constructor]
  // CHECK-MESSAGES-IGNORED: :[[@LINE-2]]:3: warning: 'operator A' must be marked explicit to avoid unintentional implicit conversions [google-explicit-constructor]
  // CHECK-FIXES: {{^  }}explicit operator A() const;

  explicit operator B() const;
};
