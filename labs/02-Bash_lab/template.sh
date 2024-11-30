#!/bin/bash

usage() {
    echo "Usage: $(basename "$0") [-h] -d <input_directory> -f <output_filename>" >&2
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


while getopts ":hd:f:e:" opt; do
    case $opt in
        d)
            input_dir="$OPTARG"
            ;;
        f)
            output_file="$OPTARG"
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

if [ -z "$input_dir" ]; then
    error "-d <input_directory> is required"
elif [ -z "$output_file" ]; then
    error "-f <output_filename> is required"
fi

echo "Input Directory: $input_dir"
echo "Output File: $output_file"
echo "Error File: $error_file"
