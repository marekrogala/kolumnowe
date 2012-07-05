


for i in {1..10}; do
  cat q$i-*.data > q${i}a.data
  sort q${i}a.data > out/q${i}.data
  rm q${i}a.data
done
