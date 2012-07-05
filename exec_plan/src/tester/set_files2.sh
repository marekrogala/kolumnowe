#!/bin/bash

#set -ex

COUNT=$1





for i in {1..10}; do
  COUNT=`ls ./data/q${i}-*.data | wc -w `
  echo "q$i files: $COUNT"
  perl -pi -e "s/number_of_files:\s(\d*)/number_of_files: $COUNT/g" `ls ../queries/q$i.ascii`
done



#perl -pi -e "s/int\stotal_files_xyz\s=\s(\d+)/int total_files_xyz = $COUNT/g" `ls ./../node_environment/*.cc`
