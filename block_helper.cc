// Copyright (C) 2015, for Authors.
// Author: An Qin (anqin.qin@gmail.com)
//
// Description:

#include "common/base/string/string_piece.h"

#include "twister/io/block_helper.h"

namespace twister {
namespace io {

BlockHelper::BlockHelper(uint32_t data_holder_size)
    : m_buffer(new char[data_holder_size]),
      m_holder(m_buffer, data_holder_size) {
}

BlockHelper::~BlockHelper() {
    delete[] m_buffer;
}

void BlockHelper::Reset() {
    m_holder.Reset();
}

bool BlockHelper::SetBlockValue(Block* block, Block::VALUE_TYPE type,
                                const char* value, int32_t length) {
    block->SetValueType(type);
    char* data_ptr = m_holder.Write(value, length);
    if (data_ptr == NULL) {
        return false;
    }
    block->SetValue(StringPiece(data_ptr, length));
    return true;
}

}  // namespace io
}  // namespace twister
