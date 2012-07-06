/*
 * ExpressionNode.cpp
 *
 *  Created on: 9 Apr 2012
 *      Author: maciek
 */

#include "ExpressionNode.h"
#include <iostream>
#include <cmath>

using namespace std;

// XXX ASSUMPTION sizeof(double) >= sizeof(int32)

// TODO check this shit before submitting!!!!

#define do_if_oper(b3, aim_buff_t3, warunek_buff, b1, b2, t1, t2, aim_t1, aim_t2, aim_buff_t1, aim_buff_t2, rows) if ((t1) == (aim_t1) && (t2) == (aim_t2)) \
{ for(int i = 0; i < (rows); ++i) if (((bool*)warunek_buff)[i]) ((aim_buff_t3*)(b3))[i] = (aim_buff_t3(((aim_buff_t1*)(b1))[i])); else ((aim_buff_t3*)(b3))[i] = (aim_buff_t3(((aim_buff_t2*)(b2))[i])); return b3;}

#define do_2_arg_oper(b3, aim_buff_t3, b1, b2, t1, t2, aim_t1, aim_t2, aim_buff_t1, aim_buff_t2, rows, oper) if ((t1) == (aim_t1) && (t2) == (aim_t2)) \
{ for(int i = 0; i < (rows); ++i) ((aim_buff_t3*)(b3))[i] = (aim_buff_t3(((aim_buff_t1*)(b1))[i])) oper (aim_buff_t3(((aim_buff_t2*)(b2))[i])); return b3;}

#define do_2_arg_comp_oper(b3, aim_buff_t3, b1, b2, t1, t2, aim_t1, aim_t2, aim_buff_t1, aim_buff_t2, rows, oper) if ((t1) == (aim_t1) && (t2) == (aim_t2)) \
{ for(int i = 0; i < (rows); ++i) ((aim_buff_t3*)(b3))[i] = ((((aim_buff_t1*)(b1))[i])) oper ((((aim_buff_t2*)(b2))[i])); return b3;}


#define do_1_arg_oper(b3, aim_buff_t3, b1, t1, aim_t1, aim_buff_t1, rows, oper) if ((t1) == (aim_t1)) \
{ for(int i = 0; i < (rows); ++i) ((aim_buff_t3*)(b3))[i] = oper (aim_buff_t3(((aim_buff_t1*)(b1))[i])); return b3;}


namespace Engine {

ExpressionNode::ExpressionNode(const OperationTree::Expression * expr, Operation * oper, MemoryManager * mem_manager)
	: expr_(expr), oper_(oper), mem_manager_(mem_manager) {
		size_t n = expr -> children_size();
		for(size_t i = 0; i < n; ++i) {
			t_.push_back(new ExpressionNode(&(expr -> children(i)), oper, mem_manager));
		}
	}

OperationTree::ScanOperation_Type ExpressionNode::init() {
//	if (debug) cerr << "ExpressionNode::init() " << t_.size() << endl;
		for(size_t i = 0; i < t_.size(); ++i) {
			types_.push_back(t_[i] -> init());
		}

	OperationTree::Expression_Operator oper = expr_ -> operator_();

		switch (oper) {
			case OperationTree::Expression_Operator_CONSTANT :
				buffers_.push_back(mem_manager_ -> allocate_normal());
				if (expr_ -> has_constant_bool()) {
					return SBOOL;
				}

				if (expr_ -> has_constant_int32()) {
					return SINT;
				}

				if (expr_ -> has_constant_double()) {
					return SDOUBLE;
				}

				if (debug) assert(false);

			case OperationTree::Expression_Operator_COLUMN :
				buffers_.push_back(mem_manager_ -> allocate_normal());
				return oper_ -> get_source_type(expr_ -> column_id());

			case OperationTree::Expression_Operator_IF :
				buffers_.push_back(mem_manager_ -> allocate_normal());
				buffers_.push_back(mem_manager_ -> allocate_normal());
				buffers_.push_back(mem_manager_ -> allocate_normal());
				return types_[1];

			case OperationTree::Expression_Operator_ADD :
			case OperationTree::Expression_Operator_MULTIPLY:
			case OperationTree::Expression_Operator_SUBTRACT:
				//if (types_[1] == SDOUBLE) swap(types_[0], types_[1]);
				if (types_[0] == SDOUBLE || types_[1] == SDOUBLE) return SDOUBLE;
				else return SINT;

			case OperationTree::Expression_Operator_FLOATING_DIVIDE:
			case OperationTree::Expression_Operator_LOG:
				buffers_.push_back(mem_manager_ -> allocate_normal());
				return SDOUBLE;

			case OperationTree::Expression_Operator_NEGATE:
				return types_[0];

			case OperationTree::Expression_Operator_NOT:
			case OperationTree::Expression_Operator_NOT_EQUAL:
			case OperationTree::Expression_Operator_EQUAL:
			case OperationTree::Expression_Operator_OR:
			case OperationTree::Expression_Operator_AND:
			case OperationTree::Expression_Operator_LOWER:
			case OperationTree::Expression_Operator_GREATER:
				buffers_.push_back(mem_manager_ -> allocate_normal());
				return SBOOL;
			default : assert(false);
		}
	}

void* ExpressionNode::pull(int rows) {
	OperationTree::Expression_Operator oper = expr_ -> operator_();

		switch (oper) {
			case OperationTree::Expression_Operator_CONSTANT :
			{
				if (debug) assert(buffers_.size() == 1);

				int max_rows = mem_manager_ -> max_rows();
				if (expr_ -> has_constant_bool()) {
					bool* b = static_cast<bool*>(buffers_.back());
					bool v = expr_ -> constant_bool();
					for(int i = 0; i < max_rows; ++i) b[i] = v;
				}

				if (expr_ -> has_constant_int32()) {
					int32* b = static_cast<int32*>(buffers_.back());
					int32 v = expr_ -> constant_int32();
					for(int i = 0; i < max_rows; ++i) b[i] = v;
				}

				if (expr_ -> has_constant_double()) {
					double* b = static_cast<double*>(buffers_.back());
					double v = expr_ -> constant_double();
					for(int i = 0; i < max_rows; ++i) b[i] = v;
				}
			}
				return buffers_.back();

			case OperationTree::Expression_Operator_COLUMN :
			{
				void * b = buffers_.back();
				switch (oper_ -> get_source_type(expr_ -> column_id())) {
				case SDOUBLE:
					memcpy(b, oper_ -> get_data(expr_ -> column_id()), rows * sizeof(double));
					break;
				case SINT:
					memcpy(b, oper_ -> get_data(expr_ -> column_id()), rows * sizeof(int32));
					break;
				case SBOOL:
					memcpy(b, oper_ -> get_data(expr_ -> column_id()), rows * sizeof(bool));
					break;
				}

				return b;
			}

			case OperationTree::Expression_Operator_IF :
			{
				void * warunek;
				void *b0;
				void *b1;
				warunek = t_[0] -> pull(rows);
				b0 = t_[1] -> pull(rows);
				b1 = t_[2] -> pull(rows);
				do_if_oper(b0, double, warunek, b0, b1, types_[1], types_[2], SDOUBLE, SDOUBLE, double, double, rows);
				do_if_oper(b0, int, warunek, b0, b1, types_[1], types_[2], SINT, SINT, int, int, rows);
				do_if_oper(b0, bool, warunek, b0, b1, types_[1], types_[2], SBOOL, SBOOL, bool, bool, rows);
				assert(false);
			}

			case OperationTree::Expression_Operator_ADD :
			{
				void *b0;
				void *b1;
				b0 = t_[0] -> pull(rows);
				b1 = t_[1] -> pull(rows);
				do_2_arg_oper(b0, double, b0, b1, types_[0], types_[1], SDOUBLE, SDOUBLE, double, double, rows, +);
				do_2_arg_oper(b0, double, b0, b1, types_[0], types_[1], SDOUBLE, SINT, double, int, rows, +);
				do_2_arg_oper(b1, double, b0, b1, types_[0], types_[1], SINT, SDOUBLE, int, double, rows, +);
				do_2_arg_oper(b0, int, b0, b1, types_[0], types_[1], SINT, SINT, int, int, rows, +);
				assert(false);
			}

			case OperationTree::Expression_Operator_MULTIPLY:
			{
				void *b0;
				void *b1;
				b0 = t_[0] -> pull(rows);
				b1 = t_[1] -> pull(rows);
				do_2_arg_oper(b0, double, b0, b1, types_[0], types_[1], SDOUBLE, SDOUBLE, double, double, rows, *);
				do_2_arg_oper(b0, double, b0, b1, types_[0], types_[1], SDOUBLE, SINT, double, int, rows, *);
				do_2_arg_oper(b1, double, b0, b1, types_[0], types_[1], SINT, SDOUBLE, int, double, rows, *);
				do_2_arg_oper(b0, int, b0, b1, types_[0], types_[1], SINT, SINT, int, int, rows, *);
				assert(false);
			}

			case OperationTree::Expression_Operator_SUBTRACT:
			{
				void *b0;
				void *b1;
				b0 = t_[0] -> pull(rows);
				b1 = t_[1] -> pull(rows);
				do_2_arg_oper(b0, double, b0, b1, types_[0], types_[1], SDOUBLE, SDOUBLE, double, double, rows, -);
				do_2_arg_oper(b0, double, b0, b1, types_[0], types_[1], SDOUBLE, SINT, double, int, rows, -);
				do_2_arg_oper(b1, double, b0, b1, types_[0], types_[1], SINT, SDOUBLE, int, double, rows, -);
				do_2_arg_oper(b0, int, b0, b1, types_[0], types_[1], SINT, SINT, int, int, rows, -);
				assert(false);
			}

			case OperationTree::Expression_Operator_FLOATING_DIVIDE:
			{
				void *b0;
				void *b1;
				b0 = t_[0] -> pull(rows);
				b1 = t_[1] -> pull(rows);
				do_2_arg_oper(b0, double, b0, b1, types_[0], types_[1], SDOUBLE, SDOUBLE, double, double, rows, /);
				do_2_arg_oper(b0, double, b0, b1, types_[0], types_[1], SDOUBLE, SINT, double, int, rows, /);
				do_2_arg_oper(b1, double, b0, b1, types_[0], types_[1], SINT, SDOUBLE, int, double, rows, /);

				void *b3 = buffers_.back();

				do_2_arg_oper(b3, double, b0, b1, types_[0], types_[1], SINT, SINT, int, int, rows, /);
				assert(false);
			}

			case OperationTree::Expression_Operator_LOG:
			{
				void *b0;
				b0 = t_[0] -> pull(rows);
				do_1_arg_oper(b0, double, b0, types_[0], SDOUBLE, double, rows, log);

				void *b3 = buffers_.back();
				do_1_arg_oper(b3, double, b0, types_[0], SINT, int, rows, log);
				assert(false);
			}

			case OperationTree::Expression_Operator_NEGATE:
			{
				void *b0;
				b0 = t_[0] -> pull(rows);
				do_1_arg_oper(b0, double, b0, types_[0], SDOUBLE, double, rows, -);
				do_1_arg_oper(b0, int, b0, types_[0], SINT, int, rows, -);
				assert(false);
			}

			case OperationTree::Expression_Operator_NOT:
			{
				void *b0;
				b0 = t_[0] -> pull(rows);
				do_1_arg_oper(b0, bool, b0, types_[0], SBOOL, bool, rows, !);
				assert(false);
			}

			case OperationTree::Expression_Operator_NOT_EQUAL:
			{
				void *b0;
				void *b1;
				b0 = t_[0] -> pull(rows);
				b1 = t_[1] -> pull(rows);
				do_2_arg_comp_oper(b0, bool, b0, b1, types_[0], types_[1], SDOUBLE, SDOUBLE, double, double, rows, !=);
				do_2_arg_comp_oper(b0, bool, b0, b1, types_[0], types_[1], SDOUBLE, SINT, double, int, rows, !=);
				do_2_arg_comp_oper(b0, bool, b0, b1, types_[0], types_[1], SINT, SDOUBLE, int, double, rows, !=);
				do_2_arg_comp_oper(b0, bool, b0, b1, types_[0], types_[1], SINT, SINT, int, int, rows, !=);
				do_2_arg_comp_oper(b0, bool, b0, b1, types_[0], types_[1], SBOOL, SBOOL, bool, bool, rows, !=);
				assert(false);
			}

			case OperationTree::Expression_Operator_EQUAL:
			{
				void *b0;
				void *b1;
				b0 = t_[0] -> pull(rows);
				b1 = t_[1] -> pull(rows);
				do_2_arg_comp_oper(b0, bool, b0, b1, types_[0], types_[1], SDOUBLE, SDOUBLE, double, double, rows, ==);
				do_2_arg_comp_oper(b0, bool, b0, b1, types_[0], types_[1], SDOUBLE, SINT, double, int, rows, ==);
				do_2_arg_comp_oper(b0, bool, b0, b1, types_[0], types_[1], SINT, SDOUBLE, int, double, rows, ==);
				do_2_arg_comp_oper(b0, bool, b0, b1, types_[0], types_[1], SINT, SINT, int, int, rows, ==);
				do_2_arg_comp_oper(b0, bool, b0, b1, types_[0], types_[1], SBOOL, SBOOL, bool, bool, rows, ==);
				assert(false);
			}
			case OperationTree::Expression_Operator_OR:
			{
				void *b0;
				void *b1;
				b0 = t_[0] -> pull(rows);
				b1 = t_[1] -> pull(rows);
				do_2_arg_oper(b0, bool, b0, b1, types_[0], types_[1], SBOOL, SBOOL, bool, bool, rows, ||);
				assert(false);
			}
			case OperationTree::Expression_Operator_AND:
				{
				void *b0;
				void *b1;
				b0 = t_[0] -> pull(rows);
				b1 = t_[1] -> pull(rows);
				do_2_arg_oper(b0, bool, b0, b1, types_[0], types_[1], SBOOL, SBOOL, bool, bool, rows, &&);
				assert(false);
			}
			case OperationTree::Expression_Operator_LOWER:
			{
				void *b0;
				void *b1;
				b0 = t_[0] -> pull(rows);
				b1 = t_[1] -> pull(rows);
				do_2_arg_comp_oper(b0, bool, b0, b1, types_[0], types_[1], SDOUBLE, SDOUBLE, double, double, rows, <);
				do_2_arg_comp_oper(b0, bool, b0, b1, types_[0], types_[1], SDOUBLE, SINT, double, int, rows, <);
				do_2_arg_comp_oper(b0, bool, b0, b1, types_[0], types_[1], SINT, SDOUBLE, int, double, rows, <);
				do_2_arg_comp_oper(b0, bool, b0, b1, types_[0], types_[1], SINT, SINT, int, int, rows, <);
				assert(false);
			}
			case OperationTree::Expression_Operator_GREATER:
				{
				void *b0;
				void *b1;
				b0 = t_[0] -> pull(rows);
				b1 = t_[1] -> pull(rows);
				do_2_arg_comp_oper(b0, bool, b0, b1, types_[0], types_[1], SDOUBLE, SDOUBLE, double, double, rows, >);
				do_2_arg_comp_oper(b0, bool, b0, b1, types_[0], types_[1], SDOUBLE, SINT, double, int, rows, >);
				do_2_arg_comp_oper(b0, bool, b0, b1, types_[0], types_[1], SINT, SDOUBLE, int, double, rows, >);
				do_2_arg_comp_oper(b0, bool, b0, b1, types_[0], types_[1], SINT, SINT, int, int, rows, >);
				assert(false);
			}

			default : assert(false);
		}
}

}
