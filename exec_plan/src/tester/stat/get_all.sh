


if [ $# -ne 1 ]
then
  TOTAL=50
else
  TOTAL=$1
fi



for i in `seq 0 $TOTAL`; do
  if [ -e $i.plot ]
  then
    ./gen_plot.sh $i.plot
  fi
done
