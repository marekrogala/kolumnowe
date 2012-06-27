#!/bin/bash

trap 'kill $(jobs -p)' EXIT
trap 'kill $(jobs -p)' SIGINT

START_PORT=2000
COUNT=$1


echo "RUNNING QUERY " $2 on $1 nodes.

MAX_NUMBER=`expr $COUNT - 1`

./exec_plan localhost:2000 &> node_0.log &
sleep 1
PORT_NODE_0=`cat node_0.log | sed 's/.*Running node on port: \([0-9]*\).*/\1/'`

for i in `seq 1 $MAX_NUMBER`; do
   PORT=`expr $START_PORT + $i`
  if [ "$i" == "$MAX_NUMBER" ]; then sleep 1; fi;
  ./exec_plan localhost:2000 &> node_$i.log &
done

echo PORT $PORT_NODE_0

./sender localhost:$PORT_NODE_0 < $2 &

echo $PIDS
watch cat node_0.log
