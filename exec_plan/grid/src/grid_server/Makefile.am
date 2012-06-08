SUBDIRS = proto

bin_PROGRAMS = grid_server_main
grid_server_main_SOURCES = grid_server.h grid_server.cc grid_server_main.cc
grid_server_main_LDADD = ../utils/libutils.a ./proto/libgrid_server_pb.la ../netio/libnetio.a $(BOOST_SYSTEM_LIB) $(BOOST_THREAD_LIB)
