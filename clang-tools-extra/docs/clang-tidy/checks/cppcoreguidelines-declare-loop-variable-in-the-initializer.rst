.. title:: clang-tidy - cppcoreguidelines-declare-loop-variable-in-the-initializer

cppcoreguidelines-declare-loop-variable-in-the-initializer
==========================================================

Checks if a loop variable is declared in the initializer part of a for-statement.

This check implements the rule `ES.74 <https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#es74-prefer-to-declare-a-loop-variable-in-the-initializer-part-of-a-for-statement>`_ of the C++ Core Guidelines.

It does also cover parts of:
    - `ES.26 <https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#es26-dont-use-a-variable-for-two-unrelated-purposes>`_
    - `ES.5 <https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#es5-keep-scopes-small>`_
    - `ES.6 <https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#es6-declare-names-in-for-statement-initializers-and-conditions-to-limit-scope>`_


.. code-block:: c++

    void forLoopFunction() {
        const int Limit{10};
        int I{0};

        // Should print a warning
        for (; I < Limit; I++) {
        }

        // Should print a warning
        for (I = 0; I < Limit; I++) {
        }

        // Good
        for (int I{0}; I < Limit; I++) {
        }
    }

Reason for this Check:

It limits the visibility of the loop variable to the scope of the loop.
It avoids using the loop variable for other purposes after the loop.
