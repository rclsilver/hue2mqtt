#!/usr/bin/env bash

set -e

make package/hue2mqtt/compile V=99

cp bin/ar71xx/packages/base/hue2mqtt_*_ar71xx.ipk /work
