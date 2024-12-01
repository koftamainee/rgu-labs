#!/bin/bash

usage() {
    echo "Usage: $(basename "$0") [-h] [-a] [-o <config_file>] [-p <default_path>] [-m <default_mode>] [-u <default_user>] [-g <default_group>] <directory>" >&2
    exit 1
}

error() {
    echo "Error: $1" >&2
    usage
}

config_file=".install-config"
default_path="$HOME"
default_mode="0644"
default_user="$USER"
default_group=$(id -gn)
auto_mode=false

while getopts ":hao:p:m:u:g:" opt; do
    case $opt in
        h)
            usage
            ;;
        a)
            auto_mode=true
            ;;
        o)
            config_file="$OPTARG"
            ;;
        p)
            default_path="$OPTARG"
            ;;
        m)
            default_mode="$OPTARG"
            ;;
        u)
            default_user="$OPTARG"
            ;;
        g)
            default_group="$OPTARG"
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
    error "Directory is required."
fi

directory="$1"

if [ ! -d "$directory" ]; then
    echo "Error: Directory does not exist: $directory" >&2
    exit 1
fi

if [ ! -f "$config_file" ]; then
    touch "$config_file"
fi

for file in "$directory"/*; do
    if [ ! -f "$file" ]; then
        continue
    fi

    if [ "$auto_mode" = true ]; then
        dest_path="$default_path"
        mode="$default_mode"
        user="$default_user"
        group="$default_group"
    else
        echo "Processing file: $file"
        read -p "Enter destination path [default: $default_path]: " dest_path
        dest_path="${dest_path:-$default_path}"

        read -p "Enter permissions (octal) [default: $default_mode]: " mode
        mode="${mode:-$default_mode}"

        read -p "Enter user [default: $default_user]: " user
        user="${user:-$default_user}"

        read -p "Enter group [default: $default_group]: " group
        group="${group:-$default_group}"
    fi

    echo "$(realpath "$file") : $mode : $user : $group" >> "$config_file"
    echo "Added to config: $(realpath "$file") : $mode : $user : $group"
done
