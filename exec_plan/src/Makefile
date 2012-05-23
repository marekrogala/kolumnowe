CC=g++
OPTIONS= -ftree-vectorizer-verbose=2 -std=c++0x -O7
exec_plan: ComputeOperation.cc ExpressionNode.cpp FilterOperation.h MemoryManager.cpp MEngine.h  Operation.h ScanOperation.cc server.h ComputeOperation.h ExpressionNode.h GroupByOperation.cpp MemoryManager.h OperationBuilder.cpp operations.pb.cc ScanOperation.h UniversalHashmap.h engine.cc FilterOperation.cpp GroupByOperation.h MEngine.cc  OperationBuilder.h operations.pb.h server.cc RealUniversalHashmap.cpp RealUniversalHashmap.h
	$(CC) $(OPTIONS) ComputeOperation.cc ExpressionNode.cpp FilterOperation.h MemoryManager.cpp MEngine.h  Operation.h ScanOperation.cc server.h ComputeOperation.h ExpressionNode.h GroupByOperation.cpp MemoryManager.h OperationBuilder.cpp operations.pb.cc ScanOperation.h UniversalHashmap.h engine.cc  FilterOperation.cpp GroupByOperation.h MEngine.cc  OperationBuilder.h operations.pb.h server.cc RealUniversalHashmap.cpp RealUniversalHashmap.h -lprotobuf -lpthread -o exec_plan

net_tester:
	g++ net_tester.cc libnode_environment.a libnetio.a libutils.a -l pthread

clean:
	rm operations.pb.h operations.pb.cc exec_plan *.gch a.out
 
