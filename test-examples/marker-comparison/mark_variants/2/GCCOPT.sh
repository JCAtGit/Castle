
MAIN_SRC="${1:-file.c}"

# Derive base names and output directory
MAIN_BASE=$(basename "$MAIN_SRC" .c)
OUTPUT_DIR="./${MAIN_BASE}_2_GCCOPT"

# Create output directory if it doesn't exist
mkdir -p "$OUTPUT_DIR"

gcc "${MAIN_SRC}" -o "$OUTPUT_DIR/${OUTPUT_DIR}.out" -O3 -flto
