.. title:: clang-tidy - cppcoreguidelines-declare-loop-variable-in-the-initializer

cppcoreguidelines-declare-loop-variable-in-the-initializer
==========================================================

Finds variables that are modified inside a ``for`` statement, are not used elsewhere
and are declared outside the ``for`` statement.

.. code-block:: c++

  const int Limit{10};

  void func1() {
    // Should print a warning
    int A{0};

    for(int I{0}; I < Limit; I++) {
      A = 3;
    }
  }

  void func2() {
    // Should print a warning
    int I{0};

    for(I = 1; I < Limit; I++) {}
  }

  void func3() {
    // Should print a warning
    int I{0};

    for(int Unused{0}; I < Limit; I++) {}
  }

  void func4() {
    // OK
    int A{0};

    for(int I{0}; I < Limit; I++) {
      const int B{A};
    }
  }

This check implements the rule `ES.74 <https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#es74-prefer-to-declare-a-loop-variable-in-the-initializer-part-of-a-for-statement>`_ of the C++ Core Guidelines.

It does also cover parts of:
    - `ES.5 <https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#es5-keep-scopes-small>`_
    - `ES.6 <https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#es6-declare-names-in-for-statement-initializers-and-conditions-to-limit-scope>`_
