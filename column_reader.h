// Copyright (C) 2015, for Authors.
// Author: An Qin (anqin.qin@gmail.com)
//
// Description:
// ColumnReader is the interface for operating the tablet data for reading.

#ifndef TWISTER_IO_COLUMN_READER_H
#define TWISTER_IO_COLUMN_READER_H

#include <string>

#include "common/base/scoped_ptr.h"
#include "common/base/stdint.h"
#include "common/file/file.h"
#include "common/system/concurrency/atomic/atomic.h"
#include "common/system/concurrency/blocking_queue.h"
#include "common/system/memory/mempool.h"

#include "twister/io/block.h"
#include "twister/io/column_metadata.pb.h"

namespace twister {
namespace io {

// Read the column(field) block for tablet data file.
class ColumnReader {
public:
    ColumnReader(const ColumnStaticInfo& info, MemPool* mempool);
    ~ColumnReader();
    bool Open(const std::string& filename, int64_t offset, int64_t size);
    bool Close();
    bool NextBlock();
    void GetBlock(Block *bobj);
    int GetRepetitionLevel() const;
    int NextRepetitionLevel() const;
    int GetDefinitionLevel() const;
    bool GetColumnType(ColumnType *column_type) const;
    bool GetColumnName(std::string *column_name) const;
    int GetMaxDefinitionLevel() const;
    int64_t GetColumnReadBytes() const;

private:
    ErrorCode ReadBlock(char* buffer, int32_t buf_len);
    void SetBlockNull(Block *block);
    bool LoadColumnData(char* buffer, int64_t offset,
                        int64_t size, uint32_t retry_times);
    void ReadCallback(uint32_t retry_times,
                      int64_t read_size, uint32_t error_code);
    bool GetAvailableBuffLen(int32_t* buff_len);

private:
    bool m_is_empty;
    Atomic<bool> m_finish_read;
    ColumnStaticInfo m_static_info;
    char *m_buffer;
    char *m_start_buffer_ptr;
    char *m_end_buffer_ptr;
    int64_t m_load_size;
    int64_t m_remain_size;
    int64_t m_size;
    int64_t m_offset;
    MemPool* m_mempool;

    Block m_block;
    Block m_next_block;
    uint32_t m_flag;
    scoped_ptr<File> m_file;
    BlockingQueue<char*> m_queue;

    static const uint32_t kMaxRetryTimes = 3;
};
} // namepsace io
} // namespace twister

#endif  // TWISTER_IO_COLUMN_READER_H
