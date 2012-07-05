








for i in [1..20]; do
  ./tester 3 1 10 &
  sleep 30
  killall grid_server
  killall worker
done
