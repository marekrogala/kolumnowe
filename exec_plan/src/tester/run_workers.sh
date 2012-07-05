#!/bin/bash

if [ $# != 1 ]; then
	echo "Usage: $0 [number_of_workers]"
	exit 1
fi

PORT=1234
N=$1
ROWS=4

#./grid_server $PORT $N < ../queries/q1.ascii&
#./worker localhost:$PORT 1 $ROWS&
for ((i=0 ; i<N ; i++)) ; do ./worker localhost:$PORT 1 $ROWS&  done
#./worker localhost:$PORT 1 $ROWS&
