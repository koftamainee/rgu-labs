#!/bin/bash

usage() {
    echo "Usage: $(basename "$0") [-h] [-s <signal>] <process_name>" >&2
    exit 1
}

error() {
    echo "Error: $1" >&2
    usage
}

signal=15

while getopts ":hs:" opt; do
    case $opt in
        h)
            usage
            ;;
        s)
            if [[ "$OPTARG" =~ ^[0-9]+$ ]]; then
                signal="$OPTARG"
            else
                error "Signal must be a number."
            fi
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

if [ "$#" -ne 1 ]; then
    error "Process name is required."
fi

process_name="$1"

pids=$(pgrep "$process_name")

if [ -z "$pids" ]; then
    echo "No processes found with name: $process_name" >&2
    exit 1
fi

for pid in $pids; do
    if kill -"$signal" "$pid" 2>/dev/null; then
        echo "Sent signal $signal to process $pid ($process_name)"
    else
        echo "Failed to send signal to process $pid" >&2
    fi
done
