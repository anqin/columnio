// Copyright (C) 2015, for Authors.
// Author: An Qin (anqin.qin@gmail.com)
//
// Created: 04/12/12
// Description:

#ifndef TWISTER_IO_PB_ASSEMBLER_H
#define TWISTER_IO_PB_ASSEMBLER_H

#include <string>
#include <vector>

#include "common/base/scoped_ptr.h"
#include "thirdparty/protobuf/descriptor.h"
#include "thirdparty/protobuf/message.h"

#include "twister/io/automaton.h"
#include "twister/io/slice_column_reader.h"
#include "twister/io/tablet_reader.h"
#include "twister/utils/proto_message.h"

namespace twister {
namespace io {

class PbRecordAssembler {
public:
    PbRecordAssembler();
    ~PbRecordAssembler();

    bool Init(TabletReader* tablet_reader);

    bool Init(SliceColumnReader* slice_column_reader,
              const SchemaDescriptor& schema_descriptor);

    bool AssembleRecord(protobuf::Message* message);

    const protobuf::Message* GetProtoMessage() const;

private:
    typedef ::std::string string;
    typedef ::protobuf::Message Message;
    typedef ::protobuf::Descriptor Descriptor;
    typedef ::protobuf::FieldDescriptor FieldDescriptor;
    typedef ::protobuf::Reflection Reflection;

    bool InitProtoMessage(const SchemaDescriptor& schema_descriptor);

    Message* MoveToLevel(Message* message,
                         const string& full_path,
                         int def_level,
                         int new_repeat);

    void AppendValue(Message* message,
                     const string& field_name,
                     const Block& block);

    bool ReadColumn(const string& field, Block* block);

    int RepLevel(const Block& block);

    int DefLevel(const Block& block);

    int NextRepLevel(const string& field);

    int MaxDefLevel(const string& field);

    string GetName(const string& field);

    string GetPath(const string& field);

private:
    TabletReader* m_tablet_reader;
    SliceColumnReader* m_slice_column_reader;
    std::vector<ColumnStaticInfo> m_column_static_info;
    scoped_ptr<ProtoMessage> m_proto_message;
    scoped_ptr<Automaton> m_automaton;
};

} // namespace io
} // namespace twister

#endif // TWISTER_IO_PB_ASSEMBLER_H
