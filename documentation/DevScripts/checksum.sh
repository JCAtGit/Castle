#!/bin/bash
# Usage: ./compare_dirs.sh <dir1> <dir2>
# Compares all files in dir1 and dir2 by SHA-256 hash.

if [[ $# -ne 2 ]]; then
    echo "Usage: $0 <dir1> <dir2>"
    exit 1
fi

dir1="$1"
dir2="$2"

if [[ ! -d "$dir1" || ! -d "$dir2" ]]; then
    echo "Error: Both arguments must be existing directories."
    exit 1
fi

# Find all files in dir1 (relative paths, including subdirectories)
find "$dir1" -type f -printf "%P\n" | while IFS= read -r file; do
    if [[ ! -f "$dir2/$file" ]]; then
        echo "MISSING: $file (not in $dir2)"
        continue
    fi
    
    # Compute SHA-256 hashes (only the hash, not the filename)
    hash1=$(sha256sum "$dir1/$file" | awk '{print $1}')
    hash2=$(sha256sum "$dir2/$file" | awk '{print $1}')
    
    if [[ "$hash1" == "$hash2" ]]; then
        echo "OK: $file"
    else
        echo "DIFFER: $file"
    fi
done