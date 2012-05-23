#include <cstring>
#include "plan_tlanslator.h"

#include "scan.h"
#include "compute.h"
#include "filter.h"
#include "groupBy.h"

#include "expression_const.h"
#include "expression_column.h"
#include "expression_oper.h"
#include "expression_oper1.h"
#include "expression_if.h"

#include "aggre_count.h"
#include "aggre_sum.h"

DBOperation* translateScan(Server* server, const ScanOperation* scan) {
	int n = scan->column().size();
	int* sourceColumnIds = new int[n];
	columnType* columnsTypes = new columnType[n];
	for(int i = 0; i < n; i++) {
		sourceColumnIds[i] = scan->column(i);
		ScanOperation_Type t = scan->type(i);
		switch(t) {
			case ScanOperation_Type_INT:
				columnsTypes[i] = INT;
				break;
			case ScanOperation_Type_DOUBLE:
				columnsTypes[i] = DOUBLE;
				break;
			case ScanOperation_Type_BOOL:
				columnsTypes[i] = BOOL;
				break;
		}
	}

	DBOperation* result = new Scan(server, n, columnsTypes, sourceColumnIds);
	return result;
}

DBOperation* translateCompute(Server* server, const ComputeOperation* compute) {
	DBOperation* source = translatePlan(server, &(compute->source()));
	int n = compute->expressions().size();
	DBExpression** expr = new DBExpression*[n];
	columnType* columnsTypes = new columnType[n];
	columnType* sourceTypes;
	int sourceSize = source->copyColumnsTypes(&sourceTypes);
	columnType ct;
	for(int i = 0; i < n; i++) {
		expr[i] = translateExpr(sourceSize, sourceTypes, &(compute->expressions(i)), &ct);
		columnsTypes[i] = ct;
	}
	DBOperation* result = new Compute(source, n, columnsTypes, expr);
	delete[] sourceTypes;
	return result;
}

DBOperation* translateFilter(Server* server, const FilterOperation* filter) {
	DBOperation* source = translatePlan(server, &(filter->source()));
	columnType* sourceTypes;
	int sourceSize = source->copyColumnsTypes(&sourceTypes);
	columnType ct;
	DBExpression* expr = translateExpr(sourceSize, sourceTypes, &(filter->expression()), &ct);
	DBOperation* result = new Filter(source, sourceSize, sourceTypes, expr);
	return result;
}

DBOperation* translateGroupBy(Server* server, const GroupByOperation* groupby) {
	DBOperation* source = translatePlan(server, &(groupby->source()));
	int groupByColNum = groupby->group_by_column().size();
	int groupByAggrNum = groupby->aggregations().size();
	int* groupByColumns = new int[groupByColNum];
	DBAggregation** aggr = new DBAggregation*[groupByAggrNum];
	columnType* columnsTypes = new columnType[groupByColNum+groupByAggrNum];

	columnType* sourceTypes;
	int sourceSize = source->copyColumnsTypes(&sourceTypes);

	for(int i = 0; i < groupByColNum; i++) {
		groupByColumns[i] = groupby->group_by_column(i);
		columnsTypes[i] = sourceTypes[groupByColumns[i]];
	}

	columnType ct;
	for(int i = 0; i < groupByAggrNum; i++) {
		aggr[i] = translateAggregation(sourceSize, sourceTypes, &(groupby->aggregations(i)), &ct);
		columnsTypes[groupByColNum + i] = ct;
	}

	DBOperation* result = new GroupBy(source, groupByColNum, groupByAggrNum, columnsTypes, groupByColumns, aggr);
	delete[] sourceTypes;
	return result;
}

DBOperation* translatePlan(Server* server, const Operation* op) {
	if(op->has_scan()) {
		return translateScan(server, &(op->scan()));
	}
	else if(op->has_compute()) {
		return translateCompute(server, &(op->compute()));
	}
	else if(op->has_filter()) {
		return translateFilter(server, &(op->filter()));
	}
	else if(op->has_group_by()) {
		return translateGroupBy(server, &(op->group_by()));
	}
}

DBAggregation* translateAggregation(int sourceSize, columnType* sourceTypes, const Aggregation* aggr, columnType* ct) {
	DBAggregation* result;
	switch(aggr->type()) {
		case Aggregation_Type_COUNT:
			result = new DBAggregation_count();
			*ct = INT;
			break;
		case Aggregation_Type_SUM:
			int n = aggr->aggregated_column();
			result = new DBAggregation_sum(n);
			if(sourceTypes[n] == BOOL)
				*ct = INT;
			else
				*ct = sourceTypes[n];
	}
	return result;
}

DBExpression* translateExpr(int sourceSize, columnType* sourceTypes, const Expression* expr, columnType* ct) {
	DBExpression* result;
	switch(expr->operator_()) {
		case Expression_Operator_CONSTANT:
			if(expr->has_constant_int32()) {
				result = new Expression_const<int32>(expr->constant_int32());
				*ct = INT;
			}
			else if(expr->has_constant_double()) {
				result = new Expression_const<double>(expr->constant_double());
				*ct = DOUBLE;
			}
			else if(expr->has_constant_bool()) {
				result = new Expression_const<bool>(expr->constant_bool());
				*ct = BOOL;
			}
			break;
		case Expression_Operator_COLUMN:
			result = new Expression_column(expr->column_id());
			*ct = sourceTypes[expr->column_id()];
			break;
		case Expression_Operator_IF: {
			DBExpression* expri = translateExpr(sourceSize, sourceTypes, &(expr->children(0)), ct);
			DBExpression* expr1 = translateExpr(sourceSize, sourceTypes, &(expr->children(1)), ct);
			DBExpression* expr2 = translateExpr(sourceSize, sourceTypes, &(expr->children(2)), ct);
			switch(*ct) {
				case INT:
					result = new Expression_if<int32>(expri, expr1, expr2);
					break;
				case DOUBLE:
					result = new Expression_if<double>(expri, expr1, expr2);
					break;
				case BOOL:
					result = new Expression_if<bool>(expri, expr1, expr2);
					break;

			}
		}
			break;
		case Expression_Operator_ADD: {
			columnType ct1;
			columnType ct2;
			DBExpression* expr1 = translateExpr(sourceSize, sourceTypes, &(expr->children(0)), &ct1);
			DBExpression* expr2 = translateExpr(sourceSize, sourceTypes, &(expr->children(1)), &ct2);
			if(ct1 == INT && ct2 == INT) {
				result = new Expression_oper<int32, int32, int32, addS<int32, int32, int32> >(expr1, expr2);
				*ct = INT;
			}
			else if(ct1 == INT && ct2 == DOUBLE) {
				result = new Expression_oper<int32, double, double, addS<int32, double, double> >(expr1, expr2);
				*ct = DOUBLE;
			}
			else if(ct1 == DOUBLE && ct2 == INT) {
				result = new Expression_oper<double, int32, double, addS<double, int32, double> >(expr1, expr2);
				*ct = DOUBLE;
			}
			else if(ct1 == DOUBLE && ct2 == DOUBLE) {
				result = new Expression_oper<double, double, double, addS<double, double, double> >(expr1, expr2);
				*ct = DOUBLE;
			}
		}
			break;
		case Expression_Operator_SUBTRACT: {
			columnType ct1;
			columnType ct2;
			DBExpression* expr1 = translateExpr(sourceSize, sourceTypes, &(expr->children(0)), &ct1);
			DBExpression* expr2 = translateExpr(sourceSize, sourceTypes, &(expr->children(1)), &ct2);
			if(ct1 == INT && ct2 == INT) {
				result = new Expression_oper<int32, int32, int32, subS<int32, int32, int32> >(expr1, expr2);
				*ct = INT;
			}
			else if(ct1 == INT && ct2 == DOUBLE) {
				result = new Expression_oper<int32, double, double, subS<int32, double, double> >(expr1, expr2);
				*ct = DOUBLE;
			}
			else if(ct1 == DOUBLE && ct2 == INT) {
				result = new Expression_oper<double, int32, double, subS<double, int32, double> >(expr1, expr2);
				*ct = DOUBLE;
			}
			else if(ct1 == DOUBLE && ct2 == DOUBLE) {
				result = new Expression_oper<double, double, double, subS<double, double, double> >(expr1, expr2);
				*ct = DOUBLE;
			}
		}
			break;
		case Expression_Operator_MULTIPLY: {
			columnType ct1;
			columnType ct2;
			DBExpression* expr1 = translateExpr(sourceSize, sourceTypes, &(expr->children(0)), &ct1);
			DBExpression* expr2 = translateExpr(sourceSize, sourceTypes, &(expr->children(1)), &ct2);
			if(ct1 == INT && ct2 == INT) {
				result = new Expression_oper<int32, int32, int32, mulS<int32, int32, int32> >(expr1, expr2);
				*ct = INT;
			}
			else if(ct1 == INT && ct2 == DOUBLE) {
				result = new Expression_oper<int32, double, double, mulS<int32, double, double> >(expr1, expr2);
				*ct = DOUBLE;
			}
			else if(ct1 == DOUBLE && ct2 == INT) {
				result = new Expression_oper<double, int32, double, mulS<double, int32, double> >(expr1, expr2);
				*ct = DOUBLE;
			}
			else if(ct1 == DOUBLE && ct2 == DOUBLE) {
				result = new Expression_oper<double, double, double, mulS<double, double, double> >(expr1, expr2);
				*ct = DOUBLE;
			}
		}
			break;
		case Expression_Operator_FLOATING_DIVIDE: {
			columnType ct1;
			columnType ct2;
			DBExpression* expr1 = translateExpr(sourceSize, sourceTypes, &(expr->children(0)), &ct1);
			DBExpression* expr2 = translateExpr(sourceSize, sourceTypes, &(expr->children(1)), &ct2);
			if(ct1 == INT && ct2 == INT) {
				result = new Expression_oper<int32, int32, double, divS<int32, int32, double> >(expr1, expr2);
				*ct = DOUBLE;
			}
			else if(ct1 == INT && ct2 == DOUBLE) {
				result = new Expression_oper<int32, double, double, divS<int32, double, double> >(expr1, expr2);
				*ct = DOUBLE;
			}
			else if(ct1 == DOUBLE && ct2 == INT) {
				result = new Expression_oper<double, int32, double, divS<double, int32, double> >(expr1, expr2);
				*ct = DOUBLE;
			}
			else if(ct1 == DOUBLE && ct2 == DOUBLE) {
				result = new Expression_oper<double, double, double, divS<double, double, double> >(expr1, expr2);
				*ct = DOUBLE;
			}
		}
			break;
		case Expression_Operator_LOG: {
			columnType ct1;
			DBExpression* expr1 = translateExpr(sourceSize, sourceTypes, &(expr->children(0)), &ct1);
			if(ct1 == INT) {
				result = new Expression_oper1<int32, double, logS<int32, double> >(expr1);
			}
			else if(ct1 == DOUBLE) {
				result = new Expression_oper1<double, double, logS<double, double> >(expr1);
			}
			*ct = DOUBLE;
		}
			break;
		case Expression_Operator_NEGATE: {
			columnType ct1;
			DBExpression* expr1 = translateExpr(sourceSize, sourceTypes, &(expr->children(0)), &ct1);
			if(ct1 == INT) {
				result = new Expression_oper1<int32, int32, negS<int32, int32> >(expr1);
				*ct = INT;
			}
			else if(ct1 == DOUBLE) {
				result = new Expression_oper1<double, double, negS<double, double> >(expr1);
				*ct = DOUBLE;
			}
		}
			break;
		case Expression_Operator_LOWER: {
			columnType ct1;
			columnType ct2;
			DBExpression* expr1 = translateExpr(sourceSize, sourceTypes, &(expr->children(0)), &ct1);
			DBExpression* expr2 = translateExpr(sourceSize, sourceTypes, &(expr->children(1)), &ct2);
			if(ct1 == INT && ct2 == INT) {
				result = new Expression_oper<int32, int32, bool, ltS<int32, int32, bool> >(expr1, expr2);
			}
			else if(ct1 == INT && ct2 == DOUBLE) {
				result = new Expression_oper<int32, double, bool, ltS<int32, double, bool> >(expr1, expr2);
			}
			else if(ct1 == DOUBLE && ct2 == INT) {
				result = new Expression_oper<double, int32, bool, ltS<double, int32, bool> >(expr1, expr2);
			}
			else if(ct1 == DOUBLE && ct2 == DOUBLE) {
				result = new Expression_oper<double, double, bool, ltS<double, double, bool> >(expr1, expr2);
			}
			else if(ct1 == BOOL && ct2 == BOOL) {
				result = new Expression_oper<bool, bool, bool, ltS<bool, bool, bool> >(expr1, expr2);
			}
			*ct = BOOL;
		}
			break;
		case Expression_Operator_GREATER: {
			columnType ct1;
			columnType ct2;
			DBExpression* expr1 = translateExpr(sourceSize, sourceTypes, &(expr->children(0)), &ct1);
			DBExpression* expr2 = translateExpr(sourceSize, sourceTypes, &(expr->children(1)), &ct2);
			if(ct1 == INT && ct2 == INT) {
				result = new Expression_oper<int32, int32, bool, gtS<int32, int32, bool> >(expr1, expr2);
			}
			else if(ct1 == INT && ct2 == DOUBLE) {
				result = new Expression_oper<int32, double, bool, gtS<int32, double, bool> >(expr1, expr2);
			}
			else if(ct1 == DOUBLE && ct2 == INT) {
				result = new Expression_oper<double, int32, bool, gtS<double, int32, bool> >(expr1, expr2);
			}
			else if(ct1 == DOUBLE && ct2 == DOUBLE) {
				result = new Expression_oper<double, double, bool, gtS<double, double, bool> >(expr1, expr2);
			}
			else if(ct1 == BOOL && ct2 == BOOL) {
				result = new Expression_oper<bool, bool, bool, gtS<bool, bool, bool> >(expr1, expr2);
			}
			*ct = BOOL;
		}
			break;
		case Expression_Operator_EQUAL: {
			columnType ct1;
			columnType ct2;
			DBExpression* expr1 = translateExpr(sourceSize, sourceTypes, &(expr->children(0)), &ct1);
			DBExpression* expr2 = translateExpr(sourceSize, sourceTypes, &(expr->children(1)), &ct2);
			if(ct1 == INT && ct2 == INT) {
				result = new Expression_oper<int32, int32, bool, eqS<int32, int32, bool> >(expr1, expr2);
			}
			else if(ct1 == INT && ct2 == DOUBLE) {
				result = new Expression_oper<int32, double, bool, eqS<int32, double, bool> >(expr1, expr2);
			}
			else if(ct1 == DOUBLE && ct2 == INT) {
				result = new Expression_oper<double, int32, bool, eqS<double, int32, bool> >(expr1, expr2);
			}
			else if(ct1 == DOUBLE && ct2 == DOUBLE) {
				result = new Expression_oper<double, double, bool, eqS<double, double, bool> >(expr1, expr2);
			}
			else if(ct1 == BOOL && ct2 == BOOL) {
				result = new Expression_oper<bool, bool, bool, eqS<bool, bool, bool> >(expr1, expr2);
			}
			*ct = BOOL;
		}
			break;
		case Expression_Operator_NOT_EQUAL: {
			columnType ct1;
			columnType ct2;
			DBExpression* expr1 = translateExpr(sourceSize, sourceTypes, &(expr->children(0)), &ct1);
			DBExpression* expr2 = translateExpr(sourceSize, sourceTypes, &(expr->children(1)), &ct2);
			if(ct1 == INT && ct2 == INT) {
				result = new Expression_oper<int32, int32, bool, neqS<int32, int32, bool> >(expr1, expr2);
			}
			else if(ct1 == INT && ct2 == DOUBLE) {
				result = new Expression_oper<int32, double, bool, neqS<int32, double, bool> >(expr1, expr2);
			}
			else if(ct1 == DOUBLE && ct2 == INT) {
				result = new Expression_oper<double, int32, bool, neqS<double, int32, bool> >(expr1, expr2);
			}
			else if(ct1 == DOUBLE && ct2 == DOUBLE) {
				result = new Expression_oper<double, double, bool, neqS<double, double, bool> >(expr1, expr2);
			}
			else if(ct1 == BOOL && ct2 == BOOL) {
				result = new Expression_oper<bool, bool, bool, neqS<bool, bool, bool> >(expr1, expr2);
			}
			*ct = BOOL;
		}
			break;
		case Expression_Operator_NOT: {
			columnType ct1;
			DBExpression* expr1 = translateExpr(sourceSize, sourceTypes, &(expr->children(0)), &ct1);
			result = new Expression_oper1<bool, bool, notS<bool, bool> >(expr1);
			*ct = BOOL;
		}
			break;
		case Expression_Operator_OR: {
			columnType ct1;
			columnType ct2;
			DBExpression* expr1 = translateExpr(sourceSize, sourceTypes, &(expr->children(0)), &ct1);
			DBExpression* expr2 = translateExpr(sourceSize, sourceTypes, &(expr->children(1)), &ct2);
			result = new Expression_oper<bool, bool, bool, orS<bool, bool, bool> >(expr1, expr2);
			*ct = BOOL;
		}
			break;
		case Expression_Operator_AND: {
			columnType ct1;
			columnType ct2;
			DBExpression* expr1 = translateExpr(sourceSize, sourceTypes, &(expr->children(0)), &ct1);
			DBExpression* expr2 = translateExpr(sourceSize, sourceTypes, &(expr->children(1)), &ct2);
			result = new Expression_oper<bool, bool, bool, andS<bool, bool, bool> >(expr1, expr2);
			*ct = BOOL;
		}
			break;
	}
	return result;
}
