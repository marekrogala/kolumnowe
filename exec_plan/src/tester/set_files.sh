#!/bin/bash

set -ex

COUNT=$1

perl -pi -e "s/number_of_files:\s(\d*)/number_of_files: $COUNT/g" `ls ../queries/*.ascii`


perl -pi -e "s/int\stotal_files_xyz\s=\s(\d+)/int total_files_xyz = $COUNT/g" `ls ./../node_environment/*.cc`
