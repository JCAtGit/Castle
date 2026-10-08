# Checksum tests
CSmith works by outputting a checksum when run, the principle is that this checksum should be the same under all optimization levels.
We want to check if the insertion of marker statement alters the semantics, i.e. the global state with the checksum.
We assume that no errors resulting from the compiler and any difference in the checksum to be resulting from the markers statements.

However for DSmith, errors can occur like "C++ exception with description "std::bad_alloc" thrown in the test body.", from which it is not immediately clear what might be the cause.
Further investigation of this specific topic regarding the marker instrumentation is left for future work.