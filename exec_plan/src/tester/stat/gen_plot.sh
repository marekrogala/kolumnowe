


if [ $# -ne 1 ]
then
  echo "Usage $0 [data_file]"
  exit
fi

set -ex

echo "
set ylabel \"[MB/s]\"
set y2label \"[% uzycia procesora]\"
set y2tics 
set ytics nomirror 
plot \"$1\" using 1:2 axis x1y1 title \"siec w MB/s\" w l, \
  \"$1\" using 1:3 axis x1y2 title \"procesor w %\" with lines" | gnuplot -p&
