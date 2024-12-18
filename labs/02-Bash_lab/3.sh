#!/bin/bash

while getopts ":e:h:d:" opt; do
    case $opt in
        d)
            input_file="$OPTARG"
            ;;
        e)
            error_file="$OPTARG"
            ;;
        h)
            echo "Usage: ./3.sh <filename>"
            exit 1
            ;;
        \?)
            echo "Unknown option: -$OPTARG" >&2
            exit 1
            ;;
        :)
            echo "Option -$OPTARG requires an argument." >&2
            exit 1
            ;;
    esac
done

if [[ -d $input_file ]]; then
    find $input_file | wc -l
    else
        error_message="File $file now found"
        if [[ -n $error_file ]]; then
            echo "$error_message" >> "$error_file"
        else
            "$error_message" >&2
        fi
    fi
