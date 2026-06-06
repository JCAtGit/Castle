// demo_struct_decls.cpp
// Compile: clang++ -c -std=c++17 demo_struct_decls.cpp

struct MyStruct {
  // 1. FieldDecl: ordinary non-static data member
  int normalField;

  // 2. IndirectFieldDecl: anonymous struct/union inside the record
  union {
    float anonymousUnionField; // becomes IndirectFieldDecl
    double anotherUnionField;
  };

  struct {
    char anonymousStructChar; // becomes IndirectFieldDecl as well
    bool anonymousStructBool;
  };

  // 3. VarDecl: static data member
  static int staticMember;

  // 4. CXXMethodDecl: member function
  void method() {}

  // 5. TypedefDecl
  typedef int MyInt;

  // 6. TypeAliasDecl (C++11)
  using MyDouble = double;

  // 7. EnumDecl: nested enumeration
  enum Color { Red, Green, Blue };

  // 8. RecordDecl: nested struct
  struct Nested {
    int nestedField;
  };

  // 9. FriendDecl (can be a function or another class)
  friend void friendFunction();

  // 10. StaticAssertDecl
  static_assert(sizeof(int) >= 4, "int too small");

  // 11. Additional: you can also have a friend class
  friend class FriendClass;
};

// Definition of the static member (required)
int MyStruct::staticMember = 42;

// Friend function definition (just for completeness)
void friendFunction() {}

// Dummy friend class
class FriendClass{};