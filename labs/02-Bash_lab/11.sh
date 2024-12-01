#!/bin/bash

usage() {
    echo "Usage: $(basename "$0") [-h] [-m <default_mode>] [-M <mode>] [-u <default_user>] [-U <user>] [-g <default_group>] [-G <group>] <config_file>" >&2
    exit 1
}

error() {
    echo "Error: $1" >&2
    usage
}

default_mode="0644"
default_user="$USER"
default_group=$(id -gn)

all_mode=""
all_user=""
all_group=""

while getopts ":hm:M:u:U:g:G:" opt; do
    case $opt in
        h)
            usage
            ;;
        m)
            default_mode="$OPTARG"
            ;;
        M)
            all_mode="$OPTARG"
            ;;
        u)
            default_user="$OPTARG"
            ;;
        U)
            all_user="$OPTARG"
            ;;
        g)
            default_group="$OPTARG"
            ;;
        G)
            all_group="$OPTARG"
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
    error "Configuration file is required."
fi

config_file="$1"

if [ ! -f "$config_file" ]; then
    echo "Error: Configuration file does not exist: $config_file" >&2
    exit 1
fi

while IFS=":" read -r path permissions user group; do
    path=$(echo "$path" | xargs)
    permissions=$(echo "$permissions" | xargs)
    user=$(echo "$user" | xargs)
    group=$(echo "$group" | xargs)

    if [ -z "$path" ]; then
        echo "Error: File path is missing in configuration file." >&2
        continue
    fi

    permissions=${permissions:-$default_mode}
    user=${user:-$default_user}
    group=${group:-$default_group}

    if [ -n "$all_mode" ]; then
        permissions="$all_mode"
    fi
    if [ -n "$all_user" ]; then
        user="$all_user"
    fi
    if [ -n "$all_group" ]; then
        group="$all_group"
    fi

    if [ ! -e "$path" ]; then
        mkdir -p "$(dirname "$path")"
        touch "$path"
    fi

    chmod "$permissions" "$path"
    chown "$user:$group" "$path"

    echo "Created file: $path with permissions: $permissions, user: $user, group: $group"
done < "$config_file"
