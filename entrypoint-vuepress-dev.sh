#!/bin/sh
trap : TERM INT
yarn install
while :; do :; done &
kill -STOP $! && wait $!

# tail -f /dev/null & wait
