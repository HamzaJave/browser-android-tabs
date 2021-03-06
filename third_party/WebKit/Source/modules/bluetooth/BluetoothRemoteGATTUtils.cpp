// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "modules/bluetooth/BluetoothRemoteGATTUtils.h"

namespace blink {

// static
DOMDataView* BluetoothRemoteGATTUtils::ConvertWTFVectorToDataView(
    const WTF::Vector<uint8_t>& wtf_vector) {
  static_assert(sizeof(*wtf_vector.Data()) == 1,
                "uint8_t should be a single byte");
  DOMArrayBuffer* dom_buffer =
      DOMArrayBuffer::Create(wtf_vector.Data(), wtf_vector.size());
  return DOMDataView::Create(dom_buffer, 0, wtf_vector.size());
}

}  // namespace blink
