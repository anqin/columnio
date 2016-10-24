// Copyright (C) 2015, for Authors.
// Author: An Qin (anqin.qin@gmail.com)
//
//
#ifndef TWISTER_IO_IO_TEST_HELPER_H
#define TWISTER_IO_IO_TEST_HELPER_H

#include <string>
#include <vector>
#include "common/base/stdint.h"

#include "twister/io/testdata/document.pb.h"

namespace twister {
namespace io {

class IOTestHelper {
public:
    IOTestHelper();
    ~IOTestHelper();

    void BuildRecordIOMessageTestFile(uint32_t num = 10);
    void GetRecordIOMessageTestFileName(std::string *file_name) const;
    void BuildTabletTestFile(const std::string& table_name,
                             std::vector<std::string> *output_files);

    Document RandDocument();

private:
    Name RandName();
    Language RandLanguage();
    Links RandLinks();
    uint32_t RandNumber();

private:
    std::string m_recordio_msg_file_name;
    uint32_t m_file_count;
};

} // namespace io
} // namespace twister
#endif  // TWISTER_IO_IO_TEST_HELPER_H
