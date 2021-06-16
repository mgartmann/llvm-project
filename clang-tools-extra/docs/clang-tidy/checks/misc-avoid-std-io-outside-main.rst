.. title:: clang-tidy - misc-avoid-std-io-outside-main

misc-avoid-std-io-outside-main
=============

Finds predefined standard stream objects like ``cin``, ``wcin``, ``cout``,
``wcout``, ``cerr`` or ``wcerr``, that are used outside the ``main``
function. It also finds uses of ``cstdio`` and ``stdio.h`` functions like
``printf()`` outside the ``main`` function.

For instance, in the following code, the use of ``std::cout`` and ``printf()``
outside of ``main()`` would get flagged whereas the use of them inside
``main()`` is not flagged:

.. code-block:: c++

  #include <iostream>
  #include <cstdio>

  void some_function() {
    std::cout << "This triggers the check."; // NOK
    std::printf("This triggers the check."); // NOK
  }

  int main() {
    std::cout << "This does not trigger the check."; // OK
    std::printf("This does not trigger the check."); // OK
  }

Since the predefined standard stream objects are global objects, their use
outside of ``main()`` worsens a program's testability and is thus discouraged.
Instead, those objects should only be used inside ``main()``. They can then be
passed as arguments to other functions like so:

.. code-block:: c++

  #include <iostream>

  void some_function(std::istream & in, std::ostream & out) {
    out << "This does not trigger the check.";

    int i{0};
    in >> i;
  }

  int main() {
    some_function(std::cin, std::cout);
  }

In contrast to using ``std::cin`` and ``std::cout`` directly, in the above
example, it is possible to inject mocked stream objects into
``some_function()`` during testing.
