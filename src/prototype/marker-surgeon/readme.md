# TODO:
- Beslissen of markers variadic of welk ander format moeten hebben.
- Bepalen maximale context en verwerken van van context kleiner dan maximale formaat

# Alt bronnen voor optimization barriers:
Een bron voor een operating system cursus, waarbij deze de zelfde mechanismen gebruikt.
https://cs162.org/static/proj/pintos-docs/docs/synch/optimization_barriers/

Een voorbeeld met spinlock reordering.
https://medium.com/@harshadaggarwal1999/memory-barriers-in-c-171bf4f03ff4

Hierbij wordt het zelfs als DCE-removal blocker gebruikt in 2016-05-27.

https://github.com/rhagenson/rfcs/blob/filepath/text/0002-optimisation-inhibition.md
# Alt-format voor codemarker mechanisme
Hierbij blijven de codemarkers in de code mbv mechanismen specifiek gebonden aan sideeffects.
De eerdere mechanisme van debm en dce werken op de onzekerheid vanuit de compiler richting potentiele sideeffects door de inhoud te verbergen.
Wij stellen juist dat wij dat expliciet doen en introduceren dus ook deze sideeffects.

static volatile int codemarker_sink = 0;

void codemarker(int id, const char *type, const char *info) {
    // Prevent removal of the function and act as a compiler barrier.
    __asm__ volatile ("" : : : "memory");
    // Create an observable side effect.
    codemarker_sink = id;
}

# "Enclosing" marker format
codemarker(String GUUID, String START=0);
codemarker(String GUUID, String END=1);

