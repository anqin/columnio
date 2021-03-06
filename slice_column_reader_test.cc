// Copyright (C) 2015, for Authors.
// Author: An Qin (anqin.qin@gmail.com)
//
// Created: 06/12/12

#include "twister/io/slice_column_reader.h"

#include <iostream>
#include <string>

#include "common/file/file.h"
#include "common/file/recordio/recordio.h"
#include "thirdparty/glog/logging.h"
#include "thirdparty/gtest/gtest.h"

#include "twister/io/slice.h"
#include "twister/io/table_builder.h"
#include "twister/io/tablet_reader.h"
#include "twister/io/test_data.h"
#include "twister/utils/proto_message.h"

namespace twister {
namespace io {

const int kRecordNumber = 1000;

class SliceColumnReaderTest : public ::testing::Test {
public:
    SliceColumnReaderTest() {}
};

TEST_F(SliceColumnReaderTest, Reading) {
    ProtoMessage proto_message;
    proto_message.CreateMessageByProtoFile("testdata/document.proto",
                                           "Document");
    TestData data;
    data.Reset(proto_message.GetMessage());
    data.GenerateMessages(kRecordNumber);

    File* file = File::Open("document.msg", File::ENUM_FILE_OPEN_MODE_W);
    RecordWriter* writer = new RecordWriter(file);
    for (int i = 0; i < data.size(); i++) {
        writer->WriteMessage(data[i]);
    }

    writer->Flush();
    delete writer;
    file->Close();
    delete file;

    TableOptions opt;
    opt.add_input_files();
    opt.mutable_input_files(0)->set_uri("document.msg");
    opt.mutable_schema_descriptor()->set_type("PB");

    opt.mutable_schema_descriptor()->set_description(
        proto_message.GetFileDescriptorSetString());

    opt.mutable_schema_descriptor()->set_record_name("Document");
    opt.set_output_file("document.col");
    opt.set_output_table("Document");

    TableBuilder builder;
    std::vector<std::string> output_files;
    builder.CreateTable(opt, &output_files);

    std::vector<Slice> slices;
    for (uint32_t i = 0; i < output_files.size(); i++) {
        MemPool mempool(MemPool::MAX_UNIT_SIZE);
        TabletReader reader(&mempool);
        EXPECT_TRUE(reader.Init(output_files[i]));
        while (reader.Next()) {
            slices.push_back(*reader.GetSlice());
        }
        reader.Close();
    }

    SliceColumnReader reader(slices);
    Block block;
    for (uint32_t i = 0; i < slices[0].GetCount(); i++) {
        while (reader.ReadColumn(i, &block)) {
        }
    }
}

} // namespace io
} // namespace twister
