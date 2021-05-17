.. title:: clang-tidy - cppcoreguidelines-avoid-do-while

cppcoreguidelines-avoid-do-while
================================

Checks if a ``do-while`` loop exists and flags it.

Using a ``while`` loop instead of a ``do-while`` could improve readability and prevents overlooking the condition at the end.

Usages of a ``do-while`` loop inside a macro definition are excluded by this check, if the condition is either ``false``, ``0``, ``nullptr`` or ``__null``.

.. code-block:: c++

  void func1() {
    int I{0};
    const int Limit{10};

    // Consider using a while loop
    do {
      I++;
    } while (I <= Limit);
  }

  void func2() {
    int I{0};
    const int Limit{10};

    // Better
    while (I <= Limit) {
      I++;
    }
  }

This check implements rule `ES.75 <https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#es75-avoid-do-statements>`_ of the C++ Core Guidelines.
