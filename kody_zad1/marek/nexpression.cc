#include <cassert>
#include <sstream>
#include<iostream>
#include "nexpression.h"
#include "ncolumnexpression.h"
#include "nconstantexpression.h"
#include "nifexpression.h"
#include "nbinaryoperatorexpression.h"
#include "nunaryoperatorexpression.h"
#include "typetraits.h"
#include "BinaryOperators.h"
#include "UnaryOperators.h"


NExpression* ReadColumnExpression(const Expression &op, const NOperation* source){
  int column_id = op.column_id();

  ColType type = source->getColumnType(column_id);

  switch(type){
    case BOOL: return new NColumnExpression<BOOL>(op, source);
    case INT: return new NColumnExpression<INT>(op, source);
    case DOUBLE: return new NColumnExpression<DOUBLE>(op, source);
  }

  assert(false);
  return 0;
}

NExpression* ReadIfExpression(const Expression &op, const NOperation* source){
  const Expression& condition = op.children(0);
  const Expression& optionTrue = op.children(1);
  const Expression& optionFalse = op.children(2);

  NExpression * conditionNode = NExpression::FromProtobuf(condition, source);
  NExpression * optionTrueNode = NExpression::FromProtobuf(optionTrue, source);
  NExpression * optionFalseNode = NExpression::FromProtobuf(optionFalse, source);

  assert(optionTrueNode->getType() == optionFalseNode->getType());
  assert(conditionNode->getType() == BOOL);

  ColType type = optionTrueNode->getType();

  switch(type){
    case BOOL: return new NIfExpression<BOOL>(conditionNode, optionTrueNode, optionFalseNode);
    case INT: return new NIfExpression<INT>(conditionNode, optionTrueNode, optionFalseNode);
    case DOUBLE: return new NIfExpression<DOUBLE>(conditionNode, optionTrueNode, optionFalseNode);
  }

  assert(false);
  return 0;
}

NExpression* ReadBinaryOperator(const Expression &op, const NOperation* source){
  const Expression& leftExpr = op.children(0);
  const Expression& rightExpr = op.children(1);


  NExpression * left = NExpression::FromProtobuf(leftExpr, source);
  NExpression * right = NExpression::FromProtobuf(rightExpr, source);

  bool bothOperandsInt = (left->getType() == INT && right->getType() == INT);


  switch(op.operator_()){
    case Expression_Operator_ADD:
      if(bothOperandsInt) { return new NBinaryOperatorExpression<INT, OpADD>(left, right); }
      else { return new NBinaryOperatorExpression<DOUBLE, OpADD>(left, right); }
    case Expression_Operator_MULTIPLY:
      if(bothOperandsInt) { return new NBinaryOperatorExpression<INT, OpMULTIPLY>(left, right); }
      else { return new NBinaryOperatorExpression<DOUBLE, OpMULTIPLY>(left, right); }
    case Expression_Operator_SUBTRACT:
      if(bothOperandsInt) { return new NBinaryOperatorExpression<INT, OpSUBTRACT>(left, right); }
      else { return new NBinaryOperatorExpression<DOUBLE, OpSUBTRACT>(left, right); }
    case Expression_Operator_FLOATING_DIVIDE:
      return new NBinaryOperatorExpression<DOUBLE, OpFLOATING_DIVIDE>(left, right);
    case Expression_Operator_LOWER:
      return new NBinaryOperatorExpression<BOOL, OpLOWER>(left, right);
    case Expression_Operator_GREATER:
      return new NBinaryOperatorExpression<BOOL, OpGREATER>(left, right);
    case Expression_Operator_EQUAL:
      return new NBinaryOperatorExpression<BOOL, OpEQUAL>(left, right);
    case Expression_Operator_NOT_EQUAL:
      return new NBinaryOperatorExpression<BOOL, OpNOT_EQUAL>(left, right);
    case Expression_Operator_AND:
      return new NBinaryOperatorExpression<BOOL, OpAND>(left, right);
    case Expression_Operator_OR:
      return new NBinaryOperatorExpression<BOOL, OpOR>(left, right);
    default:
      assert(false);
      return 0;
  }
}
NExpression* ReadUnaryOperator(const Expression &op, const NOperation* source){
  const Expression& expr = op.children(0);

  NExpression * operand = NExpression::FromProtobuf(expr, source);

  switch(op.operator_()){
    case Expression_Operator_NEGATE:
      if(operand->getType() == INT) { return new NUnaryOperatorExpression<INT, OpNEGATE>(operand); }
      else { return new NUnaryOperatorExpression<DOUBLE, OpNEGATE>(operand); }
    case Expression_Operator_LOG:
      return new NUnaryOperatorExpression<DOUBLE, OpLOG>(operand);
    case Expression_Operator_NOT:
      return new NUnaryOperatorExpression<BOOL, OpNOT>(operand);
    default:
      assert(false);
      return 0;
  }
}


NExpression* NExpression::FromProtobuf(const Expression &op, const NOperation* source){
  switch(op.operator_()){
    case Expression_Operator_COLUMN:
      return ReadColumnExpression(op, source);
    case Expression_Operator_CONSTANT:
      if(op.has_constant_int32())       return new NConstantExpression<INT>(op.constant_int32());
      else if(op.has_constant_double()) return new NConstantExpression<DOUBLE>(op.constant_double());
      else if(op.has_constant_bool())   return new NConstantExpression<BOOL>(op.constant_bool());
      break;
    case Expression_Operator_IF:
      return ReadIfExpression(op, source);
    case Expression_Operator_ADD:
    case Expression_Operator_MULTIPLY:
    case Expression_Operator_SUBTRACT:
    case Expression_Operator_FLOATING_DIVIDE:
    case Expression_Operator_LOWER:
    case Expression_Operator_GREATER:
    case Expression_Operator_EQUAL:
    case Expression_Operator_NOT_EQUAL:
    case Expression_Operator_AND:
    case Expression_Operator_OR:
      return ReadBinaryOperator(op, source);
    case Expression_Operator_LOG:
    case Expression_Operator_NEGATE:
    case Expression_Operator_NOT:
      return ReadUnaryOperator(op, source);
    default:
      break;
  }
  assert(false);
  return 0;
}

std::string NExpression::toString() const {
  std::ostringstream ss;
  ss << "NExpression";
  return ss.str();
}

std::ostream& operator<<(std::ostream& out, const NExpression * const nexp)
{
  return out << nexp->toString();
}

