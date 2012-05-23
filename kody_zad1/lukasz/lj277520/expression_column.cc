#include "expression_column.h"


Expression_column::Expression_column(int columnId) : columnId(columnId) {
}

Expression_column::~Expression_column() {
}

int Expression_column::getResult(ColumnStruct* columns, void** destination) {
	return columns->getColumnCopy(columnId, destination);
}
