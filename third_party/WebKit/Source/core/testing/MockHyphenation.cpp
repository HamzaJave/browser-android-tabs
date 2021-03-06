// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "core/testing/MockHyphenation.h"

namespace blink {

size_t MockHyphenation::LastHyphenLocation(const StringView& text,
                                           size_t before_index) const {
  String str = text.ToString();
  if (str.EndsWith("phenation", kTextCaseASCIIInsensitive)) {
    if (before_index - (str.length() - 9) > 4)
      return 4 + (str.length() - 9);
    if (str.EndsWith("hyphenation", kTextCaseASCIIInsensitive) &&
        before_index - (str.length() - 11) > 2) {
      return 2 + (str.length() - 11);
    }
  }
  return 0;
}

}  // namespace blink
