// Copyright (C) 2015, for Authors.
// Author: An Qin (anqin.qin@gmail.com)
//

#ifndef TWISTER_IO_SLICE_WRITER_H
#define TWISTER_IO_SLICE_WRITER_H

#include <string>
#include <vector>

#include "common/base/scoped_ptr.h"

#include "twister/io/tablet_schema.pb.h"

namespace twister {
namespace io {
class Slice;
class TabletWriter;

class SliceWriter {
public:
    SliceWriter();
    virtual ~SliceWriter();

    virtual void SetTabletWriter(TabletWriter *writer);

    virtual bool Open(const std::string& table_name,
                      const SchemaDescriptor& schema_descriptor,
                      const std::string& file_name_prefix);

    // Write all slices of a record.
    virtual bool Write(const std::vector<Slice>& slices);

    virtual bool Close(std::vector<std::string> *file_list);

private:
    scoped_ptr<TabletWriter> m_tablet_writer;
    char* m_buffer;
};

} // namespace io
} // namespace twister

#endif  // TWISTER_IO_SLICE_WRITER_H
