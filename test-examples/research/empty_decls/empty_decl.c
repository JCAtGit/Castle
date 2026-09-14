#include "MageAlpha.h"
__attribute__((noinline, used, noipa, optnone, optimize("-O0"))) void  mage_alpha0(int id){volatile int dummy = id; __asm__ volatile(""::"r"(id):"memory");};
__attribute__((noinline, used, noipa, optnone, optimize("-O0"))) void  mage_alpha1(int id){volatile int dummy = id; __asm__ volatile(""::"r"(id):"memory");};
__attribute__((noinline, used, noipa, optnone, optimize("-O0"))) void  mage_alpha2(int id){volatile int dummy = id; __asm__ volatile(""::"r"(id):"memory");};
__attribute__((noinline, used, noipa, optnone, optimize("-O0"))) void  mage_alpha3(int id){volatile int dummy = id; __asm__ volatile(""::"r"(id):"memory");};
__attribute__((noinline, used, noipa, optnone, optimize("-O0"))) void  mage_alpha4(int id){volatile int dummy = id; __asm__ volatile(""::"r"(id):"memory");};
__attribute__((noinline, used, noipa, optnone, optimize("-O0"))) void  mage_alpha5(int id){volatile int dummy = id; __asm__ volatile(""::"r"(id):"memory");};
__attribute__((noinline, used, noipa, optnone, optimize("-O0"))) void  mage_alpha6(int id){volatile int dummy = id; __asm__ volatile(""::"r"(id):"memory");};
__attribute__((noinline, used, noipa, optnone, optimize("-O0"))) void  mage_alpha7(int id){volatile int dummy = id; __asm__ volatile(""::"r"(id):"memory");};
/*
 * This is a RANDOMLY GENERATED PROGRAM.
 *
 * Generator: csmith 2.4.0
 * Git version: unknown
 * Options:   --seed 3 --output /home/user/Desktop/CCastle/CCastleData/CodebaseScaffolder/csmith/3.c
 * Seed:      3
 */

#include "csmith.h"


static long __undefined;

/* --- Struct/Union Declarations --- */
union U1 {
   int8_t  f0;
   unsigned f1 : 14;
   unsigned f2 : 14;
   uint64_t  f3;
   volatile int64_t  f4;
};

/* --- GLOBAL VARIABLES --- */
static union U1 g_2[6] = {{0xB3L},{1L},{0xB3L},{0xB3L},{1L},{0xB3L}};


/* --- FORWARD DECLARATIONS --- */
static union U1  func_1(void);


/* --- FUNCTIONS --- */
/* ------------------------------------------ */
/* 
 * reads : g_2
 * writes:
 */
static union U1 __attribute__((optimize("O0"), optnone))  func_1(void)
{ /* block id: 0 */
    return g_2[0];
}




/* ---------------------------------------- */
int __attribute__((optimize("O0"), optnone)) main (int argc, char* argv[])
{
    mage_alpha0(0); 
    int i;
    mage_alpha1(1); 
    int print_hash_value = 0;
    mage_alpha2(2); 
    if (argc == 2 && strcmp(argv[1], "1") == 0) print_hash_value = 1;
    mage_alpha3(3); 
    platform_main_begin();
    mage_alpha4(4); 
    crc32_gentab();
    mage_alpha5(5); 
    func_1();
    mage_alpha6(6); 
    for (i = 0; i < 6; i++)
    {
        transparent_crc(g_2[i].f0, "g_2[i].f0", print_hash_value);
        mage_alpha7(7); 
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}

/************************ statistics *************************
XXX max struct depth: 0
breakdown:
   depth: 0, occurrence: 1
XXX total union variables: 1

XXX non-zero bitfields defined in structs: 2
XXX zero bitfields defined in structs: 0
XXX const bitfields defined in structs: 0
XXX volatile bitfields defined in structs: 0
XXX structs with bitfields in the program: 1
breakdown:
   indirect level: 0, occurrence: 1
XXX full-bitfields structs in the program: 0
breakdown:
XXX times a bitfields struct's address is taken: 0
XXX times a bitfields struct on LHS: 0
XXX times a bitfields struct on RHS: 1
XXX times a single bitfield on LHS: 0
XXX times a single bitfield on RHS: 0

XXX max expression depth: 1
breakdown:
   depth: 1, occurrence: 1

XXX total number of pointers: 0

XXX times a non-volatile is read: 1
XXX times a non-volatile is write: 0
XXX times a volatile is read: 0
XXX    times read thru a pointer: 0
XXX times a volatile is write: 0
XXX    times written thru a pointer: 0
XXX times a volatile is available for access: 0
XXX percentage of non-volatile access: 100

XXX forward jumps: 0
XXX backward jumps: 0

XXX stmts: 1
XXX max block depth: 0
breakdown:
   depth: 0, occurrence: 1

XXX percentage a fresh-made variable is used: 100
XXX percentage an existing variable is used: 0
FYI: the random generator makes assumptions about the integer size. See platform.info for more details.
XXX total OOB instances added: 0
********************* end of statistics **********************/

