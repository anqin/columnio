// Copyright (C) 2015, for Authors.
// Author: An Qin (anqin.qin@gmail.com)
//
// Description:
// ColumnWriter is the Interface for operating the Tablet data format writing.

#ifndef TWISTER_IO_COLUMN_WRITER_H
#define TWISTER_IO_COLUMN_WRITER_H

#include <deque>
#include <string>

#include "common/base/stdint.h"
#include "common/base/string/string_piece.h"
#include "common/base/uncopyable.h"
#include "common/file/file.h"

#include "twister/io/column_metadata.pb.h"

namespace twister {
namespace io {

// Write the column block data into tablet data file.
class ColumnWriter {
    DECLARE_UNCOPYABLE(ColumnWriter);

public:
    explicit ColumnWriter(ColumnDynamicInfo* column_info);
    ~ColumnWriter();

    void Write(const char *block_buf, uint32_t buf_len);
    bool Flush(File *file);

private:
    bool WriteFile(File *file, const char *buffer, uint32_t len);

private:
    ColumnDynamicInfo* m_column_info;
    std::deque<StringPiece> m_data_blocks;
};

}  // namespace io
}  // namespace twister

#endif  // TWISTER_IO_COLUMN_WRITER_H
