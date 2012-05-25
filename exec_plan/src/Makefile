PROGNAME   = exec_plan
CC        = g++
CXX        = g++
OPTIONS= -std=c++0x -O7
BOOST_CPPFLAGS = -pthread -I/usr/include
BOOST_SYSTEM_LIB = -lboost_system
BOOST_THREAD_LIB = -lboost_thread
CCFLAGS = -Wall --pedantic -O2 -std=c++0x
CXXFLAGS   = -Wall --pedantic libnode_environment.a libutils.a libnetio.a $(BOOST_SYSTEM_LIB) $(BOOST_THREAD_LIB) $(BOOST_CPPFLAGS) -lprotobuf $(OPTIONS)

FILES = ComputeOperation.o ExpressionNode.o FilterOperation.o MemoryManager.o MEngine.o  Operation.o ScanOperation.o ComputeOperation.o ExpressionNode.o GroupByOperation.o MemoryManager.o OperationBuilder.o operations.pb.cc ScanOperation.o UniversalHashmap.o engine.o FilterOperation.o RealUniversalHashmap.o

net_tester: net_tester.cc GroupSender.cc GroupReceiver.cc operations.pb.cc 
	$(CC) net_tester.cc GroupSender.cc GroupReceiver.cc BlockSerializer.cpp operations.pb.cc  -o net_tester

clean:
	rm *.o exec_plan *.gch a.out


FILES      = operations.pb.o *.o *.h

$(PROGNAME): $(PROGNAME).cc $(FILES)
	$(CXX) $(CXXFLAGS) -o $(PROGNAME) $(FILES)


#proto/operations.pb.cc: operations.proto proto/operations.pb.h

#proto/operations.pb.h: operations.proto
#	mkdir -p proto
#	protoc operations.proto --cpp_out=.

%.o: %.cc %.h operations.pb.h
	$(CC) -c $(CCFLAGS) $< -o $*.o

