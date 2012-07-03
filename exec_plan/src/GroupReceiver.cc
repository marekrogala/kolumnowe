#include <cassert>
#include <iostream>

#include "GroupReceiver.h"
#include "BlockSerializer.h"

namespace Engine {

GroupReceiver::GroupReceiver(NodeEnvironmentInterface *nei, 
		std::vector<OperationTree::ScanOperation_Type> source_types, Operation*source) : 
	Operation(nei), nei_(nei), source_types_(source_types), source_(source) {
		eofs_to_be_received = Layers::count_nodes_in_other_layer();
	}

std::vector<void*> GroupReceiver::pull(int &rows) {
	cerr << "GroupReceiver::pull " << rows << endl;	
	char *data = NULL;
	size_t data_len;
	if (source_ != NULL) {
		source_ -> pull(rows);
		source_ = NULL;
	}
		std::cerr << "Receiving..." << std::endl;

		//int eofs_to_be_received = Layers::count_nodes_in_other_layer();

		do {
			if(eofs_to_be_received == 0)
				break;
				
			if(data != NULL) { free(data); data = NULL; }
		  
		  data = nei_->ReadPacketBlocking(&data_len);
			cerr <<data_len << endl;
		  if(data_len == 0) eofs_to_be_received--;
		} while(data_len == 0 && eofs_to_be_received > 0);
		
		if(eofs_to_be_received == 0) {
		  rows = 0;
		  return std::vector<void*>();
		}
		
		BlockSerializer serializer;
		std::vector<void*> buffers;
		rows = serializer.deserializeBlock(source_types_, data_len, data, buffers);
		
		if(data != NULL) { free(data); data = NULL; }
		
		cerr << "RECIVED: \n";
		printCols(source_types_, buffers, rows);
		cerr << "END\n";
	//		//cerr << "SCOLS: " << source_types_.size() << "COLS: " <<data.size() <<" ROWS: "<< rows << "\n";
	//		for(int row = 0; row < rows; row++){
	//			for (int column = 0, columns = buffers_.size(); column < columns; ++column) {
	//				cerr << "REC ROW: ";
	//				switch (source_types_[column]) {
	//					case OperationTree::ScanOperation_Type_INT:
	//						cerr << * ((int*) buffers_[column] + row) <<" ";
	//						break;
	//					case OperationTree::ScanOperation_Type_DOUBLE:
	//						cerr << * ((double*) buffers_[column] + row) <<" ";
	//					case OperationTree::ScanOperation_Type_BOOL:
	//						cerr <<  * ((bool*) buffers_[column] + row) <<" ";
	//					}
	//			}
	//			cerr << endl; 
	//	}

		return buffers;
}

std::vector<OperationTree::ScanOperation_Type> GroupReceiver::init() {
	assert(false);
}

}
