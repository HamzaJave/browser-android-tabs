/*
 * Copyright (C) 2000 Lars Knoll (knoll@kde.org)
 *           (C) 2000 Antti Koivisto (koivisto@kde.org)
 *           (C) 2000 Dirk Mueller (mueller@kde.org)
 * Copyright (C) 2003, 2005, 2006, 2007, 2008 Apple Inc. All rights reserved.
 * Copyright (C) 2006 Graham Dennis (graham.dennis@gmail.com)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 */

#ifndef StyleRareInheritedData_h
#define StyleRareInheritedData_h

#include "core/CoreExport.h"
#include "core/css/StyleAutoColor.h"
#include "core/css/StyleColor.h"
#include "core/style/TextSizeAdjust.h"
#include "platform/Length.h"
#include "platform/graphics/Color.h"
#include "platform/heap/Handle.h"
#include "platform/text/TabSize.h"
#include "platform/wtf/PassRefPtr.h"
#include "platform/wtf/RefCounted.h"
#include "platform/wtf/RefVector.h"
#include "platform/wtf/text/AtomicString.h"

namespace blink {

class AppliedTextDecoration;
class CursorData;
class QuotesData;
class ShadowList;
class StyleImage;
class StyleInheritedVariables;

typedef RefVector<AppliedTextDecoration> AppliedTextDecorationList;
typedef HeapVector<CursorData> CursorList;

// This struct is for rarely used inherited CSS3, CSS2, and WebKit-specific
// properties.  By grouping them together, we save space, and only allocate this
// object when someone actually uses one of these properties.
// TODO(sashab): Move this into a private class on ComputedStyle, and remove
// all methods on it, merging them into copy/creation methods on ComputedStyle
// instead. Keep the allocation logic, only allocating a new object if needed.
class CORE_EXPORT StyleRareInheritedData
    : public RefCounted<StyleRareInheritedData> {
 public:
  static PassRefPtr<StyleRareInheritedData> Create() {
    return AdoptRef(new StyleRareInheritedData);
  }
  PassRefPtr<StyleRareInheritedData> Copy() const {
    return AdoptRef(new StyleRareInheritedData(*this));
  }
  ~StyleRareInheritedData();

  bool operator==(const StyleRareInheritedData&) const;
  bool operator!=(const StyleRareInheritedData& o) const {
    return !(*this == o);
  }
  bool ShadowDataEquivalent(const StyleRareInheritedData&) const;
  bool QuotesDataEquivalent(const StyleRareInheritedData&) const;

  Persistent<StyleImage> list_style_image;

  StyleColor TextStrokeColor() const {
    return text_stroke_color_is_current_color_ ? StyleColor::CurrentColor()
                                               : StyleColor(text_stroke_color_);
  }
  StyleColor TextFillColor() const {
    return text_fill_color_is_current_color_ ? StyleColor::CurrentColor()
                                             : StyleColor(text_fill_color_);
  }
  StyleColor TextEmphasisColor() const {
    return text_emphasis_color_is_current_color_
               ? StyleColor::CurrentColor()
               : StyleColor(text_emphasis_color_);
  }
  StyleAutoColor CaretColor() const {
    if (caret_color_is_current_color_)
      return StyleAutoColor::CurrentColor();
    if (caret_color_is_auto_)
      return StyleAutoColor::AutoColor();
    return StyleAutoColor(caret_color_);
  }
  StyleColor VisitedLinkTextStrokeColor() const {
    return visited_link_text_stroke_color_is_current_color_
               ? StyleColor::CurrentColor()
               : StyleColor(visited_link_text_stroke_color_);
  }
  StyleColor VisitedLinkTextFillColor() const {
    return visited_link_text_fill_color_is_current_color_
               ? StyleColor::CurrentColor()
               : StyleColor(visited_link_text_fill_color_);
  }
  StyleColor VisitedLinkTextEmphasisColor() const {
    return visited_link_text_emphasis_color_is_current_color_
               ? StyleColor::CurrentColor()
               : StyleColor(visited_link_text_emphasis_color_);
  }
  StyleAutoColor VisitedLinkCaretColor() const {
    if (visited_link_caret_color_is_current_color_)
      return StyleAutoColor::CurrentColor();
    if (visited_link_caret_color_is_auto_)
      return StyleAutoColor::AutoColor();
    return StyleAutoColor(visited_link_caret_color_);
  }

  void SetTextStrokeColor(const StyleColor& color) {
    text_stroke_color_ = color.Resolve(Color());
    text_stroke_color_is_current_color_ = color.IsCurrentColor();
  }
  void SetTextFillColor(const StyleColor& color) {
    text_fill_color_ = color.Resolve(Color());
    text_fill_color_is_current_color_ = color.IsCurrentColor();
  }
  void SetTextEmphasisColor(const StyleColor& color) {
    text_emphasis_color_ = color.Resolve(Color());
    text_emphasis_color_is_current_color_ = color.IsCurrentColor();
  }
  void SetCaretColor(const StyleAutoColor& color) {
    caret_color_ = color.Resolve(Color());
    caret_color_is_current_color_ = color.IsCurrentColor();
    caret_color_is_auto_ = color.IsAutoColor();
  }
  void SetVisitedLinkTextStrokeColor(const StyleColor& color) {
    visited_link_text_stroke_color_ = color.Resolve(Color());
    visited_link_text_stroke_color_is_current_color_ = color.IsCurrentColor();
  }
  void SetVisitedLinkTextFillColor(const StyleColor& color) {
    visited_link_text_fill_color_ = color.Resolve(Color());
    visited_link_text_fill_color_is_current_color_ = color.IsCurrentColor();
  }
  void SetVisitedLinkTextEmphasisColor(const StyleColor& color) {
    visited_link_text_emphasis_color_ = color.Resolve(Color());
    visited_link_text_emphasis_color_is_current_color_ = color.IsCurrentColor();
  }
  void SetVisitedLinkCaretColor(const StyleAutoColor& color) {
    visited_link_caret_color_ = color.Resolve(Color());
    visited_link_caret_color_is_current_color_ = color.IsCurrentColor();
    visited_link_caret_color_is_auto_ = color.IsAutoColor();
  }

  Color text_stroke_color_;
  float text_stroke_width;
  Color text_fill_color_;
  Color text_emphasis_color_;
  Color caret_color_;

  Color visited_link_text_stroke_color_;
  Color visited_link_text_fill_color_;
  Color visited_link_text_emphasis_color_;
  Color visited_link_caret_color_;

  RefPtr<ShadowList>
      text_shadow;  // Our text shadow information for shadowed text drawing.
  AtomicString
      highlight;  // Apple-specific extension for custom highlight rendering.

  Persistent<CursorList> cursor_data;

  Length indent;
  float effective_zoom_;

  // Paged media properties.
  short widows;
  short orphans;

  unsigned text_stroke_color_is_current_color_ : 1;
  unsigned text_fill_color_is_current_color_ : 1;
  unsigned text_emphasis_color_is_current_color_ : 1;
  unsigned caret_color_is_current_color_ : 1;
  unsigned caret_color_is_auto_ : 1;
  unsigned visited_link_text_stroke_color_is_current_color_ : 1;
  unsigned visited_link_text_fill_color_is_current_color_ : 1;
  unsigned visited_link_text_emphasis_color_is_current_color_ : 1;
  unsigned visited_link_caret_color_is_current_color_ : 1;
  unsigned visited_link_caret_color_is_auto_ : 1;

  unsigned text_security : 2;         // ETextSecurity
  unsigned user_modify : 2;           // EUserModify (editing)
  unsigned word_break : 2;            // EWordBreak
  unsigned overflow_wrap : 1;         // EOverflowWrap
  unsigned line_break : 3;            // LineBreak
  unsigned user_select : 2;           // EUserSelect
  unsigned speak : 3;                 // ESpeak
  unsigned hyphens : 2;               // Hyphens
  unsigned text_emphasis_fill : 1;    // TextEmphasisFill
  unsigned text_emphasis_mark : 3;    // TextEmphasisMark
  unsigned text_emphasis_position : 1;  // TextEmphasisPosition
  unsigned text_align_last_ : 3;        // TextAlignLast
  unsigned text_justify_ : 2;           // TextJustify
  unsigned text_orientation_ : 2;       // TextOrientation
  unsigned text_combine_ : 1;           // CSS3 text-combine-upright properties
  unsigned text_indent_line_ : 1;       // TextIndentEachLine
  unsigned text_indent_type_ : 1;       // TextIndentHanging
  // CSS Image Values Level 3
  unsigned image_rendering_ : 3;          // EImageRendering
  unsigned text_underline_position_ : 1;  // TextUnderlinePosition
  unsigned text_decoration_skip_ : 3;     // TextDecorationSkip
  unsigned ruby_position_ : 1;            // RubyPosition

  // Though will-change is not itself an inherited property, the intent
  // expressed by 'will-change: contents' includes descendants.
  unsigned subtree_will_change_contents_ : 1;

  unsigned self_or_ancestor_has_dir_auto_attribute_ : 1;

  unsigned respect_image_orientation_ : 1;

  AtomicString hyphenation_string;
  short hyphenation_limit_before;
  short hyphenation_limit_after;
  short hyphenation_limit_lines;

  uint8_t line_height_step_;

  AtomicString text_emphasis_custom_mark;
  RefPtr<QuotesData> quotes;

  Color tap_highlight_color;

  RefPtr<AppliedTextDecorationList> applied_text_decorations;
  TabSize tab_size_;

  RefPtr<StyleInheritedVariables> variables;
  TextSizeAdjust text_size_adjust_;

 private:
  StyleRareInheritedData();
  StyleRareInheritedData(const StyleRareInheritedData&);
};

}  // namespace blink

#endif  // StyleRareInheritedData_h
