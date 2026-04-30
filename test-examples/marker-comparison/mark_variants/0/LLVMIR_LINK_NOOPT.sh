#!/bin/bash

# LLVM-IR Linking, staat optimalizaties toe over markers heen.
# Usage: ./compile-with-markers.sh <main_source.c> [marker_source.c] [marker_header.h]
# All generated files go into ./<main_basename>/ directory

MAIN_SRC="${1:-file.c}"
MARKER_SRC="${2:-MarkerDefinition.c}"
MARKER_HDR="${3:-MarkerDefinition.h}"   # Not directly used, but for reference

# Derive base name (without .c) and output directory
MAIN_BASE=$(basename "$MAIN_SRC" .c)
MARKER_BASE=$(basename "$MARKER_SRC" .c)
OUTPUT_DIR="./${MAIN_BASE}_0_LLVMIR_LINK_NOOPT"

# Create output directory if it doesn't exist
mkdir -p "$OUTPUT_DIR"

# Compile to LLVM bitcode, placing .bc files in the output directory
clang "$MAIN_SRC" -c -emit-llvm -o "$OUTPUT_DIR/${MAIN_BASE}.bc"
clang "$MARKER_SRC" -c -emit-llvm -o "$OUTPUT_DIR/${MARKER_BASE}.bc"

# Link bitcode files (output merged.bc in the same directory)
llvm-link "$OUTPUT_DIR/${MAIN_BASE}.bc" "$OUTPUT_DIR/${MARKER_BASE}.bc" -o "$OUTPUT_DIR/$OUTPUT_DIR.bc"

# Disassemble to LLVM IR
llvm-dis "$OUTPUT_DIR/$OUTPUT_DIR.bc" -o "$OUTPUT_DIR/$OUTPUT_DIR.ll"

# Compile to executable without LTO
clang "$OUTPUT_DIR/$OUTPUT_DIR.bc" -o "$OUTPUT_DIR/${OUTPUT_DIR}.out"

echo "All outputs are in: $OUTPUT_DIR"
