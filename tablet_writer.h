// Copyright (C) 2015, for Authors.
// Author: An Qin (anqin.qin@gmail.com)
//
// Description:
// TabletWriter is responsible for writing the blocks of tablet
// with the interaction of low io modular of high control logic.

#ifndef TWISTER_IO_TABLET_WRITER_H
#define TWISTER_IO_TABLET_WRITER_H

#include <string>
#include <vector>

#include "common/base/scoped_ptr.h"
#include "common/base/stdint.h"
#include "common/base/uncopyable.h"
#include "common/file/file.h"
#include "common/system/memory/mempool.h"

#include "twister/io/column_metadata.pb.h"
#include "twister/io/tablet_schema.pb.h"

namespace twister {
class DataHolder;
namespace io {
class Block;
class ColumnWriter;

class TabletWriter {
    DECLARE_UNCOPYABLE(TabletWriter);

public:
    TabletWriter();
    virtual ~TabletWriter();

    virtual void SetBuffer(char* buffer, uint32_t len);

    // Construct the tablet schema before open.
    virtual bool BuildTabletSchema(const std::string& table_name,
                                   const SchemaDescriptor& schema_descriptor,
                                   const std::vector<ColumnStaticInfo>& info);
    virtual bool BuildTabletSchema(const std::string& table_name,
                                   const SchemaDescriptor& schema_descriptor);
    virtual bool Open(const std::string& file_name);
    virtual bool Close();
    virtual bool Write(const std::vector<const Block*>& blocks,
                       const std::vector<uint32_t>& indexes);
    void GetOutputFileList(std::vector<std::string> *file_list) const;

private:
    bool Write(const Block& block, uint32_t index);
    bool InitColumnWriters();
    void DestroyColumnWriters();
    bool Dump();
    uint32_t GetTabletSchemaSize() const;

private:
    uint32_t m_write_size;
    uint32_t m_tablet_schema_size;
    uint32_t m_tablet_buffer_size;
    std::string m_file_name;
    std::vector<std::string> m_dump_files;
    std::vector<ColumnWriter*> m_column_writers;
    scoped_ptr<TabletSchema> m_tablet_schema;
    scoped_ptr<DataHolder> m_data_holder;
};

}  // namespace io
}  // namespace twister

#endif  // TWISTER_IO_TABLET_WRITER_H
