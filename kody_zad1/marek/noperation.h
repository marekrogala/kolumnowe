#ifndef OPERATION_H_
#define OPERATION_H_

#include "proto/operations.pb.h"
#include "typetraits.h"
#include "server.h"

#include <cstring>
#include <ostream>

inline char* StaticCalculateValuePosition(char* base, int column_offset, int column_size, int row_id){
  return base + column_offset*BLOCK_SIZE + column_size * row_id;
}

class NOperation {
    public:
        virtual ~NOperation() {}
        static NOperation* FromProtobuf(const Operation &op);
        virtual std::string toString() const;
        virtual ColType getColumnType(int column_id) const = 0;
        virtual int getColumnCount() const = 0;
        virtual void setServer(class Server * server);
        void execute();
        virtual int fetch(int number, char* &destination) = 0;
        int getColumnUnitOffset(int column_id) const;
        int getUnitSize() const;
        char* calculateColumnPosition(char* base, int column_id) const;
        char* calculateValuePosition(char* base, int column_id, int row_id) const;
        char* allocateBlock() const;
        void copyFromBlockToBlock(char* source, int start_pos_source,
                             char* destination, int start_pos_destination, int number);
    protected:
        void calculateOffsets();
        NOperation(){}
        class Server * server_;
    private:
        std::vector<int> column_unit_offsets_;
        int unit_size_;
};

std::ostream& operator<<(std::ostream& out, const NOperation * const nop);

#endif

