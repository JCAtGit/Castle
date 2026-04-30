
MAIN_SRC="${1:-file.c}"
MARKER_SRC="${2:-MarkerDefinition.c}"

# Derive base names and output directory
MAIN_BASE=$(basename "$MAIN_SRC" .c)
MARKER_BASE=$(basename "$MARKER_SRC" .c)
OUTPUT_DIR="./${MAIN_BASE}_CLANGOPT"

# Create output directory if it doesn't exist
mkdir -p "$OUTPUT_DIR"

clang "${MAIN_SRC}" -o "$OUTPUT_DIR/${MAIN_BASE}_c_clangopt.out" -O3 -flto
