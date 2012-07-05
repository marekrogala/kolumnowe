mkdir tmp



DATA_SIZE=100

for i in 1; do
   echo "TEST NR $i"
#./exec_plan $i ./../queries_single/q$i.ascii $DATA_SIZE 2> ./tmp/single_t$i.out
# sort ./tmp/single_t$i.out > ./tmp/single$i.out
# rm ./tmp/single_t$i.out

  FILES=`ls data/q${i}-*.data | wc -w`
  WORKERS=4
  ./tester $WORKERS $i $DATA_SIZE 2> ./tmp/multi_t$i.out
  sort ./tmp/multi_t$i.out > ./tmp/multi$i.out
  rm ./tmp/multi_t$i.out
  diff -u ./tmp/multi$i.out ./tmp/single$i.out
done
