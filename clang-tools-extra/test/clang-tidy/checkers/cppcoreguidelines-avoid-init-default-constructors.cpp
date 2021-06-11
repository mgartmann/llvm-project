// RUN: %check_clang_tidy %s cppcoreguidelines-avoid-init-default-constructors %t

class NoDefaultCtor {
  int z;

  NoDefaultCtor(int a) : z{1} {}
};

class HasBody {
  int z;

  HasBody() : z{1} { ; }
};

class OneParameterOneInitList {
  int z;
  int y;

  OneParameterOneInitList(int a = 1) : z{a}, y{1} {}
};

// Test needed because class member initializations are listed as ``cxxCtorInitializer`` in clang's AST
class OnlyInClassMemberInit {
  int z{1};
  int y = 1;

  OnlyInClassMemberInit() {}
};

// User may wants explicitly overwrite the class member value in the constructor
class ExplicitOverwrite {
  int z{1};

  ExplicitOverwrite() : z{2} {}
};

class HasBodyInit {
  int z;

  HasBodyInit() {
    z = 1;
  }
};

class OneInitList {
  int z;

  OneInitList() : z{1} {}
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: Default constructors like 'OneInitList' should do more than just initialize member variables [cppcoreguidelines-avoid-init-default-constructors]
};

class OneDirectInit {
  int z;

  OneDirectInit() : z(1) {}
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: Default constructors like 'OneDirectInit' should do more than just initialize member variables [cppcoreguidelines-avoid-init-default-constructors]
};

class OneInitListOneDirectInit {
  int z;
  int y;
  int w;

  OneInitListOneDirectInit() : z{1}, y(2) {}
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: Default constructors like 'OneInitListOneDirectInit' should do more than just initialize member variables [cppcoreguidelines-avoid-init-default-constructors]
};
