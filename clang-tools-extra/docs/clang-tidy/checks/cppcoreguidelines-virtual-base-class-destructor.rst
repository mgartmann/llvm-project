.. title:: clang-tidy - cppcoreguidelines-virtual-base-class-destructor

cppcoreguidelines-virtual-base-class-destructor
===============================================

Finds base classes and structs whose destructor is neither public and virtual nor protected and non-virtual.
A base class's destructor should be specified in one of these ways to prevent undefined behaviour.

This check implements `C.35 <https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#c35-a-base-class-destructor-should-be-either-public-and-virtual-or-protected-and-non-virtual>`_ from the CppCoreGuidelines.

For example, the following classes/structs get flagged by the check since they violate guideline C.35:

.. code-block:: c++

  struct Foo {  // NOK, protected destructor should not be virtual
         ~~~
    virtual void f();
  protected:
    virtual ~Foo(){};
  };

  class Bar {    // NOK, public destructor should be virtual
        ~~~
    virtual void f();
  public:
    ~Bar(){};
  };

This would be rewritten to look like this:

.. code-block:: c++

  struct Foo {  // OK, destructor is not virtual anymore
         ~~~
    virtual void f();
  protected:
    ~Foo(){};
  };

  class Bar {    // OK, destructor is now virtual
        ~~~
    virtual void f();
  public:
    virtual ~Bar(){};
  };

Options
-------

.. option:: IndentationWidth

   An unsigned integer specifying how wide an indentation is in terms of blank spaces.
   This option is used for classes/structs with implicit destructors, where a
   used-declared destructor needs to be inserted.


