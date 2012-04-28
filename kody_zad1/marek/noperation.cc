#include <cassert>
#include <iostream>
#include "noperation.h"
#include "nscanoperation.h"
#include "nfilteroperation.h"
#include "ncomputeoperation.h"
#include "ngroupbyoperation.h"

NOperation* NOperation::FromProtobuf(const Operation &op){
  if(op.has_scan()){
    return new NScanOperation(op.scan());
  }
  else if(op.has_compute()){
    return new NComputeOperation(op.compute());
  }
  else if(op.has_filter()){
    return new NFilterOperation(op.filter());
  }
  else if(op.has_group_by()){
    return new NGroupByOperation(op.group_by());
  }

  assert(false);

  return 0;
}

std::string NOperation::toString() const
{
  return "Unknown operation";
}

char* NOperation::calculateColumnPosition(char* base, int column_id) const {
  return base + getColumnUnitOffset(column_id)*BLOCK_SIZE;
}

char* NOperation::calculateValuePosition(char* base, int column_id, int row_id) const {
  return StaticCalculateValuePosition(base, getColumnUnitOffset(column_id), SizeOfType[getColumnType(column_id)], row_id);
}

char* NOperation::allocateBlock() const {
  return (char*)malloc(this->getUnitSize() * BLOCK_SIZE);
}

void NOperation::execute()
{
  int fetched;
  char* result;
  while((fetched = this->fetch(BLOCK_SIZE, result))){
    for(int column_id = 0, size = this->getColumnCount(); column_id < size; ++column_id){
      char* ptr = this->calculateColumnPosition(result, column_id);
      switch(this->getColumnType(column_id)){
        case BOOL:
          this->server_->ConsumeByteBools(column_id, fetched, (TypeTraits<BOOL>::T *)ptr);
          break;
        case INT:
          this->server_->ConsumeInts(column_id, fetched, (TypeTraits<INT>::T*)ptr);
          break;
        case DOUBLE:
          this->server_->ConsumeDoubles(column_id, fetched, (TypeTraits<DOUBLE>::T*)ptr);
          break;
        default:
          break;
      }
    }
    free(result);
  }
}

void NOperation::setServer(class Server * server)
{
  this->server_ = server;
}

int NOperation::getColumnUnitOffset(int column_id) const {
  return this->column_unit_offsets_[column_id];
}

int NOperation::getUnitSize() const {
  return this->unit_size_;
}

void NOperation::calculateOffsets() {
  int offset = 0;
  for(int i = 0, size = this->getColumnCount(); i < size; ++i){
      this->column_unit_offsets_.push_back(offset);
      offset += SizeOfType[this->getColumnType(i)];
  }
  this->unit_size_ = offset;
}


void NOperation::copyFromBlockToBlock(char* source, int start_pos_source,
                     char* destination, int start_pos_destination, int number){
  for(int i = 0, size = this->getColumnCount(); i < size; ++i){
      int var_size = SizeOfType[getColumnType(i)];
      char* source_pos = calculateValuePosition(source, i, start_pos_source);
      char* dest_pos = calculateValuePosition(destination, i, start_pos_destination);
      memcpy(dest_pos, source_pos, var_size * number);
  }
}

std::ostream& operator<<(std::ostream& out, const NOperation * const nop)
{
  return out << nop->toString();
}
