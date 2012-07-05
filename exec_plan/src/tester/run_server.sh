#!/bin/bash

if [ $# != 2 ]; then
	echo "Usage: $0 [number_of_workers] [query_number]"
	exit 1
fi

PORT=1234
N=$1
PROTO=$2

./set_files.sh $N

./grid_server $PORT $N < ../queries/q$PROTO.ascii
