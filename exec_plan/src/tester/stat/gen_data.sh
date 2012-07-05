


#set -ex

start_date=`date +"%s"`
received=`perl net_info.pl < /proc/net/dev`

 
PREV_TOTAL=0
PREV_IDLE=0
 

while true; do


  ########## porcessor
  CPU=(`cat /proc/stat | grep '^cpu '`) # Get the total CPU statistics.
  unset CPU[0]                          # Discard the "cpu" prefix.
  IDLE=${CPU[4]}                        # Get the idle CPU time.
 
  # Calculate the total CPU time.
  TOTAL=0
  for VALUE in "${CPU[@]}"; do
    let "TOTAL=$TOTAL+$VALUE"
  done
 
  # Calculate the CPU usage since we last checked.
  let "DIFF_IDLE=$IDLE-$PREV_IDLE"
  let "DIFF_TOTAL=$TOTAL-$PREV_TOTAL"
  let "DIFF_USAGE=(1000*($DIFF_TOTAL-$DIFF_IDLE)/$DIFF_TOTAL+5)/10"
 
  # Remember the total and idle CPU times for the next check.
  PREV_TOTAL="$TOTAL"
  PREV_IDLE="$IDLE"
 



  #################network
  old=$received
  received=`perl net_info.pl < /proc/net/dev`
  speed=`echo "scale=3; ($received - $old) / 1024 / 1024" | bc -l`
  curr_time=`date +"%s"`
  last=$[curr_time - start_date]
  echo "$last 0$speed $DIFF_USAGE"
  sleep 1
done
