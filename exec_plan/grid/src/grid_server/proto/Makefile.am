PROTO_FILES=grid_server.proto

CC_RES_PROTO_FILES=$(PROTO_FILES:%.proto=%.pb.cc)
H_RES_PROTO_FILES=$(PROTO_FILES:%.proto=%.pb.h)

BUILT_SOURCES=$(CC_RES_PROTO_FILES) $(H_RES_PROTO_FILES)
CLEANFILES = $(BUILT_SOURCES)

EXTRA_DIST = $(PROTO_FILES)

noinst_LTLIBRARIES = libgrid_server_pb.la
libgrid_server_pb_la_SOURCES = grid_server.pb.cc grid_server.pb.h

INCLUDES = -I $(PROTOBUF_INCLUDES) -I {prefix}/include -I .

all-local: all-cpp

all-cpp: $(CC_RES_PROTO_FILES)
	echo "all cpp-proto done"

grid_server.pb.cc: grid_server.proto
	$(PROTOC) grid_server.proto --cpp_out .

clean: clean-am
	rm -f ${BUILT_SOURCES}