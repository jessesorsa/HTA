#!/bin/bash
pids=( )
cleanup() {
    for pid in "${pids[@]}"; do
        kill -0 "$pid" && kill "$pid"
    done
}

trap cleanup EXIT TERM

python receiver.py & pids+=( "$!" )
FLASK_APP=wserver.py FLASK_DEBUG=1 flask run --reload & pids+=( "$!" )

wait