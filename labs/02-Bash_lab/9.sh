#!/bin/bash

usage() {
    echo "Usage: $(basename "$0") <file_with_file_list>" >&2
    exit 1
}

if [ "$#" -ne 1 ]; then
    usage
fi

file_with_list="$1"

if [ ! -f "$file_with_list" ]; then
    echo "Error: File does not exist: $file_with_list" >&2
    exit 1
fi

while IFS= read -r file; do
    if [ -z "$file" ]; then
        continue
    fi

    if [ -f "$file" ]; then
        rm "$file" && echo "Deleted: $file" || echo "Failed to delete: $file" >&2
    else
        echo "File does not exist or is not a regular file: $file" >&2
    fi
done < "$file_with_list"
