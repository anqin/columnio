// Copyright (C) 2015, for Authors.
// Author: An Qin (anqin.qin@gmail.com)
//

#include <vector>
#include "common/file/file.h"
#include "common/file/recordio/recordio.h"
#include "thirdparty/glog/logging.h"
#include "thirdparty/gtest/gtest.h"


#include "twister/io/io_test_helper.h"
#include "twister/io/short_display_reader.h"
#include "twister/io/slice.h"
#include "twister/io/table_builder.h"
#include "twister/io/tablet_reader.h"
#include "twister/io/tablet_schema.pb.h"
#include "twister/io/testdata/document.pb.h"
#include "twister/utils/proto_message.h"

namespace twister {
namespace io {

class ShortDisplayReaderTest : public ::testing::Test {
protected:
    virtual void SetUp() {
        IOTestHelper helper;
        helper.BuildTabletTestFile("Document", &m_output);

        m_reader = new ShortDisplayReader();
    }
    virtual void TearDown() {
        delete m_reader;
    }

protected:
    ShortDisplayReader *m_reader;
    std::vector<std::string> m_output;
};

TEST_F(ShortDisplayReaderTest, Read) {
    EXPECT_TRUE(m_reader->Init("Document", m_output[0]));

    TabletSchema ts;
    m_reader->GetTabletSchema(&ts);

    EXPECT_EQ("Document", ts.table_name());
    EXPECT_EQ("PB", ts.schema_descriptor().type());

    ProtoMessage pm;
    pm.CreateMessageByProtoFile("./testdata/document.proto", "Document");
    EXPECT_EQ(pm.GetFileDescriptorSetString(),
              ts.schema_descriptor().description());

    EXPECT_EQ("Document", ts.schema_descriptor().record_name());
    // 6 stands for column number in document proto message.
    EXPECT_EQ(6, ts.column_metadatas_size());

    while (m_reader->Next()) {
        VLOG(30) << "======== NEW RECORD ========";
        Slice* slice = m_reader->GetSlice();

        LOG(ERROR) << slice->DebugString();
        std::vector<uint32_t> field_count;
        m_reader->GetFieldCount(&field_count);
        std::vector<std::string> field_name;
        m_reader->GetFieldName(&field_name);

        EXPECT_EQ(field_name.size(), slice->GetCount());
        EXPECT_EQ(field_count.size(), slice->GetCount());

        for (uint32_t i = 0; i < slice->GetCount(); ++i) {
            const Block* block = slice->GetBlock(i);

            VLOG(30) << "name: " << field_name[i];
            VLOG(30) << "content: " << block->DebugString();
            VLOG(30) << "repeated num: " << field_count[i];
        }
    }
    EXPECT_TRUE(m_reader->Close());
}

}  // namespace io
}  // namespace twister
