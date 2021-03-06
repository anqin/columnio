// Copyright (C) 2015, for Authors.
// Author: An Qin (anqin.qin@gmail.com)
//
// Description:

#include "twister/io/dissector.h"

DECLARE_string(twister_default_charset_encoding);

namespace twister {
namespace io {
bool RecordDissector::InitCharsetConverter(
        const std::string& charset_encoding) {
    if (charset_encoding != ""
        && charset_encoding != FLAGS_twister_default_charset_encoding) {
        m_charset_converter.reset(new CharsetConverter());
        if (!m_charset_converter->Create(
                charset_encoding, FLAGS_twister_default_charset_encoding)) {
            LOG(ERROR) << "Can not convert charset encoding : "
                << charset_encoding << " into : "
                << FLAGS_twister_default_charset_encoding;
            return false;
        }
        LOG(INFO) << "Input Record will be converted from " << charset_encoding
            << " to " << FLAGS_twister_default_charset_encoding;
    } else {
        LOG(INFO) << "Input Record will be deal with charset encoding "
            << FLAGS_twister_default_charset_encoding;
    }
    return true;
}

bool RecordDissector::Convert(const std::string& input, std::string* output) {
    if (m_charset_converter.get() == NULL) {
        return false;
    }

    if (m_charset_converter->Convert(input, output)) {
        LOG(INFO) << "convert " << input << " to  " << *output;
        return true;
    }

    LOG(ERROR) << "Convert charset error : " << input;
    return false;
}

}  // namespace io
}  // namespace twister
