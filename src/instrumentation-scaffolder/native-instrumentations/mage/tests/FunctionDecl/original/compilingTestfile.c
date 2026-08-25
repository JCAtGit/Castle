// compile_ok.c – a self‑contained file with various function attributes and macros.
// This file compiles without errors.

#pragma once

#include <stdio.h>

// ----------------------------------------------------------------------------
// Compiler‑specific optimization disabling (applies to subsequent functions)
#if defined(__clang__)
    #pragma clang optimize off
#elif defined(__GNUC__)
    #pragma GCC optimize ("O0")
#elif defined(_MSC_VER)
    #pragma optimize("", off)
#endif

// ----------------------------------------------------------------------------
// Macro definitions for attribute lists and other uses.
// These macros may appear before, after, inside, or encompassing declarations.

// A macro that expands to a single attribute (placed before a declaration)
#define ATTR_NOINLINE __attribute__((noinline))

// A macro that expands to multiple attributes (placed after a declaration)
#define ATTR_USED_OPTNONE __attribute__((used)) __attribute__((optnone))

// A macro that expands to an attribute with compiler-specific guards
#if defined(__clang__)
    #define ATTR_NOIPA __attribute__((noipa))   // Clang supports noipa
#elif defined(__GNUC__)
    #define ATTR_NOIPA __attribute__((noipa))
#else
    #define ATTR_NOIPA
#endif

// A macro that encompasses an entire declaration (used as a prefix)
#define DECL_WITH_ATTRS(ret, name, args, attrs) attrs ret name args

// A macro used inside a declaration to modify the function name
#define FUNC_NAME_1 func1
#define FUNC_NAME_2 func2

// ----------------------------------------------------------------------------
// Function declarations with various attributes and macro placements

// Macro BEFORE the declaration
ATTR_NOINLINE
void func_before_macro(void);

// Macro AFTER the declaration
void func_after_macro(void) __attribute__((optimize("O0")));

// Macro ENCOMPASSING the declaration (using the DECL_WITH_ATTRS macro)
DECL_WITH_ATTRS(void, func_encompassed, (void), __attribute__((noreturn)));

// Macro INSIDE the declaration (using FUNC_NAME_1)
void FUNC_NAME_1(void) __attribute__((used));

// Multiple attributes in a list (compatible: noinline and used)
__attribute__((noinline, used))
void func_multi_attrs(void);

// Attributes that do not conflict with each other:
//   - noipa   (prevents interprocedural analysis)
//   - optnone (disables optimization for this function)
//   - used    (keeps the function even if unused)
ATTR_NOIPA
__attribute__((optnone, used))
void func_nonconflicting(void);

// ----------------------------------------------------------------------------
// Additional macros that appear inside the function body (for demonstration)
#define BODY_MACRO() do { printf("Hello from %s\n", __func__); } while (0)

// Function definition to exercise the macros
void func_with_body_macro(void) {
    BODY_MACRO();
}

// ----------------------------------------------------------------------------
// A macro that expands to a declaration with attributes inside the macro itself
#define DECLARE_FUNC(name) \
    __attribute__((noinline)) \
    void name(void)

DECLARE_FUNC(func_via_macro);

// ----------------------------------------------------------------------------
// Main function – required for linking
int main(void) {
    func_before_macro();
    func_after_macro();
    func_encompassed();
    func1();
    func_multi_attrs();
    func_nonconflicting();
    func_with_body_macro();
    func_via_macro();
    return 0;
}

// ----------------------------------------------------------------------------
// Dummy definitions to satisfy the linker (since the functions are declared but
// not defined, and some have 'used' attribute to keep them)
void func_before_macro(void) {}
void func_after_macro(void) {}
void func_encompassed(void) {}
void func1(void) {}
void func_multi_attrs(void) {}
void func_nonconflicting(void) {}
void func_via_macro(void) {}
