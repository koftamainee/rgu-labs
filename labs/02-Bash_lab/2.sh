#!/bin/bash

total_lines=0
error_file=""

while getopts ":e:" opt; do
    case $opt in
        e) 
            error_file="$OPTARG"
            ;;
         \?)
            echo "Wrong flag: -$OPTARG" >&2
            exit 1
            ;;
        :)
            echo "Flag -$OPTARG needed in argument." >&2
            exit 1
            ;;
    esac
done

shift $((OPTIND -1))

for file in "$@"; do
    if [[ -f $file ]]; then
        lines=$(wc -l < "$file")
        total_lines=$((total_lines + lines))
    else
        error_message="File $file now found"

        if [[ -n $error_file ]]; then
            echo "$error_message" >> "$error_file"
        else
            "$error_message" >&2
        fi
    fi
done

echo $total_lines