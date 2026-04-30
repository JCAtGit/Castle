
#!/bin/bash

# Object linking, staat optimalizaties toe over markers heen.
# Usage: ./compile-object.sh <main_source.c> [marker_source.c] [marker_header.h]
# All generated files go into ./<main_basename>/ directory

MAIN_SRC="${1:-file.c}"
MARKER_SRC="${2:-MarkerDefinition.c}"
MARKER_HDR="${3:-MarkerDefinition.h}"   # Not directly used, but for reference

# Derive base names and output directory
MAIN_BASE=$(basename "$MAIN_SRC" .c)
MARKER_BASE=$(basename "$MARKER_SRC" .c)
OUTPUT_DIR="./${MAIN_BASE}_0_OLINK_POSTOPT"

# Create output directory if it doesn't exist
mkdir -p "$OUTPUT_DIR"

# Step 1: Compile each .c to object file (.o) inside OUTPUT_DIR
clang "$MAIN_SRC" -c -o "$OUTPUT_DIR/${MAIN_BASE}.o"
clang "$MARKER_SRC" -c -o "$OUTPUT_DIR/${MARKER_BASE}.o"

# Step 2: Link object files into executable (no LTO)
clang "$OUTPUT_DIR/${MAIN_BASE}.o" "$OUTPUT_DIR/${MARKER_BASE}.o" -o "$OUTPUT_DIR/${OUTPUT_DIR}.out" -O3 -flto

echo "All outputs are in: $OUTPUT_DIR"

