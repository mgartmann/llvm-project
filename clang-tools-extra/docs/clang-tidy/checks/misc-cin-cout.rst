.. title:: clang-tidy - misc-cin-cout

misc-cin-cout
=============

The check diagnoses when a predefined standard stream object (i.e., ``cin``, ``wcin``, 
``cout``, ``wcout``, ``cerr``, ``wcerr``, ``clog`` and ``wclog``) is used outside the ``main`` function.

For instance, in the following code, the use of ``std::cout`` outside of ``main()`` would get
flagged whereas the use of ``std::cout`` inside ``main()`` is not flagged:

.. code-block:: c++

  #include <iostream>

  void some_function() { 
    std::cout << "This triggers the check."; 
         ~~~~ 
  } 

  int main() {
    std::cout << "This does not trigger the check.";
  }

Since the predefined standard stream objects are global objects, their use outside of ``main()`` worsens a 
program's testability and is thus discouraged. Instead, those objects should only be used inside ``main()``.
They can then be passed as arguments to other functions like so:

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

In contrast to using ``std::cin`` and ``std::cout`` directly, in the above example, it is possible to inject 
mocked stream objects into ``some_function()`` during testing.