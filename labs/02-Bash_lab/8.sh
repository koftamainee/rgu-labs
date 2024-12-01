#!/bin/bash

usage() {
    echo "Usage: $(basename "$0") <directory>" >&2
    exit 1
}

if [ "$#" -ne 1 ]; then
    usage
fi

directory="$1"

if [ ! -d "$directory" ]; then
    echo "Error: Directory does not exist: $directory" >&2
    exit 1
fi

du -sh "$directory"*/ | sort -h
