// compile_fail.c – similar to compile_ok.c but deliberately fails to compile
// due to a conflict between `always_inline` and `noinline` attributes.
// The conflict is detected via a preprocessor error when both are enabled.

#pragma once

#include <stdio.h>

// ----------------------------------------------------------------------------
// Compiler‑specific optimization disabling (same as compile_ok.c)
#if defined(__clang__)
    #pragma clang optimize off
#elif defined(__GNUC__)
    #pragma GCC optimize ("O0")
#elif defined(_MSC_VER)
    #pragma optimize("", off)
#endif

// ----------------------------------------------------------------------------
// Macro definitions – we define both ALWAYS_INLINE and NOINLINE
// and then trigger an error if both are used together.

#define ATTR_ALWAYS_INLINE __attribute__((always_inline))
#define ATTR_NOINLINE      __attribute__((noinline))

// Conflict detection: if both attributes are requested, cause a compile error.
// (Here we assume both macros are always defined, so the error is always hit.)
#if defined(ATTR_ALWAYS_INLINE) && defined(ATTR_NOINLINE)
    #error "Conflict: always_inline and noinline cannot be used together"
#endif

// Additional attribute macros (same as compile_ok.c)
#define ATTR_USED_OPTNONE __attribute__((used)) __attribute__((optnone))

#if defined(__clang__)
    #define ATTR_NOIPA __attribute__((noipa))
#elif defined(__GNUC__)
    #define ATTR_NOIPA __attribute__((noipa))
#else
    #define ATTR_NOIPA
#endif

#define DECL_WITH_ATTRS(ret, name, args, attrs) attrs ret name args
#define FUNC_NAME_1 func1
#define FUNC_NAME_2 func2

// ----------------------------------------------------------------------------
// Function declarations – many are similar to compile_ok.c, but we also
// include a function that uses both conflicting attributes directly
// (though the preprocessor error will already stop compilation).

// This declaration uses both attributes, but the compiler will never see it
// because the #error stops preprocessing.
__attribute__((always_inline, noinline))
void func_conflict(void);

// Other declarations (copied from compile_ok.c for similarity)
ATTR_NOINLINE
void func_before_macro(void);

void func_after_macro(void) __attribute__((optimize("O0")));

DECL_WITH_ATTRS(void, func_encompassed, (void), __attribute__((noreturn)));

void FUNC_NAME_1(void) __attribute__((used));

__attribute__((noinline, used))
void func_multi_attrs(void);

ATTR_NOIPA
__attribute__((optnone, used))
void func_nonconflicting(void);

#define BODY_MACRO() do { printf("Hello from %s\n", __func__); } while (0)

void func_with_body_macro(void) {
    BODY_MACRO();
}

#define DECLARE_FUNC(name) \
    __attribute__((noinline)) \
    void name(void)

DECLARE_FUNC(func_via_macro);

// ----------------------------------------------------------------------------
// Main function (similar)
int main(void) {
    func_before_macro();
    func_after_macro();
    func_encompassed();
    func1();
    func_multi_attrs();
    func_nonconflicting();
    func_with_body_macro();
    func_via_macro();
    func_conflict(); // never reached due to preprocessor error
    return 0;
}

// ----------------------------------------------------------------------------
// Dummy definitions
void func_before_macro(void) {}
void func_after_macro(void) {}
void func_encompassed(void) {}
void func1(void) {}
void func_multi_attrs(void) {}
void func_nonconflicting(void) {}
void func_via_macro(void) {}
void func_conflict(void) {}