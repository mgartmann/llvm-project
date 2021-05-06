// RUN: %check_clang_tidy %s cppcoreguidelines-virtual-base-class-destructor %t

// CHECK-MESSAGES: :[[@LINE+1]]:8: warning: destructor of struct 'PrivateVirtualBaseStruct' is private and prevents using the type. Consider making it public and virtual or protected and non-virtual [cppcoreguidelines-virtual-base-class-destructor]
struct PrivateVirtualBaseStruct {
    virtual void f();
private:
    virtual ~PrivateVirtualBaseStruct(){};
};

struct PublicVirtualBaseStruct { // OK
    virtual void f();
    virtual ~PublicVirtualBaseStruct(){};
};

// CHECK-MESSAGES: :[[@LINE+1]]:8: warning: destructor of struct 'ProtectedVirtualBaseStruct' is protected and virtual. It should either be public and virtual or protected and non-virtual [cppcoreguidelines-virtual-base-class-destructor]
struct ProtectedVirtualBaseStruct {
    virtual void f();
protected:
    virtual ~ProtectedVirtualBaseStruct(){};
    // CHECK-FIXES: ~ProtectedVirtualBaseStruct(){};
};

// CHECK-MESSAGES: :[[@LINE+1]]:8: warning: destructor of struct 'PrivateNonVirtualBaseStruct' is private and prevents using the type. Consider making it public and virtual or protected and non-virtual [cppcoreguidelines-virtual-base-class-destructor]
struct PrivateNonVirtualBaseStruct {
    virtual void f();
private:
    ~PrivateNonVirtualBaseStruct(){};
};

// CHECK-MESSAGES: :[[@LINE+1]]:8: warning: destructor of struct 'PublicNonVirtualBaseStruct' is public and non-virtual. It should either be public and virtual or protected and non-virtual [cppcoreguidelines-virtual-base-class-destructor]
struct PublicNonVirtualBaseStruct {
    virtual void f();
    ~PublicNonVirtualBaseStruct(){};
    // CHECK-FIXES: virtual ~PublicNonVirtualBaseStruct(){};
};

struct PublicNonVirtualNonBaseStruct { // OK according to C.35, since this struct does not have any virtual methods.
    void f();
    ~PublicNonVirtualNonBaseStruct(){};
};

// CHECK-MESSAGES: :[[@LINE+3]]:8: warning: destructor of struct 'PublicImplicitNonVirtualBaseStruct' is public and non-virtual. It should either be public and virtual or protected and non-virtual [cppcoreguidelines-virtual-base-class-destructor]
// CHECK-FIXES: struct PublicImplicitNonVirtualBaseStruct {
// CHECK-FIXES-NEXT: virtual ~PublicImplicitNonVirtualBaseStruct() = default;
struct PublicImplicitNonVirtualBaseStruct {
    virtual void f();
};

// CHECK-MESSAGES: :[[@LINE+4]]:8: warning: destructor of struct 'PublicASImplicitNonVirtualBaseStruct' is public and non-virtual. It should either be public and virtual or protected and non-virtual [cppcoreguidelines-virtual-base-class-destructor]
// CHECK-FIXES: struct PublicASImplicitNonVirtualBaseStruct {
// CHECK-FIXES-NEXT: virtual ~PublicASImplicitNonVirtualBaseStruct() = default;
// CHECK-FIXES-NEXT: private:
struct PublicASImplicitNonVirtualBaseStruct {
private:
    virtual void f();
};

struct ProtectedNonVirtualBaseStruct { // OK
    virtual void f();
protected:
    ~ProtectedNonVirtualBaseStruct(){};
};

// CHECK-MESSAGES: :[[@LINE+1]]:7: warning: destructor of class 'PrivateVirtualBaseClass' is private and prevents using the type. Consider making it public and virtual or protected and non-virtual [cppcoreguidelines-virtual-base-class-destructor]
class PrivateVirtualBaseClass {
    virtual void f();
    virtual ~PrivateVirtualBaseClass(){};
};

class PublicVirtualBaseClass { // OK
    virtual void f();
public:
    virtual ~PublicVirtualBaseClass(){};
};

// CHECK-MESSAGES: :[[@LINE+1]]:7: warning: destructor of class 'ProtectedVirtualBaseClass' is protected and virtual. It should either be public and virtual or protected and non-virtual [cppcoreguidelines-virtual-base-class-destructor]
class ProtectedVirtualBaseClass {
    virtual void f();
protected:
    virtual ~ProtectedVirtualBaseClass(){};
    // CHECK-FIXES: ~ProtectedVirtualBaseClass(){};
};

// CHECK-MESSAGES: :[[@LINE+4]]:7: warning: destructor of class 'PublicImplicitNonVirtualBaseClass' is public and non-virtual. It should either be public and virtual or protected and non-virtual [cppcoreguidelines-virtual-base-class-destructor]
// CHECK-FIXES: public:
// CHECK-FIXES-NEXT: virtual ~PublicImplicitNonVirtualBaseClass() = default;
// CHECK-FIXES-NEXT: };
class PublicImplicitNonVirtualBaseClass {
    virtual void f();
};

// CHECK-MESSAGES: :[[@LINE+5]]:7: warning: destructor of class 'PublicASImplicitNonVirtualBaseClass' is public and non-virtual. It should either be public and virtual or protected and non-virtual [cppcoreguidelines-virtual-base-class-destructor]
// CHECK-FIXES: public:
// CHECK-FIXES-NEXT: virtual ~PublicASImplicitNonVirtualBaseClass() = default;
// CHECK-FIXES-NEXT: int foo = 42;
// CHECK-FIXES-NEXT: };
class PublicASImplicitNonVirtualBaseClass {
    virtual void f();
public:
    int foo = 42;
};

// CHECK-MESSAGES: :[[@LINE+1]]:7: warning: destructor of class 'PublicNonVirtualBaseClass' is public and non-virtual. It should either be public and virtual or protected and non-virtual [cppcoreguidelines-virtual-base-class-destructor]
class PublicNonVirtualBaseClass {
    virtual void f();
public:
    ~PublicNonVirtualBaseClass(){};
    // CHECK-FIXES: virtual ~PublicNonVirtualBaseClass(){};
};

class PublicNonVirtualNonBaseClass { // OK accoring to C.35, since this class does not have any virtual methods.
    void f();
public:
    ~PublicNonVirtualNonBaseClass(){};
};

class ProtectedNonVirtualClass { // OK
    virtual void f();
protected:
    ~ProtectedNonVirtualClass(){};
};