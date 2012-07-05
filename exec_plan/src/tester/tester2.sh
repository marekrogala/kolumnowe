#/!bin/bash

trap 'kill $(jobs -p)' EXIT
trap 'kill $(jobs -p)' SIGINT

#set -ex
DIR="~/kol/new/codwh-2012/distributed_version/run/"

if [ $# -ne 3 ] 
then
  echo "Usage $0 [max_number_of_peers] [query_id] [data_size]"
  exit
fi



machines=()
COUNT=10
LAB=blue
SERVER_NR=14
QUERY_ID=$2
DATA_SIZE=$3

SERVER_PORT=2375

SERVER_NAME="${LAB}$SERVER_NR:$SERVER_PORT"

SERVER_NAME="students:$SERVER_PORT"


MAX_PEERS=20



j=0
for i in `seq 0 $MAX_PEERS`; do
  ETYKIETA=$i
  if [ $ETYKIETA -lt 10 ] 
  then
    ETYKIETA="0$ETYKIETA"
  fi

  CHCK=`ssh $LAB$ETYKIETA "echo 2>&1" 2>/dev/null && echo "success" || echo "fail"` 
  if [ "success" = $CHCK ]
  then
    machines[$j]=$LAB$ETYKIETA
    j=`expr $j + 1`
  fi
done

echo "oto maszyny: ${machines[@]}"
sleep 2


if [ $j -lt $1 ]
then
  COUNT=$j
else
  COUNT=$1
fi

echo "number of nodes: $COUNT"

./grid_server $SERVER_PORT $COUNT < ../queries/q$QUERY_ID.ascii&
sleep 3



MAX_NUMBER=`expr $COUNT - 1`

for i in `seq 0 $MAX_NUMBER`; do
  echo "starting worker $i on ${machines[$i]}"
  ssh ${machines[$i]} "$DIR/worker_task.sh $DIR $i $SERVER_NAME $QUERY_ID $DATA_SIZE" &
done



wait


