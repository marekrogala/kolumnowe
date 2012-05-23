#include "scan.h"
#include "columnStruct.h"

class Sink {
	public:
		static void consume(Server* server, ColumnStruct* output);
};
