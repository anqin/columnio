// Copyright (C) 2015, for Authors.
// Author: An Qin (anqin.qin@gmail.com)
//
// Created: 03/20/12
// Description:

#ifndef TWISTER_IO_FIELD_PRODUCTOR_H
#define TWISTER_IO_FIELD_PRODUCTOR_H
#pragma once

#include <string>
#include <vector>

#include "common/base/scoped_ptr.h"
#include "common/base/uncopyable.h"

#include "twister/io/block.h"
#include "twister/io/column_metadata.pb.h"
#include "twister/io/table_options.pb.h"
#include "twister/io/tablet_schema.pb.h"

namespace twister {
namespace io {

class RecordDissector;
class RecordImporter;

// FieldProductor is responsible for unique entrance of columnar
// storage, and drive the data creation and save from different
// data sources.
// The output of FieldProductor is unique-formatted buffer and
// some metadata of column. This consumes the input heterogeneity.
class FieldProductor {
public:
    FieldProductor();
    FieldProductor(const ::google::protobuf::RepeatedPtrField<URI>& file_paths,
                   const SchemaDescriptor& schema_descriptor,
                   RecordImporter* importer,
                   RecordDissector* dissector);
    virtual ~FieldProductor();

    // Interface to present source data info.
    // Each file path is presented as full-path format, not accepted
    // wildcard. And, schema metadata will be back.
    virtual void Reset(const ::google::protobuf::RepeatedPtrField<URI>&
                       file_paths,
                       const SchemaDescriptor& schema_descriptor);
    virtual void SetBuffer(char *buffer, uint32_t length);

    // Interface to get schema static info.
    // The schema static info is back to construct columnar writers.
    virtual bool GetSchemaColumnStat(std::vector<ColumnStaticInfo>*
                                     column_stats);

    // Check whether exist record data from current pointer
    // in record files.
    virtual bool HasRecord();

    // Get field data of record one by one.
    // The ouput buffer of interface is dumped with formatted columner
    // data organized as <field_name, r_d_value>.
    virtual bool NextRecordFields(std::vector<const Block*>* output_blocks,
                                  std::vector<uint32_t>* indexes);

private:
    DECLARE_UNCOPYABLE(FieldProductor);

    scoped_ptr<RecordDissector> m_dissector;
    scoped_ptr<RecordImporter> m_importer;
};

} // namespace io
} // namespace twister

#endif // TWISTER_IO_FIELD_PRODUCTOR_H
