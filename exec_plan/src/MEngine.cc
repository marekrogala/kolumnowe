#include "MEngine.h"
#include "operations.pb.h"
#include "ScanOperation.h"
#include "OperationBuilder.h"
#include "BlockSerializer.h"
#include "layers.h"

Engine::MEngine::MEngine(NodeEnvironmentInterface * nei,
		const OperationTree::Operation &operation, int max_rows) :
	nei_(nei), max_rows_(max_rows), operation_(operation) {
	}

void Engine::MEngine::run() {
//	cerr << "Started running query." << endl;

	Layers::init(1, nei_);

	bool group_flag = 1;

	root_operation_ = OperationBuilder::build(nei_, operation_,
			new MemoryManager(max_rows_));

	InitRes r0 = root_operation_ -> init(group_flag);

	group_flag = 0;

	root_operation_ = OperationBuilder::build(nei_, operation_,
			new MemoryManager(max_rows_));


	InitRes r1 = root_operation_ -> init(group_flag);

	InitRes r;
	root_operation_ = OperationBuilder::build(nei_, operation_,
			new MemoryManager(max_rows_));

	if (r0.second == NULL || r1.second == NULL) {
		group_flag = 1;
		Layers::init(1, nei_);

	  r = root_operation_ -> init(group_flag);
	} else {
		Layers::init(2, nei_);
		group_flag = Layers::get_my_layer();
	  r = root_operation_ -> init(group_flag);
	}

	Types types = r.first;

  //cerr << "Result type" << endl;
  //for (int i = 0; i < types.size(); ++i) {
  //    cerr << types[i] << " ";
  //}
  //cerr << endl;

	int all_rows = 0;
	BlockSerializer blockSerializer;

	if (0 == nei_ ->my_node_number()) {
		DataSinkInterface * sink = nei_ -> OpenDataSink();

		int ile = nei_ -> nodes_count() - 1;
		size_t len;
		while (ile) {

			//cerr << "Worker " << nei_ -> my_node_number() << " waiting for packet" << endl;
			char * buffer = nei_ -> ReadPacketBlocking(&len);
			if (len == 0) {
			//	cerr << "Somebody ended." << endl;
				ile--;
				continue;
			}
			//cerr << "Worker " << nei_ -> my_node_number() << " got packet size = " << len << endl;
			vector<void*> data;

			int rows = blockSerializer.deserializeBlock(types, len, buffer, data);
			delete[] buffer;
			all_rows += rows;
			//cerr << "Worker " << nei_ -> my_node_number() << " rows " << rows << endl;
			for (int i = 0; i < types.size(); ++i) {
				switch (types[i]) {
				case SINT:
					sink -> ConsumeInts(i, rows, static_cast<int32*> (data[i]));
							delete[] static_cast<int32*>(data[i]);
					break;
				case SDOUBLE:
					sink -> ConsumeDoubles(i, rows,
							static_cast<double*> (data[i]));
							delete[] static_cast<double*>(data[i]);
					break;
				case SBOOL:
					sink -> ConsumeByteBools(i, rows,
							static_cast<bool*> (data[i]));
							delete[] static_cast<bool*>(data[i]);
					break;
				}
			}

		}
		delete sink;
	} else {
		// TYLKO SCAN
		
		while (true) {
			int rows = max_rows_;
			vector<void*> data = r.second -> pull(rows);
			if (rows == 0) {
				nei_ -> SendPacket(0, NULL, 0);
				break;
			}
			char * buffer;
			//cerr << "SENDING TO 0: \n";
			//printCols(types, data, rows);
			//cerr << "END\n";
			int bufferSize = blockSerializer.serializeBlock(types, data, rows,
					&buffer);
				nei_ -> SendPacket(0, buffer, bufferSize);

				delete[] buffer;

			all_rows += rows;
		//	if (debug)
		//		cerr << "Worker " << nei_ -> my_node_number() << " GOT "
		//				<< rows << " ROWS" << endl;


		}
	}

		//cerr << "=============================" << endl;
		//cerr << "Worker " << nei_ -> my_node_number() << " Consumed " << all_rows << " rows " << endl;
		//cerr << "Completed query." << endl;

}

