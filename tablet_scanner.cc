// Copyright (C) 2015, for Authors.
// Author: An Qin (anqin.qin@gmail.com)
//
// Description:

#include "twister/io/tablet_scanner.h"

#include "twister/io/scanner.h"
#include "twister/io/tablet_reader.h"

namespace twister {
namespace io {

TabletScanner::TabletScanner(TabletReader* tablet_reader) {
    m_tablet_reader = tablet_reader;
}

TabletScanner::~TabletScanner() {}

bool TabletScanner::NextSlice(Slice** slice) {
    if (m_tablet_reader->Next()) {
        *slice = m_tablet_reader->GetSlice();
        return true;
    }
    return false;
}

} // namespace io
} // namespace twister
