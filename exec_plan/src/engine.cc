#include "operations.pb.h"
#include <google/protobuf/text_format.h>
//#include "server.h"
#include <cstdio>
#include <iostream>
#include <fstream>
#include <cstring>
#include "MEngine.h"
#include "node_environment.h"

const int BUFF_SIZE = 10000;

using namespace std;

int main(int argc, char ** argv) {
      GOOGLE_PROTOBUF_VERIFY_VERSION; 



      int query_no = 1;
      //int max_rows = atoi(argv[3]); // TODO remove this shit before submitting
      int max_rows = 2;


      NodeEnvironmentInterface * nei = CreateNodeEnvironment(argc, argv);
      //pobranie zapytania
		size_t operationAsciiLen;
		char* operationAscii;
	  operationAscii = nei->ReadPacketBlocking(&operationAsciiLen);
      //wyslanie do nastepnego if != ostatni
	  if(nei->my_node_number() != nei->nodes_count()-1) {
		  nei->SendPacket(nei->my_node_number()+1, operationAscii, (int)operationAsciiLen);
	  }
	  // tworzenie operation

      OperationTree::Operation operation;
      //const char * query_file = "../tests/r1.ascii";
      //int queryId = 1;


      if (!google::protobuf::TextFormat::ParseFromString(string(operationAscii), &operation)) {
          cerr << "Failed to parse operation." << endl;
          return -1;
      }


	    Engine::MEngine engine(nei, operation, max_rows);
      engine.run();
      
		//	delete server;
      delete nei;
      google::protobuf::ShutdownProtobufLibrary();

      return 0;
}
