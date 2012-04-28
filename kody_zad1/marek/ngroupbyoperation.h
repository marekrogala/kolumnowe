#ifndef NGROUPBYOPERATION_H_
#define NGROUPBYOPERATION_H_

#include "proto/operations.pb.h"
#include "noperation.h"
#include "nexpression.h"

#include <cstring>
#include <ostream>
#include <utility>
#include <vector>

#include <tr1/unordered_map>
using namespace std::tr1;
using namespace std;

struct AggregateKey {
    long hash_value;
    char * exact_value;
    int exact_value_length;
};

typedef struct
{
  long operator() (const AggregateKey &k) const {
    return k.hash_value;
  }
} AggregateKeyHash;

typedef struct
{
  bool operator() (const AggregateKey &x, const AggregateKey &y) const {
    return memcmp(x.exact_value, y.exact_value, x.exact_value_length) == 0;
  }
} AggregateKeyEq;


class NGroupByOperation : public NOperation {
  public:
    virtual ~NGroupByOperation();
    NGroupByOperation(const GroupByOperation &op);
    virtual std::string toString() const;
    virtual ColType getColumnType(int column_id) const;
    virtual int getColumnCount() const;
    virtual void setServer(class Server * server);
    virtual int fetch(int number, char* &destination);
  private:
    typedef pair<char*,int> MAP_VALUES_T;
    NOperation * source_;
    std::vector<int> group_by_columns_;
    std::vector<std::pair<Aggregation_Type, int> > aggregations_;

    bool loaded_;
    int available_count_, used_count_;
    int used_blocks_, used_in_current_block_;

    int keys_size_, values_size_;
    unordered_map<AggregateKey, MAP_VALUES_T, AggregateKeyHash, AggregateKeyEq> hash_map_;

    std::vector<char*> blocks_;
    std::vector<char*> key_blocks_;
    int free_in_last_block_;

    void load();
    void makeKeys(AggregateKey * keys, char* data, int nrows);
    MAP_VALUES_T addEmptyAggregationSpace();
    void aggregateToExisting(MAP_VALUES_T * key, char* data, int nrows);
    void initializeAggregation(MAP_VALUES_T key, char* data, int row_id);

    template<ColType T> void AddTypedColumns(MAP_VALUES_T * key, int col, char* data, int source_column, int nrows);
    template<ColType T> void IncTypedColumns(MAP_VALUES_T * key, int col, int nrows);
    void clean();
};

#endif

