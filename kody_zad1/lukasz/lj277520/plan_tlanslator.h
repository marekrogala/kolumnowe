#ifndef PLAN_TLANSLATOR_H
#define PLAN_TLANSLATOR_H

#include <stdint.h>
#include <math.h>
#include <iostream>

#include "columnStruct.h"
#include "scan.h"
#include "dboperation.h"
#include "operations.pb.h"

#include "dbexpression.h"
#include "dbaggregation.h"

typedef int32_t int32;

DBOperation* translateScan(Server* server, const ScanOperation* scan);
DBOperation* translateCompute(Server* server, const ComputeOperation* compute);
DBOperation* translateFilter(Server* server, const FilterOperation* filter);
DBOperation* translateGroupBy(Server* server, const GroupByOperation* groupby);
DBOperation* translatePlan(Server* server, const Operation* op);
DBAggregation* translateAggregation(int sourceSize, columnType* sourceTypes, const Aggregation* aggr, columnType* ct);
DBExpression* translateExpr(int sourceSize, columnType* sourceTypes, const Expression* expr, columnType* ct);

#endif
