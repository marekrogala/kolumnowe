#include "proto/operations.pb.h"
#include "noperation.h"

#include <google/protobuf/text_format.h>
#include <string>
#include <fstream>
#include <streambuf>
#include <iostream>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <fcntl.h>

using namespace std;

int main(int argc, char * argv[]){
  Operation op;

  int query_id = atoi(argv[1]);

  cout << "Executing query_id " << query_id << ", query from " << argv[2] << endl;

  int fd = open(argv[2], O_RDONLY);
  google::protobuf::io::FileInputStream  input(fd);
  google::protobuf::TextFormat::Parse(&input, &op);
  close(fd);

  cout << "Query description read." << endl;

  NOperation * noperation = NOperation::FromProtobuf(op);

  cout << "Query interpreted as: " << endl;
  cout << noperation << endl;

  cout << endl << "Executing query..." << endl;

  class Server *server = CreateServer(query_id);
  noperation->setServer(server);
  noperation->execute();
  delete server;
  delete noperation;

  cout << "Query executed, server deleted." << endl;

  return 0;
}

