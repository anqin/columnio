// Copyright (C) 2015, for Authors.
// Author: An Qin (anqin.qin@gmail.com)
//
// Created 03/21/12

#ifndef TWISTER_IO_TABLE_BUILDER_H
#define TWISTER_IO_TABLE_BUILDER_H

#include <string>
#include <vector>

#include "common/base/scoped_ptr.h"
#include "common/base/uncopyable.h"

#include "twister/io/table_options.pb.h"

namespace twister {
namespace io {

class TabletWriter;
class FieldProductor;

// TableBuilder focus on converting non-columnar data to one logical table
// which is stored as a number of tablets
// users should provide certain input and output information by Reset and the
// converting will be done by calling CreatTable
class TableBuilder {
public:
    TableBuilder();

    TableBuilder(TabletWriter* tablet_writer,
                 FieldProductor* field_productor);

    ~TableBuilder();

    // generate output file on columnar storage
    // using input data which specified by options
    bool CreateTable(const TableOptions& options);

    bool CreateTable(const TableOptions& options,
                     std::vector<std::string>* output_files);

private:
    // construct schema descriptor and open writer
    bool InitFileSetting(const TableOptions& options);

    // get fields of each record and write them to tablet one by one
    bool ProcessRecords();

private:
    DECLARE_UNCOPYABLE(TableBuilder);

    scoped_ptr<TabletWriter> m_tablet_writer;
    scoped_ptr<FieldProductor> m_field_productor;
    scoped_array<char> m_buffer;
};

}  // namespace io
}  // namespace twister

#endif  // TWISTER_IO_TABLE_BUILDER_H
