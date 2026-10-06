int g;

int without_barrier(void) {
    g = 0;
    if (g)          /* compiler "knows" g == 0 here */
        return 1;
    return 0;
}

int with_barrier(void) {
    g = 0;
    __asm__ __volatile__("" ::: "memory");   /* memory might change */
    if (g)          /* must reload g, cannot fold */
        return 1;
    return 0;
}