#pragma once
#if defined(__clang__)
#pragma clang optimize off
#elif defined(__GNUC__)
#pragma GCC optimize ("O0")
#elif defined(_MSC_VER)
#pragma optimize("", off)
#endif
__attribute__((noinline, used, noipa, optnone, optimize("-O0"))) void  mage_alpha(int id){volatile int dummy = id; __asm__ volatile(""::"r"(id):"memory");};
