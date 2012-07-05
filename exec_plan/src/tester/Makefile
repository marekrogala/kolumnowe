all: worker grid_server

.PHONY: worker grid_server

worker:
	make -C .. worker

grid_server:
	make -C .. grid_server

clean_test:
	rm -f *.out stat/*.plot

clean:
	rm -f worker grid_server *.out stat/*.plot
