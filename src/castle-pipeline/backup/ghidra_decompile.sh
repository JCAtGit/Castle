#!/bin/bash

# Fixed paths (always provided)
ANALYZE_HEADLESS="/home/user/Desktop/CASTLE/DeComScaffolder/Decompilers/ghidra_12.0.4_PUBLIC_20260303/ghidra_12.0.4_PUBLIC/support/analyzeHeadless"
DECOMPILE_SCRIPT="/home/user/Desktop/CASTLE/DeComScaffolder/scripts/ghidra_postscript.java"

# Usage
BINARY="$1"
OUTPUT_C="$2"
TEMP_BASE="${3:-/tmp/ghidra_temp}"

if [ -z "$BINARY" ] || [ -z "$OUTPUT_C" ]; then
    echo "Usage: $0 <binary_file> <output_c_file> [temp_base_dir]"
    exit 1
fi

if [ ! -f "$BINARY" ]; then
    echo "Error: Binary file '$BINARY' not found."
    exit 1
fi

if [ ! -f "$ANALYZE_HEADLESS" ]; then
    echo "Error: analyzeHeadless not found at: $ANALYZE_HEADLESS"
    exit 1
fi

if [ ! -f "$DECOMPILE_SCRIPT" ]; then
    echo "Error: Decompile script not found at: $DECOMPILE_SCRIPT"
    exit 1
fi

mkdir -p "$TEMP_BASE" || { echo "Error: Cannot create $TEMP_BASE"; exit 1; }

PROJECT_NAME="ghidra_$$_$(date +%s)"

# Extract script directory and filename
SCRIPT_DIR="$(dirname "$DECOMPILE_SCRIPT")"
SCRIPT_NAME="$(basename "$DECOMPILE_SCRIPT")"

echo "Decompiling $BINARY -> $OUTPUT_C ..."
echo "Using temporary project directory: $TEMP_BASE"
echo "Project name: $PROJECT_NAME"

# Run Ghidra headless
"$ANALYZE_HEADLESS" "$TEMP_BASE" "$PROJECT_NAME" \
    -import "$BINARY" \
    -scriptPath "$SCRIPT_DIR" \
    -postScript "$SCRIPT_NAME" "$OUTPUT_C" \
    -deleteProject

if [ $? -eq 0 ] && [ -f "$OUTPUT_C" ]; then
    echo "Success: Decompiled C written to $OUTPUT_C"
else
    echo "Error: Decompilation failed."
    exit 1
fi
