// Copyright (C) 2015, for Authors.
// Author: An Qin (anqin.qin@gmail.com)
//
// Description:
#ifndef  TWISTER_IO_BLOCK_HELPER_H
#define  TWISTER_IO_BLOCK_HELPER_H

#include "twister/common/base/data_holder.h"
#include "twister/io/block.h"

namespace twister {

class DataHolder;

namespace io {

class BlockHelper {
public:
    explicit BlockHelper(uint32_t data_holder_size);
    ~BlockHelper();

public:
    void Reset();
    bool SetBlockValue(Block* block, Block::VALUE_TYPE type,
                       const char* value, int32_t length);
private:
    char* m_buffer;
    DataHolder m_holder;
};

}  // namespace io
}  // namespace twister


#endif  // TWISTER_IO_BLOCK_HELPER_H

