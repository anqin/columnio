// Copyright (C) 2015, for Authors.
// Author: An Qin (anqin.qin@gmail.com)
//
// Description:

#ifndef  TWISTER_IO_SLICE_SCANNER_H
#define  TWISTER_IO_SLICE_SCANNER_H

#include <stdint.h>

#include "common/base/scoped_ptr.h"
#include "common/base/string/string_piece.h"

#include "twister/io/scanner.h"

namespace twister {
namespace io {
class Slice;

class SliceScanner : public Scanner {
public:
    explicit SliceScanner(const StringPiece& result);
    ~SliceScanner();
    bool NextSlice(Slice** slice);

private:
    const char* m_buffer;
    const char* m_end_buffer;
    const char* m_cur_buffer;
    scoped_ptr<Slice> m_slice;
};

} // namespace io
} // namespace twister

#endif  // TWISTER_IO_SLICE_SCANNER_H
