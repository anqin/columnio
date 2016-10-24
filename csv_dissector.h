// Copyright (C) 2015, for Authors.
// Author: An Qin (anqin.qin@gmail.com)
//
// Description:

#ifndef  TWISTER_IO_CSV_DISSECTOR_H
#define  TWISTER_IO_CSV_DISSECTOR_H

#include <string>
#include <vector>

#include "common/base/object_pool.h"
#include "common/base/scoped_ptr.h"
#include "common/base/string/string_piece.h"
#include "common/encoding/charset_converter.h"
#include "common/file/file.h"

#include "twister/io/dissector.h"
#include "twister/utils/proto_message.h"

namespace twister {
class DataHolder;

namespace io {
class Block;

// RecordImporter for CSV record data.
// Interfaces for operating data are specific for CSV
// records, opening record file, reading record data from file,
// preseting read process, etc.
class CsvRecordImporter : public RecordImporter {
public:
    CsvRecordImporter();
    virtual ~CsvRecordImporter();

    // Open record file and hold reader pointer
    virtual bool Open(const ::google::protobuf::RepeatedPtrField<URI>& files);

    // Clear & close file handler
    virtual bool Close();

    // Read from file and back with record data
    virtual bool ReadRecord(StringPiece *value);

    // Ship to next record
    virtual bool NextRecord();

    // Check next record exist
    virtual bool HasRecord() {
        return !m_eof;
    }

    virtual int64_t GetReadBytes() const {
        return m_total_read_bytes;
    }

private:
    bool OpenSingleFile(const std::string& file);

private:
    const ::google::protobuf::RepeatedPtrField<URI>* m_files;
    int64_t m_files_pos;
    bool m_eof;
    scoped_ptr<common::File> m_file;

    int32_t m_buffer_size;
    char* m_buffer;
    StringPiece m_next_record;

    uint64_t m_total_read_bytes;
};

// RecordDissector for CSV record data.
// Record data is parsed with its CSV schema, and calculate
// repetition & definition level. The return of dissector is
// followed the unique format, agreed with FieldProductor.
class CsvRecordDissector : public RecordDissector {
public:
    CsvRecordDissector();
    virtual ~CsvRecordDissector();

    virtual bool Init(const SchemaDescriptor& schema_descriptor);
    virtual void SetBuffer(char *buffer, uint32_t len);

    // Dissect record data and back with columnar-specific data
    // all fields in record will be appended to output_fields
    virtual bool DissectRecord(const StringPiece& record,
                               std::vector<const Block*>* output_blocks,
                               std::vector<uint32_t>* indexes);

    // Parse schema and back with column static info
    // Try not to call this method multiple times
    // but at least once before any DissectRecord
    // all column in schema will be appended to column_stats
    virtual bool GetSchemaColumnStat(std::vector<ColumnStaticInfo>*
                                     column_stats);

    void ReleaseBlocks();

private:
    void SetStringBlockValue(Block* bobj, const std::string& value);
    void SetColumnType(ColumnType ctype, Block::VALUE_TYPE* btype);

private:
    scoped_ptr<ProtoMessage> m_proto_message;
    scoped_ptr<DataHolder> m_data_holder;
    ObjectPool<Block> m_block_pool;
    std::vector<Block*> m_block_vec;
    uint32_t m_column_count;
    std::vector<Block::VALUE_TYPE> m_column_type;
};

}  // namespace io
}  // namespace twister

#endif  // TWISTER_IO_CSV_DISSECTOR_H
