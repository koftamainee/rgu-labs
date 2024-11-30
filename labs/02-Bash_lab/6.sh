#!/bin/bash

usage() {
    echo "Usage: $(basename "$0") [-h] [-r] file1 [file2 ...]" >&2
    exit 1
}

error() {
    echo "Error: $1" >&2
    usage
}

remove_links=false

while getopts ":hr" opt; do
    case $opt in
        h)
            usage
            ;;
        r)
            remove_links=true
            ;;
        \?)
            error "Unknown option: -$OPTARG"
            ;;
        :)
            error "Option -$OPTARG requires an argument."
            ;;
    esac
done

shift $((OPTIND - 1))

if [ "$#" -eq 0 ]; then
    error "No files specified."
fi

for file in "$@"; do
    if [ ! -f "$file" ]; then
        echo "File does not exist: $file" >&2
        continue
    fi

    if $remove_links; then
        for i in {1..9}; do
            link="${file}.${i}"
            if [ -e "$link" ]; then
                rm "$link" && echo "Removed link: $link" || echo "Failed to remove: $link" >&2
            fi
        done
    else
        for i in {1..9}; do
            link="${file}.${i}"
            if [ ! -e "$link" ]; then
                ln "$file" "$link" && echo "Created link: $link"
                break
            fi
        done
    fi
done
