PROGNAME   = exec_plan
CXX        = g++
CXXFLAGS   = -Wall --pedantic -l pthread -l protobuf -O6
FILES      = proto/operations.pb.o server.cc noperation.o nscanoperation.o nfilteroperation.o ncomputeoperation.o nexpression.o ncolumnexpression.o nconstantexpression.o ntypedexpression.o nifexpression.o nbinaryoperatorexpression.o nunaryoperatorexpression.o ngroupbyoperation.o

$(PROGNAME): $(PROGNAME).cc $(FILES)
	$(CXX) $(CXXFLAGS) -o $(PROGNAME) $(PROGNAME).cc $(FILES)

proto/operations.pb.cc: operations.proto proto/operations.pb.h

proto/operations.pb.h: operations.proto
	mkdir -p proto
	protoc operations.proto --cpp_out=proto

%.o: %.cc %.h proto/operations.pb.h
	$(CXX) -c $(CXXFLAGS) $< -o $*.o

clean:
	rm -f *.o
	rm -f $(PROGNAME)

