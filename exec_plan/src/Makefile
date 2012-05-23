CC=g++
OPTIONS= -ftree-vectorizer-verbose=2 -std=c++0x -O7
exec_plan: ComputeOperation.cc ExpressionNode.cpp FilterOperation.h MemoryManager.cpp MEngine.h  Operation.h ScanOperation.cc server.h ComputeOperation.h ExpressionNode.h GroupByOperation.cpp MemoryManager.h OperationBuilder.cpp operations.pb.cc ScanOperation.h UniversalHashmap.h engine.cc FilterOperation.cpp GroupByOperation.h MEngine.cc  OperationBuilder.h operations.pb.h server.cc RealUniversalHashmap.cpp RealUniversalHashmap.h
	$(CC) $(OPTIONS) ComputeOperation.cc ExpressionNode.cpp FilterOperation.h MemoryManager.cpp MEngine.h  Operation.h ScanOperation.cc server.h ComputeOperation.h ExpressionNode.h GroupByOperation.cpp MemoryManager.h OperationBuilder.cpp operations.pb.cc ScanOperation.h UniversalHashmap.h engine.cc  FilterOperation.cpp GroupByOperation.h MEngine.cc  OperationBuilder.h operations.pb.h server.cc RealUniversalHashmap.cpp RealUniversalHashmap.h -lprotobuf -lpthread -o exec_plan


BOOST_CPPFLAGS = -pthread -I/usr/include
BOOST_SYSTEM_LIB = -lboost_system
BOOST_THREAD_LIB = -lboost_thread

net_tester: net_tester.cc GroupSender.cc GroupReceiver.cc operations.pb.cc 
	$(CC) net_tester.cc GroupSender.cc GroupReceiver.cc operations.pb.cc libnode_environment.a libutils.a libnetio.a $(BOOST_SYSTEM_LIB) $(BOOST_THREAD_LIB) $(BOOST_CPPFLAGS) -lprotobuf -std=c++0x -O7 -o net_tester

clean:
	rm operations.pb.h operations.pb.cc exec_plan *.gch a.out
 
