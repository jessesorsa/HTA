#!/bin/bash
pids=( )
cleanup() {
    for pid in "${pids[@]}"; do
        kill -0 "$pid" && kill "$pid"
    done
}
trap cleanup EXIT TERM

(cd /home/pi/HTA && python3 receiver.py) & pids+=( "$!" )
(cd /home/pi/HTA && python3 wserver.py) & pids+=( "$!" )

wait
