#!/bin/bash

trap 'kill $(jobs -p)' EXIT
trap 'kill $(jobs -p)' SIGINT

START_PORT=2000
COUNT=$1

MAX_NUMBER=`expr $COUNT - 1`

for i in `seq 0 $MAX_NUMBER`; do
   PORT=`expr $START_PORT + $i`
  if [ "$i" == "$MAX_NUMBER" ]; then sleep 3; fi;
  ./exec_plan localhost:2000 &
done

echo $PIDS
wait
