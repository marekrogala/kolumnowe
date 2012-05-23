#include "MEngine.h"
#include "operations.pb.h"
#include "ScanOperation.h"
#include "OperationBuilder.h"

Engine::MEngine::MEngine(const OperationTree::Operation &operation, Server * server, int max_rows) : server_(server), max_rows_(max_rows) {
    root_operation_ =  OperationBuilder::build(server, operation, new MemoryManager(max_rows));

}

void Engine::MEngine::run() {
    cerr << "Started running query." << endl;
    vector<OperationTree::ScanOperation_Type> types = root_operation_ -> init();

    cerr << "Result type" << endl;
    for(int i = 0; i < types.size(); ++i) {
    	cerr << types[i] << " ";
    }
    cerr << endl;

    int all_rows = 0;
    while (true) {
        int rows = max_rows_;
        vector<void*> data = root_operation_ -> pull(rows);
        all_rows += rows;
        if (debug) cerr << "GOT " << rows << " ROWS" << endl;

        if (rows == 0) break;

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
        }
    } 

    cerr << "=============================" << endl;
    cerr << "Consumed " << all_rows << " rows " << endl;
    cerr << "Completed query." << endl;
}




