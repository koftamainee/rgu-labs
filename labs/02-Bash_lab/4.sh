#!/bin/bash

while getopts ":e:hd:f:" opt; do
    case $opt in
        f)
            filename="$OPTARG"
            ;;
        d)
            input_dir="$OPTARG"
            ;;
        e)
            error_file="$OPTARG"
            ;;
        h)
            echo "Usage: ./3.sh -d <directory> -f <filename>"
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

if [[ -d $input_dir ]]; then
    find $input_dir -type f -name $filename
    else
        error_message="dir $input_dir now found"
        if [[ -n $error_file ]]; then
            echo "$error_message" >> "$error_file"
        else
            "$error_message" >&2
        fi
    fi
