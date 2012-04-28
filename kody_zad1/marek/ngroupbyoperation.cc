#include <cassert>
#include <sstream>
#include <iostream>
#include "ngroupbyoperation.h"
#include "noperation.h"
#include "nexpression.h"

NGroupByOperation::NGroupByOperation(const GroupByOperation &op) :
    loaded_(false)
{
  this->source_ = NOperation::FromProtobuf(op.source());

  this->keys_size_ = 0;

  for (int i = 0, size = op.group_by_column().size(); i < size; ++i)
  {
    int column_id = op.group_by_column(i);
    this->group_by_columns_.push_back(column_id);
    this->keys_size_ += SizeOfType[this->source_->getColumnType(column_id)];
  }
  for (int i = 0, size = op.aggregations().size(); i < size; ++i)
  {
    const Aggregation& aggregation = op.aggregations(i);
    int column = aggregation.has_aggregated_column() ? aggregation.aggregated_column() : -1;
    assert(column != -1 || aggregation.type() == Aggregation_Type_COUNT);
    this->aggregations_.push_back(std::make_pair(aggregation.type(), column));
  }
  this->calculateOffsets();
  this->values_size_ = this->getUnitSize() - this->keys_size_;

  this->free_in_last_block_ = 0;
}

std::string NGroupByOperation::toString() const
{
  std::ostringstream ss;
  ss << "NGroupByOperation( source=";
  ss << this->source_;
  ss << ", group_by_columns=(";
  for (int i = 0, size = this->group_by_columns_.size(); i < size; ++i)
  {
    ss << " " << this->group_by_columns_[i];
  }
  ss << ") ";
  ss << ", aggregations=(";
  for (int i = 0, size = this->aggregations_.size(); i < size; ++i)
  {
    ss << " " << this->aggregations_[i].first << ":" << this->aggregations_[i] .second;
  }
  ss << ") ";
  ss << " )";
  return ss.str();
}

NGroupByOperation::~NGroupByOperation()
{
  delete this->source_;
}



ColType NGroupByOperation::getColumnType(int column_id) const
{
  if (column_id < (int) this->group_by_columns_.size())
  {
    return this->source_->getColumnType(this->group_by_columns_[column_id]);
  }
  else
  {
    const std::pair<Aggregation_Type, int> & aggregation = this->aggregations_[column_id
        - (int) this->group_by_columns_.size()];
    if (aggregation.first == Aggregation_Type_COUNT)
    {
      return INT;
    }
    else
    {
      return this->source_->getColumnType(aggregation.second);
    }
  }
}

int NGroupByOperation::getColumnCount() const
{
  return this->group_by_columns_.size() + this->aggregations_.size();
}

void NGroupByOperation::setServer(class Server * server)
{
  NOperation::setServer(server);
  this->source_->setServer(server);
}

void NGroupByOperation::makeKeys(AggregateKey * keys, char* data, int nrows){
  char * key_block = (char*)malloc(keys_size_*nrows);
  key_blocks_.push_back(key_block);

  for(int row = 0; row < nrows; row++){
    keys[row].exact_value_length = keys_size_;
  }

  for(int row = 0; row < nrows; row++){
    keys[row].exact_value = key_block+row*keys_size_;
  }

  for(int row = 0; row < nrows; row++){
    keys[row].hash_value = 0;
  }

  for(int i = 0, size = group_by_columns_.size(); i < size; ++i){
    int column_id = group_by_columns_[i];
    int var_size = SizeOfType[source_->getColumnType(column_id)];
    char* source_pos = (char*)source_->calculateValuePosition(data, column_id, 0);
    char* dest_pos = key_block + getColumnUnitOffset(i);
    for(int row = 0; row < nrows; row++){
      memcpy(dest_pos, source_pos, var_size);
      source_pos += var_size;
      dest_pos += keys_size_;
    }
  }

  char * ptr = key_block;
  for(int row = 0; row < nrows; row++){
    for(int i = 0; i < keys_size_; ++i){
      keys[row].hash_value = keys[row].hash_value*37 + *(ptr++);
    }
  }
}

NGroupByOperation::MAP_VALUES_T NGroupByOperation::addEmptyAggregationSpace(){
  if(free_in_last_block_ == 0){
    char* block = allocateBlock();
    blocks_.push_back(block);
    memset(block, 0, getUnitSize()*BLOCK_SIZE);

    free_in_last_block_ = BLOCK_SIZE;
  }
  free_in_last_block_--;

  int position = BLOCK_SIZE - free_in_last_block_- 1;
  return make_pair(blocks_[blocks_.size() - 1], position);
}

template<ColType T>
void AddTyped(char* place, char* from){
  typename TypeTraits<T>::T * placex = (typename TypeTraits<T>::T *)(place);
  typename TypeTraits<T>::T * fromx = (typename TypeTraits<T>::T *)(from);
//  cerr << "ADDING " << *placex << " += " << *fromx<<endl;
  (*placex) += (*fromx);
}

template<ColType T>
void NGroupByOperation::AddTypedColumns(MAP_VALUES_T * key, int col, char* data, int source_column, int nrows){
  int column_offset = getColumnUnitOffset(col);
  int source_column_offset = source_->getColumnUnitOffset(source_column);
  int column_size = SizeOfType[getColumnType(col)];

  for(int row_id = 0; row_id < nrows; ++row_id){
    char* block = key[row_id].first;
    int position = key[row_id].second;
    AddTyped<T>(StaticCalculateValuePosition(block, column_offset, column_size, position),
        StaticCalculateValuePosition(data, source_column_offset, column_size, row_id));
  }
}

template<ColType T>
void IncTyped(char* place){
  typename TypeTraits<T>::T * placex = (typename TypeTraits<T>::T *)(place);
  (*placex)++;
}

template<ColType T>
void NGroupByOperation::IncTypedColumns(MAP_VALUES_T * key, int col, int nrows){
  int column_offset = getColumnUnitOffset(col);
  int column_size = SizeOfType[getColumnType(col)];

  for(int row_id = 0; row_id < nrows; ++row_id){
    char* block = key[row_id].first;
    int position = key[row_id].second;
    IncTyped<T>(StaticCalculateValuePosition(block, column_offset, column_size, position));
  }
}

void NGroupByOperation::aggregateToExisting(MAP_VALUES_T * key, char* data, int nrows){
  for(int i = 0, size = aggregations_.size(); i < size; ++i){
//    cerr << "AGGREGATION"<<endl;
    int col = i + group_by_columns_.size();
    if(aggregations_[i].first == Aggregation_Type_COUNT){
      IncTypedColumns<INT>(key, col, nrows);
    } else {
      int source_column = aggregations_[i].second;
      switch(source_->getColumnType(source_column)){
        case INT:
          AddTypedColumns<INT>(key, col, data, source_column, nrows);
          break;
        case BOOL:
          AddTypedColumns<BOOL>(key, col, data, source_column, nrows);
          break;
        case DOUBLE:
          AddTypedColumns<DOUBLE>(key, col, data, source_column, nrows);
          break;
        default:
          assert(false);
      }
    }
  }
}

void NGroupByOperation::initializeAggregation(MAP_VALUES_T key, char* data, int row_id)
{
  char* block = key.first;
  int position = key.second;

  for(int i = 0, size = group_by_columns_.size(); i < size; ++i){
    int column_id = group_by_columns_[i];
    int var_size = SizeOfType[getColumnType(column_id)];
    char* source_pos = source_->calculateValuePosition(data, column_id, row_id);
    char* dest_pos = calculateValuePosition(block, i, position);
    memcpy(dest_pos, source_pos, var_size);
  }

}

void NGroupByOperation::load()
{
  int fetched = 0;
  char* data;
  AggregateKey key[BLOCK_SIZE];
  MAP_VALUES_T space[BLOCK_SIZE];

  available_count_ = 0;

  while((fetched = source_->fetch(BLOCK_SIZE, data))){

    makeKeys(key, data, fetched);

    for(int i = 0; i < fetched; ++i){
      if(hash_map_.count(key[i])){
        space[i] = hash_map_[key[i]];
      } else {
        space[i] = addEmptyAggregationSpace();
        initializeAggregation(space[i], data, i);
        hash_map_[key[i]] = space[i];

        available_count_++;
      }
    }

    aggregateToExisting(space, data, fetched);

    free(data);
  }

  clean();

  loaded_ = true;
  used_count_ = 0;
  used_blocks_ = 0;
  used_in_current_block_ = 0;
}

void NGroupByOperation::clean(){
  for(int i = 0, size = key_blocks_.size(); i < size; i++){
    free(key_blocks_[i]);
  }
  hash_map_.clear();
}

int NGroupByOperation::fetch(int number, char* &destination)
{
//  cerr << "fetch("<<number<<")"<<endl;
  if (!this->loaded_)
  {
    load();
  }
//  cerr << "avail " << available_count_ << " used " << used_count_<<endl;
  if(available_count_ == used_count_){
//    cerr << "exiting"<<endl;
    return 0;
  }
//  cerr << "passed"<<endl;

  int given = 0;
  destination = allocateBlock();

  while(number > 0 && available_count_ > used_count_){
    int toCopy = std::min(std::min(number, available_count_ - used_count_),
                      BLOCK_SIZE - used_in_current_block_);
//    cerr << "trying to copy " << toCopy << ": " << available_count_ - used_count_ << " " <<  BLOCK_SIZE - used_in_current_block_<<endl;
    copyFromBlockToBlock(blocks_[used_blocks_], used_in_current_block_,
                         destination, given, toCopy);
//    cerr << "Giving " << toCopy << " records"<<endl;
    given += toCopy;
    used_in_current_block_ += toCopy;
    used_count_ += toCopy;
    number -= toCopy;
    if(used_in_current_block_ == BLOCK_SIZE) {
      free(blocks_[used_blocks_]);
      used_in_current_block_ = 0;
      used_blocks_++;
    }
  }

//  cerr << " --> " << given << ""<<endl;
  return given;
}
