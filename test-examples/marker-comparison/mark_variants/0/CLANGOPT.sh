#!/bin/bash

MAIN_SRC="${1:-file.c}"
MARKER_SRC="${2:-MarkerDefinition.c}"
MARKER_HDR="${3:-MarkerDefinition.h}"   # Not directly used, but for reference

# Derive base name (without .c) and output directory
MAIN_BASE=$(basename "$MAIN_SRC" .c)
MARKER_BASE=$(basename "$MARKER_SRC" .c)
OUTPUT_DIR="./${MAIN_BASE}_0_CLANGOPT"

# Create output directory if it doesn't exist
mkdir -p "$OUTPUT_DIR"

# Move into the output directory so saved-temps files appear here
cd "$OUTPUT_DIR" || exit 1

# Compile with LTO, tell the linker plugin to save temporary bitcode
clang "../$MAIN_SRC" \
	  "../${MARKER_SRC}" \
    -o "${MAIN_BASE}_0_clangopt.out" \
    -O3 -flto \
    -Wl,-plugin-opt=save-temps

for bc in *.lto.*.bc *.0.*.bc; do
    [ -f "$bc" ] && llvm-dis "$bc" -o "${bc%.bc}.ll"
done

echo "All outputs (including .ll files) are in: $OUTPUT_DIR"
