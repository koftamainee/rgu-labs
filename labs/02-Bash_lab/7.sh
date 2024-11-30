#!/bin/bash

usage() {
    echo "Usage: $(basename "$0") [-h] -d <input_directory> -f <search_string> [-o <output_directory>] [-s]" >&2
    exit 1
}

error() {
    if [ -f "$error_file" ]; then
        echo "Error: $1" >"$error_file"
    else
        echo "Error: $1" >&2
    fi
    usage
}

input_dir=""
search_string=""
output_dir="."
create_symlinks=false
error_file=""

while getopts ":hd:f:o:se:" opt; do
    case $opt in
        d)
            input_dir="$OPTARG"
            ;;
        f)
            search_string="$OPTARG"
            ;;
        o)
            output_dir="$OPTARG"
            ;;
        s)
            create_symlinks=true
            ;;
        e)
            error_file="$OPTARG"
            ;;
        h)
            usage
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

if [ -z "$input_dir" ]; then
    error "-d <input_directory> is required"
elif [ -z "$search_string" ]; then
    error "-f <search_string> is required"
fi

if [ ! -d "$input_dir" ]; then
    error "Input directory does not exist: $input_dir"
fi

if [ ! -d "$output_dir" ]; then
    mkdir -p "$output_dir" || error "Failed to create output directory: $output_dir"
fi

find "$input_dir" -type f | while read -r file; do
    if grep -q "$search_string" "$file"; then
        filename=$(basename "$file")
        target="$output_dir/$filename"

        if [ "$create_symlinks" = true ]; then
            ln -s "$file" "$target" || error "Failed to create symbolic link for $file"
        else
            ln "$file" "$target" || error "Failed to create hard link for $file"
        fi
    fi
done

echo "Links created successfully."
