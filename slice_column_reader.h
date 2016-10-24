// Copyright (C) 2015, for Authors.
// Author: An Qin (anqin.qin@gmail.com)
//
// Created: 06/12/12

#ifndef TWISTER_IO_SLICE_COLUMN_READER_H
#define TWISTER_IO_SLICE_COLUMN_READER_H

#include <vector>

#include "twister/io/slice.h"

namespace twister {
namespace io {

class SliceColumnReader {
public:
    explicit SliceColumnReader(const std::vector<Slice>& slices);
    ~SliceColumnReader();

    bool ReadColumn(int index, Block* block);
    int NextRepLevel(int index);

private:
    const std::vector<Slice>* m_slices;
    std::vector<uint32_t> m_cursor;
};

}  // namespace io
}  // namespace twister

#endif  // TWISTER_IO_SLICE_COLUMN_READER_H
