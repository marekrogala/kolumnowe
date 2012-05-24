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
      int max_rows = 2000000;

      OperationTree::Operation operation;
      const char * query_file = "../tests/r1.ascii";
      int queryId = 1;

      std::fstream input(query_file, ios::in);
      if (!input) {
          cerr << argv[2] << ": File not found." << endl;
          return -1;
      } else {
          char data[BUFF_SIZE];
          input.read(data, BUFF_SIZE);
          if (!google::protobuf::TextFormat::ParseFromString(string(data), &operation)) {
              cerr << "Failed to parse operation." << endl;
              return -1;
          }
      }

      NodeEnvironmentInterface * nei = CreateNodeEnvironment(argc, argv, queryId);
      Engine::MEngine engine(nei, operation, max_rows);
      engine.run();
      
		//	delete server;
      delete nei;
      google::protobuf::ShutdownProtobufLibrary();

      return 0;
}
