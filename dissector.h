// Copyright (C) 2015, for Authors.
// Author: An Qin (anqin.qin@gmail.com)
//
// Created 03/20/12
// Description:

#ifndef TWISTER_IO_DISSECTOR_H
#define TWISTER_IO_DISSECTOR_H
#pragma once

#include <string>
#include <vector>

#include "common/base/class_register.h"
#include "common/base/global_function_register.h"
#include "common/base/scoped_ptr.h"
#include "common/base/string/string_piece.h"
#include "common/encoding/charset_converter.h"

#include "twister/io/block.h"
#include "twister/io/column_metadata.pb.h"
#include "twister/io/table_options.pb.h"
#include "twister/io/tablet_schema.pb.h"

namespace twister {
namespace io {

// Abstract importer to fetch records from files.
// The importer called by FieldProductor will be instanlized to
// specific inherited importer.
class RecordImporter {
public:
    virtual ~RecordImporter() {}

    // Open record file and hold reader pointer
    virtual bool Open(const ::google::protobuf::RepeatedPtrField<URI>&
                      files) = 0;

    // Clear & close file handler
    virtual bool Close() = 0;

    // Read from file and back with record data
    virtual bool ReadRecord(StringPiece *value) = 0;

    // Ship to next record
    virtual bool NextRecord() = 0;

    // Check whether exist next record
    virtual bool HasRecord() = 0;

    virtual double GetProcessedPercentage() const {
        return 1.0;
    }

    virtual int64_t GetReadBytes() const {
        return 0;
    }
};

// Abstract dissector to consume record data, and calculate
// repetition level and definition level according to specific
// schema and given method.
class RecordDissector {
public:
    virtual ~RecordDissector() {}

    virtual bool Init(const SchemaDescriptor& schema_descriptor) = 0;
    virtual void SetBuffer(char *buffer, uint32_t length) = 0;

    // Dissect record data and back with columnar-specific data
    virtual bool DissectRecord(const StringPiece& data,
                               std::vector<const Block*>* output_blocks,
                               std::vector<uint32_t>* indexes) = 0;

    // Parse schema and back with column static info
    virtual bool GetSchemaColumnStat(std::vector<ColumnStaticInfo>*
                                    column_stats) = 0;

protected:
    // Init CharsetConverter, if source encoding is "" or default encoding,
    // we will deal it with default encoding, otherwise we will convert
    // source input into default encoding
    bool InitCharsetConverter(const std::string& charset_encoding);

    // Convert input string using m_charset_converter
    bool Convert(const std::string& input, std::string* output);

private:
    scoped_ptr<CharsetConverter> m_charset_converter;
};

} // namespace io
} // namespace twister

CLASS_REGISTER_DEFINE_REGISTRY(RecordImporter, twister::io::RecordImporter);

#define REGISTER_RECORD_IMPORTER(format, class_name)\
    CLASS_REGISTER_OBJECT_CREATOR(RecordImporter, \
                                  twister::io::RecordImporter, \
                                  #format, class_name)\

#define CREATE_RECORD_IMPORTER_OBJECT(format_as_string)\
    CLASS_REGISTER_CREATE_OBJECT(RecordImporter, format_as_string)

CLASS_REGISTER_DEFINE_REGISTRY(RecordDissector, twister::io::RecordDissector);

#define REGISTER_RECORD_DISSECTOR(format, class_name)\
    CLASS_REGISTER_OBJECT_CREATOR(RecordDissector, \
                                  twister::io::RecordDissector, \
                                  #format, class_name)\

#define CREATE_RECORD_DISSECTOR_OBJECT(format_as_string)\
    CLASS_REGISTER_CREATE_OBJECT(RecordDissector, format_as_string)

#endif // TWISTER_IO_DISSECTOR_H
