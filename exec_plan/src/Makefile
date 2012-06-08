PROGNAME   = exec_plan
CC        = g++
CXX        = g++
OPTIONS= -std=c++0x -O7
BOOST_CPPFLAGS = -pthread -I/usr/include
BOOST_SYSTEM_LIB = -lboost_system
BOOST_THREAD_LIB = -lboost_thread
CCFLAGS = -O7 -std=c++0x
CXXFLAGS   =  ../grid/src/node_environment/libnode_environment.a ../grid/src/utils/libutils.a ../grid/src/netio/libnetio.a ../grid/src/grid_server/proto/grid_server.pb.o $(BOOST_SYSTEM_LIB) $(BOOST_THREAD_LIB) $(BOOST_CPPFLAGS) -lprotobuf $(OPTIONS)

FILES = ComputeOperation.o ExpressionNode.o FilterOperation.o MemoryManager.o MEngine.o  Operation.h ScanOperation.o GroupByOperation.o OperationBuilder.o UniversalHashmap.h engine.o RealUniversalHashmap.o BlockSerializer.o operations.pb.o

all: $(PROGNAME)

$(PROGNAME): $(FILES)
	$(CXX) -o $(PROGNAME) $(FILES) $(CXXFLAGS)

net_tester: net_tester.cc GroupSender.cc GroupReceiver.cc operations.pb.cc 
	$(CC) net_tester.cc GroupSender.cc GroupReceiver.cc BlockSerializer.cc operations.pb.cc $(CXXFLAGS) -o net_tester

clean:
	rm *.o exec_plan *.gch a.out


#proto/operations.pb.cc: operations.proto proto/operations.pb.h

#proto/operations.pb.h: operations.proto
#	mkdir -p proto
#	protoc operations.proto --cpp_out=.

%.o: %.cc %.h
	$(CC) -c $(CCFLAGS) $< -o $*.o

