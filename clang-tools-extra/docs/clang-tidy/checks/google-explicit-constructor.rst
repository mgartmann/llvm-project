.. title:: clang-tidy - google-explicit-constructor

google-explicit-constructor
===========================


Checks that constructors callable with a single argument and conversion
operators are marked explicit to avoid the risk of unintentional implicit
conversions.

Consider this example:

.. code-block:: c++

  struct S {
    int x;
    operator bool() const { return true; }
  };

  bool f() {
    S a{1};
    S b{2};
    return a == b;
  }

The function will return ``true``, since the objects are implicitly converted to
``bool`` before comparison, which is unlikely to be the intent.

The check will suggest inserting ``explicit`` before the constructor or
conversion operator declaration. However, copy and move constructors should not
be explicit, as well as constructors taking a single ``initializer_list``
argument.

This code:

.. code-block:: c++

  struct S {
    S(int a);
    explicit S(const S&);
    operator bool() const;
    ...

will become

.. code-block:: c++

  struct S {
    explicit S(int a);
    S(const S&);
    explicit operator bool() const;
    ...



See https://google.github.io/styleguide/cppguide.html#Explicit_Constructors

Options
-------

.. option:: IgnoredConstructors

    Non-explicit single-argument constructors in this semicolon-separated list
    will be ignored and will not trigger a warning. To ignore constructors of a
    class ``A``, the list would look as follows: ``A::A``. If class ``A``
    resided in a namespace ``Foo``, its constructors have to be ignored as
    follows: ``Foo::A::A``. The default list is empty. This option is used by
    this check's
    `cppcoreguidelines-explicit-constructor-and-conversion <cppcoreguidelines-explicit-constructor-and-conversion.html>`_
    alias to comply with the CppCoreGuidelines.

.. option:: IgnoredConversionOperators

    Non-explicit conversion operators in this semicolon-separated list will be
    ignored and will not trigger a warning. The list to ignore a conversion
    operator `operator B()` in class ``A`` would look as follows:
    ``A::operator B``. If an operator's class resides in any namespace,
    its namespace has to be specified as well. If class ``A`` resides in a
    namespace ``Foo``, its ``operator B()`` would be ignored as follows:
    ``Foo::A::operator B()``. Templated operators have to be ignored by their
    template parameter name displayed in this check's warning:

.. code-block:: c++

  struct A {
    template <typename Ty>
    operator Ty() const;
    // warning: 'operator type-parameter-0-1' must be marked explicit ...
    ...

    In the above example, ``operator Ty`` is ignored as follows: 
    ``Foo::A::operator type-parameter-0-0``. The default list is empty.

This check implements `C.46 <https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#c46-by-default-declare-single-argument-constructors-explicit>`_
and `C.164 <https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#c164-avoid-implicit-conversion-operators>`_
from the CppCoreGuidelines.
