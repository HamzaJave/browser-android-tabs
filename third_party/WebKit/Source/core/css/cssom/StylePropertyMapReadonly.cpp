// Copyright 2016 the chromium authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "core/css/cssom/StylePropertyMapReadonly.h"

#include "bindings/core/v8/ExceptionState.h"
#include "core/css/CSSValueList.h"
#include "core/css/cssom/CSSSimpleLength.h"
#include "core/css/cssom/CSSStyleValue.h"
#include "core/css/cssom/StyleValueFactory.h"

namespace blink {

namespace {

class StylePropertyMapIterationSource final
    : public PairIterable<String, CSSStyleValueOrCSSStyleValueSequence>::
          IterationSource {
 public:
  explicit StylePropertyMapIterationSource(
      HeapVector<StylePropertyMapReadonly::StylePropertyMapEntry> values)
      : index_(0), values_(values) {}

  bool Next(ScriptState*,
            String& key,
            CSSStyleValueOrCSSStyleValueSequence& value,
            ExceptionState&) override {
    if (index_ >= values_.size())
      return false;

    const StylePropertyMapReadonly::StylePropertyMapEntry& pair =
        values_.at(index_++);
    key = pair.first;
    value = pair.second;
    return true;
  }

  DEFINE_INLINE_VIRTUAL_TRACE() {
    visitor->Trace(values_);
    PairIterable<String, CSSStyleValueOrCSSStyleValueSequence>::
        IterationSource::Trace(visitor);
  }

 private:
  size_t index_;
  const HeapVector<StylePropertyMapReadonly::StylePropertyMapEntry> values_;
};

}  // namespace

CSSStyleValue* StylePropertyMapReadonly::get(const String& property_name,
                                             ExceptionState& exception_state) {
  CSSPropertyID property_id = cssPropertyID(property_name);
  if (property_id == CSSPropertyInvalid || property_id == CSSPropertyVariable) {
    // TODO(meade): Handle custom properties here.
    exception_state.ThrowTypeError("Invalid propertyName: " + property_name);
    return nullptr;
  }

  CSSStyleValueVector style_vector = GetAllInternal(property_id);
  if (style_vector.IsEmpty())
    return nullptr;

  return style_vector[0];
}

CSSStyleValueVector StylePropertyMapReadonly::getAll(
    const String& property_name,
    ExceptionState& exception_state) {
  CSSPropertyID property_id = cssPropertyID(property_name);
  if (property_id != CSSPropertyInvalid && property_id != CSSPropertyVariable)
    return GetAllInternal(property_id);

  // TODO(meade): Handle custom properties here.
  exception_state.ThrowTypeError("Invalid propertyName: " + property_name);
  return CSSStyleValueVector();
}

bool StylePropertyMapReadonly::has(const String& property_name,
                                   ExceptionState& exception_state) {
  CSSPropertyID property_id = cssPropertyID(property_name);
  if (property_id != CSSPropertyInvalid && property_id != CSSPropertyVariable)
    return !GetAllInternal(property_id).IsEmpty();

  // TODO(meade): Handle custom properties here.
  exception_state.ThrowTypeError("Invalid propertyName: " + property_name);
  return false;
}

StylePropertyMapReadonly::IterationSource*
StylePropertyMapReadonly::StartIteration(ScriptState*, ExceptionState&) {
  return new StylePropertyMapIterationSource(GetIterationEntries());
}

}  // namespace blink
