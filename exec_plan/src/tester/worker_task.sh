





if [ $# -ne 5 ]
then
  echo "Usage $0 [DIR] [WORKER_ID] [SERVER_NAME] [QUERY_ID] [DATA_SIZE]"
  exit
fi




DIR=$1
ID=$2
SERVER_NAME=$3
QUERY_ID=$4
DATA_SIZE=$5


cd $DIR


cd ./stat
./gen_data.sh > ./$ID.plot &
GEN_DATA_PID=$!
cd ..

./worker $SERVER_NAME $QUERY_ID $DATA_SIZE > $ID.out 
killall $GEN_DATA_PID



