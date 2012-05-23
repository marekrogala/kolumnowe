#include "MEngine.h"
#include "operations.pb.h"
#include "ScanOperation.h"
#include "OperationBuilder.h"
#include "BlockSerializer.h"

Engine::MEngine::MEngine(NodeEnvironmentInterface * nei,
		const OperationTree::Operation &operation, int max_rows) :
	nei_(nei), max_rows_(max_rows) {
	root_operation_ = OperationBuilder::build(nei, operation,
			new MemoryManager(max_rows));

}

void Engine::MEngine::run() {
	cerr << "Started running query." << endl;
	vector<OperationTree::ScanOperation_Type> types = root_operation_ -> init();

	cerr << "Result type" << endl;
	for (int i = 0; i < types.size(); ++i) {
		cerr << types[i] << " ";
	}
	cerr << endl;

	int all_rows = 0;
	int whoGathers = 0;
	BlockSerializer blockSerializer;

	if (whoGathers == nei_ ->my_node_number()) {
		DataSinkInterface * sink = nei_ -> OpenDataSink();

		int ile = nei_ -> nodes_count() - 1;
		size_t len;
		while (ile) {
			char * buffer = nei_ -> ReadPacketBlocking(&len);
			if (len == 0) {
				while (true);
				assert(0);
				ile--;
			}
			vector<void*> data;
			int rows = blockSerializer.deserializeBlock(types, len, buffer,
					data);
			for (int i = 0; i < types.size(); ++i) {
				switch (types[i]) {
				case SINT:
					sink -> ConsumeInts(i, rows, static_cast<int32*> (data[i]));
					break;
				case SDOUBLE:
					sink -> ConsumeDoubles(i, rows,
							static_cast<double*> (data[i]));
					break;
				case SBOOL:
					sink -> ConsumeByteBools(i, rows,
							static_cast<bool*> (data[i]));
					break;
				}
				delete data[i];
			}
		}
	} else {
		while (true) {
			int rows = max_rows_;
			vector<void*> data = root_operation_ -> pull(rows);
			if (rows == 0) {
				nei_ -> SendPacket(whoGathers, NULL, 0);
				break;
			}
			char * buffer;
			int bufferSize = blockSerializer.serializeBlock(types, data, rows,
					&buffer);
				nei_ -> SendPacket(whoGathers, buffer, bufferSize);

				delete buffer;

			all_rows += rows;
			if (debug)
				cerr << "Worker " << nei_ -> my_node_number() << " GOT "
						<< rows << " ROWS" << endl;

			/*
			 for(int i = 0; i < data.size(); ++i) {
			 switch(types[i]) {
			 case OperationTree::ScanOperation_Type_INT:
			 server_ -> ConsumeInts(i, rows, static_cast<int32*>(data[i]));
			 break;
			 case OperationTree::ScanOperation_Type_DOUBLE:
			 server_ -> ConsumeDoubles(i, rows, static_cast<double*>(data[i]));
			 break;
			 case OperationTree::ScanOperation_Type_BOOL:
			 server_ -> ConsumeByteBools(i, rows, static_cast<bool*>(data[i]));
			 break;
			 }
			 }*/
		}

		cerr << "=============================" << endl;
		cerr << "Consumed " << all_rows << " rows " << endl;
		cerr << "Completed query." << endl;
	}
}

