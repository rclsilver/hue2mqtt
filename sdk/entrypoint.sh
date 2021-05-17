#!/usr/bin/env bash

set -e

# Install dependents packages
./scripts/feeds update -a
./scripts/feeds install libmosquitto

# Apply patches
for _PATCH_FILE in $(find /work/patches -type f); do
    _TARGET_FILE=$(echo ${_PATCH_FILE} | sed -r 's/\/work\/patches\/(.+)/\1/g' | sed -r 's/\.patch$//g')
    patch -N ${_TARGET_FILE} ${_PATCH_FILE}
done

if [[ -z "${*}" ]]; then
    # Launch a new shell
    echo "Use /work/build.sh to build the package"
    bash -l
else
    bash -c "${*}"
fi
