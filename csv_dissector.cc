// Copyright (C) 2015, for Authors.
// Author: An Qin (anqin.qin@gmail.com)
//
// Description:

#include "twister/io/csv_dissector.h"

#include "common/base/string/algorithm.h"

#include "twister/common/base/data_holder.h"
#include "twister/io/block.h"

DECLARE_string(twister_csv_delim);

namespace twister {
namespace io {

REGISTER_RECORD_IMPORTER(CSV, CsvRecordImporter);
REGISTER_RECORD_DISSECTOR(CSV, CsvRecordDissector);
static const uint32_t kXFSBufSize = 4 * 1024 * 1024;

// CsvRecordImporter
CsvRecordImporter::CsvRecordImporter()
    : m_files(NULL),
      m_files_pos(0),
      m_eof(false),
      m_file(NULL),
      m_buffer_size(32 * 1024 * 1024),
      m_buffer(new char[m_buffer_size]),
      m_total_read_bytes(0) {
}

CsvRecordImporter::~CsvRecordImporter() {
    delete []m_buffer;
}

bool CsvRecordImporter::Open(const ::google::protobuf::RepeatedPtrField<URI>&
                             files) {
    m_files = &files;
    CHECK_GT(m_files->size(), 0);
    m_files_pos = 0;
    m_eof = false;

    return OpenSingleFile(m_files->Get(m_files_pos ++).uri());
}

bool CsvRecordImporter::OpenSingleFile(const std::string& file) {
    if (!Close()) {
        return false;
    }
    OpenFileOptions file_options;
    file_options.cache_buf_len = kXFSBufSize;
    file_options.localize_first = true;
    m_file.reset(File::Open(file.c_str(),
                            File::ENUM_FILE_OPEN_MODE_R, file_options));
    if (m_file.get() == NULL) {
        LOG(ERROR) << "Fail to open file " << file;
        m_eof = true;
        return false;
    }
    DCHECK(m_next_record.empty());
    return NextRecord();
}

bool CsvRecordImporter::Close() {
    if (m_file.get() != NULL) {
        m_file->Close();
        m_file.reset();
    }
    return true;
}

bool CsvRecordImporter::ReadRecord(StringPiece *value) {
    if (!m_eof) {
        *value = m_next_record;
        return true;
    }
    return false;
}

bool CsvRecordImporter::NextRecord() {
    m_next_record.clear();
    int32_t ret = m_file->ReadLine(m_buffer, m_buffer_size);
    if (ret < 0) {
        LOG(ERROR) << "Failed to read record";
        m_eof = true;
        return false;
    }

    if (ret >= m_buffer_size - 1) {
        LOG(ERROR) << "Record is too big to be read";
        return false;
    }

    if (ret == 0) {
        if (m_files_pos != m_files->size()) {
            // try to open next file & read first record in buffer
            return OpenSingleFile(m_files->Get(m_files_pos ++).uri());
        } else {
            LOG(INFO) << "No more record in files";
            m_next_record.clear();
            m_eof = true;
            return false;
        }
    }

    m_next_record.set(m_buffer, ret);
    m_total_read_bytes += ret;
    return true;
}

// CsvRecordDissector
CsvRecordDissector::CsvRecordDissector()
    : m_column_count(0) {
}

CsvRecordDissector::~CsvRecordDissector() {
    ReleaseBlocks();
}

bool CsvRecordDissector::Init(const SchemaDescriptor& schema_descriptor) {
    const std::string& message_name = schema_descriptor.record_name();
    const std::string& schema = schema_descriptor.description();
    if (!InitCharsetConverter(schema_descriptor.charset_encoding())) {
        return false;
    }

    m_proto_message.reset(new ProtoMessage());
    return m_proto_message->CreateMessageByFileDescriptorSet(schema,
                                                             message_name);
}

void CsvRecordDissector::SetBuffer(char *buffer, uint32_t length) {
    m_data_holder.reset(new DataHolder(buffer, length));
}

bool CsvRecordDissector::DissectRecord(const StringPiece& record,
                                       std::vector<const Block*>* output_blocks,
                                       std::vector<uint32_t>* indexes) {
    std::vector<std::string> block_vector;
    SplitStringKeepEmpty(record, FLAGS_twister_csv_delim, &block_vector);

    ReleaseBlocks();
    m_data_holder->Reset();
    output_blocks->clear();
    indexes->clear();

    for (size_t i = 0; i < m_column_count; ++i) {
        Block *bobj = m_block_pool.Acquire();
        m_block_vec.push_back(bobj);
        bobj->SetRepLevel(0);
        if (i < block_vector.size()) {
            bobj->SetDefLevel(1);
            bobj->SetValueType(m_column_type[i]);
            SetStringBlockValue(bobj, block_vector[i]);
        } else {
            bobj->SetDefLevel(0);
            bobj->SetValueType(Block::TYPE_NULL);
        }
        output_blocks->push_back(bobj);
        indexes->push_back(i);
    }
    return true;
}

void CsvRecordDissector::ReleaseBlocks() {
    for (size_t i = 0; i < m_block_vec.size(); ++i) {
        m_block_pool.Release(m_block_vec[i]);
    }
    m_block_vec.clear();
}

void CsvRecordDissector::SetStringBlockValue(Block* bobj,
                                            const std::string& value) {
    std::string convert;
    char* data_ptr = NULL;
    uint32_t size = 0;
    if (Convert(value, &convert)) {
        data_ptr = m_data_holder->Write(convert.c_str(), convert.length());
        size = convert.length();
    } else {
        data_ptr = m_data_holder->Write(value.c_str(), value.length());
        size = value.length();
    }
    CHECK_NOTNULL(data_ptr);
    bobj->SetValue(StringPiece(data_ptr, size));
}

bool CsvRecordDissector::GetSchemaColumnStat(std::vector<ColumnStaticInfo>*
                                             column_stats) {
    bool ret = m_proto_message->GetSchemaColumnStat(column_stats);
    m_column_count = column_stats->size();
    m_column_type.resize(m_column_count);
    for (size_t i = 0; i < m_column_count ; ++i) {
        SetColumnType((*column_stats)[i].column_type(),
                      &(m_column_type[i]));
    }

    return ret;
}

void CsvRecordDissector::SetColumnType(ColumnType ctype,
                                       Block::VALUE_TYPE* btype) {
#define SetType(Type)  \
    case Type: \
        *btype = Block::Type;\
        break;

    switch (ctype) {

        SetType(TYPE_UNDEFINED);
        SetType(TYPE_INT32);
        SetType(TYPE_SINT32);
        SetType(TYPE_FIXED32);
        SetType(TYPE_SFIXED32);
        SetType(TYPE_UINT32);
        SetType(TYPE_INT64);
        SetType(TYPE_SINT64);
        SetType(TYPE_FIXED64);
        SetType(TYPE_SFIXED64);
        SetType(TYPE_UINT64);
        SetType(TYPE_FLOAT);
        SetType(TYPE_DOUBLE);
        SetType(TYPE_BOOL);
        SetType(TYPE_STRING);
        SetType(TYPE_BYTES);

    default:
        LOG(FATAL) << "non-exist field type";
        break;
    }
}

} // namespace io
} // namespace twister
