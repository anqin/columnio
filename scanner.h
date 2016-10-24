// Copyright (C) 2015, for Authors.
// Author: An Qin (anqin.qin@gmail.com)
//
// Description: Scanner is used to send slices to excutor one by one. It has
// the following two derived classes:
// 1. TabletScanner: used in leafserver, get slices from TabletReader;
// 2. SliceScanner: used in interserver, get slices from child servers.

#ifndef  TWISTER_IO_SCANNER_H
#define  TWISTER_IO_SCANNER_H

namespace twister {
namespace io {

class Slice;

class Scanner {
public:
    virtual ~Scanner() {}
    virtual bool NextSlice(Slice** slice) = 0;
};

} // namespace io
} // namespace twister

#endif  // TWISTER_IO_SCANNER_H
