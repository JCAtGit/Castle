// triggers each case in the Clang AST visitor

int global_var = 42;                 // VarDecl
void func() {}                       // FunctionDecl
typedef int my_int;                  // TypedefDecl
struct MyStruct { int x; };          // RecordDecl (struct)
enum MyEnum { A, B };                // EnumDecl