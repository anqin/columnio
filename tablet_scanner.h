// Copyright (C) 2015, for Authors.
// Author: An Qin (anqin.qin@gmail.com)
//
// Description:

#ifndef  TWISTER_IO_TABLET_SCANNER_H
#define  TWISTER_IO_TABLET_SCANNER_H

#include "twister/io/scanner.h"

namespace twister {
namespace io {

class TabletReader;

class TabletScanner : public Scanner {
public:
    explicit TabletScanner(TabletReader* tablet_reader);
    ~TabletScanner();
    bool NextSlice(Slice** slice);

private:
    TabletReader* m_tablet_reader;
};

} // namespace io
} // namespace twister

#endif  // TWISTER_IO_TABLET_SCANNER_H
