#!/bin/bash
pids=( )
cleanup() {
    for pid in "${pids[@]}"; do
        kill -0 "$pid" && kill "$pid"
    done
}
trap cleanup EXIT TERM

# echo "Waiting for 30 seconds for the services to start..."
# sleep 30
# echo "Starting the services..."

(cd /home/pi/HTA && python3 receiver.py) & pids+=( "$!" )
(cd /home/pi/HTA && python3 wserver.py) & pids+=( "$!" )

# python receiver.py & pids+=( "$!" )
# FLASK_APP=wserver.py FLASK_DEBUG=1 flask run --reload & pids+=( "$!" )

wait