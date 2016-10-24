// Copyright (C) 2015, for Authors.
// Author: An Qin (anqin.qin@gmail.com)
//
//
#ifndef TWISTER_IO_HISTORY_INFO_IO_H
#define TWISTER_IO_HISTORY_INFO_IO_H

#include "common/base/scoped_ptr.h"
#include "common/base/string/string_piece.h"
#include "common/file/file.h"
#include "thirdparty/protobuf/message.h"

namespace twister {
namespace io {

class HistoryInfoWriter {
public:
    HistoryInfoWriter();
    ~HistoryInfoWriter();

    bool Reset(File *file);

    bool WriteMessage(const ::google::protobuf::Message& message);
    bool WriteRecord(const char *data, uint32_t size);
    bool WriteRecord(const StringPiece& data);

private:
    bool Write(const char *data, uint32_t size);

private:
    File *m_file;
};

class HistoryInfoReader {
public:
    HistoryInfoReader();
    ~HistoryInfoReader();

    bool Reset(File *file);
    // return 1 stands for ok, 0 for no more data, -1 for error.
    int Next();

    bool ReadMessage(::google::protobuf::Message *message);
    bool ReadRecord(const char **data, uint32_t *size);
    bool ReadRecord(StringPiece *data);

private:
    bool Read(char *data, uint32_t size);

private:
    File *m_file;
    scoped_array<char> m_buffer;
    uint32_t m_buffer_size;
    uint32_t m_data_size;
};

} // namespace io
} // namespace twister
#endif  // TWISTER_IO_HISTORY_INFO_IO_H
