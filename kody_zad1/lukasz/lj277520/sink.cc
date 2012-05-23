#include "sink.h"

void Sink::consume(Server* server, ColumnStruct* output) {
	for(int i = 0; i < output->size; i++) {
		switch(output->types[i]) {
			case INT:
				//server->ConsumeInts(i, output->sizes[i], (int32*)(output->columns[i]));
				server->ConsumeInts(i, output->nrows, (int32*)(output->columns[i]));
				break;
			case DOUBLE:
				//server->ConsumeDoubles(i, output->sizes[i], (double*)(output->columns[i]));
				server->ConsumeDoubles(i, output->nrows, (double*)(output->columns[i]));
				break;
			case BOOL:
				//server->ConsumeByteBools(i, output->sizes[i], (bool*)(output->columns[i]));
				server->ConsumeByteBools(i, output->nrows, (bool*)(output->columns[i]));
				break;
		}
	}
	delete output;
}
