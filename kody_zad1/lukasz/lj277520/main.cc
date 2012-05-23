
//#include "server.h"
#include "dboperation.h"
#include "scan.h"
#include "compute.h"
#include "filter.h"
#include "dbexpression.h"
#include "expression_column.h"

#include "expression_oper.h"
#include "expression_oper1.h"
#include "expression_const.h"
#include "expression_if.h"

#include "groupBy.h"
#include "aggre_count.h"
#include "aggre_sum.h"

#include "sink.h"

#include "plan_tlanslator.h"

#include <google/protobuf/text_format.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>

#include <fcntl.h>

#define s 1024

int main(int argc, char *argv[]) {
	int c = strtol(argv[1], NULL, 0);
	Server *server = CreateServer(c);
	Operation op;
	int fd = open(argv[2], O_RDONLY);
	// Zero is the stdin file descriptor.
	google::protobuf::io::FileInputStream input(fd);
	google::protobuf::TextFormat::Parse(&input, &op);
	DBOperation* o = translatePlan(server, &op);
	ColumnStruct* result;
	int n = o->getOutput(s, &result);
	while(n != 0) {
		Sink::consume(server, result);
		n = o->getOutput(s, &result);
	}
	delete result;
//	n = o->getOutput(s, &result);
////	while(n != 0) {
//		Sink::consume(server, result);
	delete o;
	delete server;
}

/*int main() {
	int ints[5];
	Server *server = CreateServer(1);
//	Operation* scan = new Scan(server);
	columnType* ct = new columnType[2];
	ct[0] = INT;
	ct[1] = INT;
	int* sc = new int[2];
	sc[0] = 0;
	sc[1] = 1;
	DBOperation* scan = new Scan(server, 2, ct, sc);
//	int n = server->GetInts(0, 5, ints);
	DBExpression* ecol0 = new Expression_column(0);
//	Expression** e = new Expression*[1];
//	e[0] = ecol0;
	DBExpression* ecol1 = new Expression_column(1);
//	Expression* expr = new Expression_add(ecol0, ecol1);
	DBExpression* exprOperAdd = new Expression_oper<int32, int32, int32, addS<int32, int32, int32> >(ecol0, ecol1);
	DBExpression* exprOperNeg = new Expression_oper1<int32, int32, negS<int32, int32> >(ecol0);
	DBExpression** e = new DBExpression*[1];
	//e[0] = expr;
	e[0] = exprOperAdd;
//	e[0] = exprOperNeg;
	columnType* ct2 = new columnType[1];
	ct2[0] = INT;

	DBExpression* constant = new Expression_const<int32>(36);
	DBExpression* gt = new Expression_oper<int32, int32, bool, gtS<int32, int32, bool> >(ecol0, constant);
	DBExpression* exprIf = new Expression_if<int32>(gt, exprOperNeg, ecol0); // wiem ze tu bedzie problem przy zwalnianiu pamieci - col0 - stworzyc na nowo
//	e[0] = exprIf;
//	DBOperation* filter = new Filter(scan, gt);
	DBOperation* compute = new Compute(scan, 1, ct2, e);

	columnType* ctt = new columnType[3];
	ctt[0] = INT;
	ctt[1] = INT;
	ctt[3] = INT;

	int* gbc = new int[1];
	gbc[0] = 0;

	DBAggregation* agCount = new DBAggregation_count();
	DBAggregation* agSum = new DBAggregation_sum(1);
	DBAggregation** agregations = new DBAggregation*[2];
	agregations[0] = agCount;
	agregations[1] = agSum;

	DBOperation* groupBy = new GroupBy(scan, 1, 2, ctt, gbc, agregations);

//	int n = compute->getInts(0, 5, ints);
	ColumnStruct* result;
	//int n = scan->getOutput(10, &result);
//	int n = filter->getOutput(10, &result);
//	int n = compute->getOutput(10, &result);
	int n  = groupBy->getOutput(10, &result);
	Sink::consume(server, result);
	//n = scan->getOutput(6, &result);
	//n = filter->getOutput(5, &result);
	//int n = compute->getOutput(5, &result);
	//Sink::consume(server, result);
// 	server->ConsumeInts(0, n, ints);
	//n = scan->getInts(1, 5, ints);
 	//server->ConsumeInts(1, n, ints);
	return 0;
}*/
