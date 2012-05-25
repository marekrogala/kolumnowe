PROGNAME   = exec_plan
CC        = g++
CXX        = g++
OPTIONS= -std=c++0x -O7
BOOST_CPPFLAGS = -pthread -I/usr/include
BOOST_SYSTEM_LIB = -lboost_system
BOOST_THREAD_LIB = -lboost_thread
CCFLAGS = -Wall --pedantic -O2 -std=c++0x
CXXFLAGS   = -Wall --pedantic ../grid/src/node_environment/libnode_environment.a ../grid/src/utils/libutils.a ../grid/src/netio/libnetio.a $(BOOST_SYSTEM_LIB) $(BOOST_THREAD_LIB) $(BOOST_CPPFLAGS) -lprotobuf $(OPTIONS)

FILES = ComputeOperation.o ExpressionNode.o FilterOperation.o MemoryManager.o MEngine.o  Operation.h ScanOperation.o ComputeOperation.o ExpressionNode.o GroupByOperation.o MemoryManager.o OperationBuilder.o operations.pb.cc ScanOperation.o UniversalHashmap.h engine.o FilterOperation.o RealUniversalHashmap.o operations.pb.o

$(PROGNAME): $(FILES)
	$(CXX) $(CXXFLAGS) -o $(PROGNAME) $(FILES)

net_tester: net_tester.cc GroupSender.cc GroupReceiver.cc operations.pb.cc 
	$(CC) net_tester.cc GroupSender.cc GroupReceiver.cc BlockSerializer.cc operations.pb.cc $(CXXFLAGS) -o net_tester

clean:
	rm *.o exec_plan *.gch a.out


#proto/operations.pb.cc: operations.proto proto/operations.pb.h

#proto/operations.pb.h: operations.proto
#	mkdir -p proto
#	protoc operations.proto --cpp_out=.

%.o: %.cc %.h operations.pb.h
	$(CC) -c $(CCFLAGS) $< -o $*.o

