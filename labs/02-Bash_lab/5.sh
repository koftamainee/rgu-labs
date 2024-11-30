#!/bin/bash

usage() {
    echo "Usage: $(basename "$0") [-h] -c <worlds_count> -- <dir1> <dir2> ..." >&2
    exit 1
}

error() {
    if [ -f error_file ]; then
        echo "Error: $1" >error_file
    else
        echo "Error: $1" >&2
    fi
    usage
}

count_words() {
    local file="$1"
    local count=$(tr -cs '[:alnum:]' '\n' < "$file" | wc -l)
    echo "$file $count"
}

while getopts ":hd:c:e:" opt; do
    case $opt in
        c)
            min_words="$OPTARG"
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

shift $(( OPTIND - 1 ))

if [ -z "$min_words" ]; then
    error "-c <words_count> is required"
fi


for dir in "$@"; do
    if [[ ! -d "$dir" ]]; then
        error "Dir '$dir' is not found."
        continue
    fi

    find "$dir" -type f -print0 | while IFS= read -r -d '' file; do
        result=$(count_words "$file")
        filename="${result%% *}"
        word_count="${result##* }"

        if (( word_count >= min_words )); then
            echo "$filename ($word_count words)"
        fi
    done
done
