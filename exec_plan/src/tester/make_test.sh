#!/bin/bash


set -ex

for ((i = 1; i <= 25; ++i)); do
  ./exec_plan $i ../queries/q$i.ascii > ../q$i.expected
done


