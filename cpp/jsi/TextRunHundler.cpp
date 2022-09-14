//
//  TextRunHundler.cpp
//  qlik-oss-react-native-helium
//
//  Created by Vittorio Cellucci on 2022-07-12.
//

#include "TextRunHundler.hpp"
#include "include/core/SkFont.h"
#include "include/core/SkFontMetrics.h"
#include "include/core/SkFontMgr.h"
#include "include/core/SkFontStyle.h"
#include "include/core/SkString.h"
#include "include/core/SkTypeface.h"

TextRunHandler::TextRunHandler(const char* utf8Text, SkPoint offset)
: fUtf8Text(utf8Text)
, fOffset(offset)
{
  fCurrentPosition = fOffset;
}

void TextRunHandler::reset() {
  fOffset = {0, 0};
//  fBuilder.
  beginLine();
}

void TextRunHandler::beginLine() {
  fCurrentPosition = fOffset;
  fMaxRunAscent = 0;
  fMaxRunDescent = 0;
  fMaxRunLeading = 0;
}
void TextRunHandler::runInfo(const RunInfo& info) {
  SkFontMetrics metrics;
  info.fFont.getMetrics(&metrics);
  fMaxRunAscent = std::min(fMaxRunAscent, metrics.fAscent);
  fMaxRunDescent = std::max(fMaxRunDescent, metrics.fDescent);
  fMaxRunLeading = std::max(fMaxRunLeading, metrics.fLeading);
}

void TextRunHandler::advance(const char* p) {
  fUtf8Text = p;
}

void TextRunHandler::commitRunInfo() {
//  fCurrentPosition.fY -= fMaxRunAscent;
}

SkShaper::RunHandler::Buffer TextRunHandler::runBuffer(const RunInfo& info) {
  auto glyphCount = SkTFitsIn<int>(info.glyphCount) ? info.glyphCount : INT_MAX;
  auto utf8RangeSize = SkTFitsIn<int>(info.utf8Range.size()) ? info.utf8Range.size() : INT_MAX;
  
  const auto& runBuffer = fBuilder.allocRunTextPos(info.fFont, glyphCount, utf8RangeSize);
  if (runBuffer.utf8text && fUtf8Text) {
    memcpy(runBuffer.utf8text, fUtf8Text + info.utf8Range.begin(), utf8RangeSize);
  }
  fClusters = runBuffer.clusters;
  fGlyphCount = glyphCount;
  fClusterOffset = info.utf8Range.begin();
  
  return { runBuffer.glyphs,
    runBuffer.points(),
    nullptr,
    runBuffer.clusters,
    fCurrentPosition };
}

void TextRunHandler::commitRunBuffer(const RunInfo& info) {
  SkASSERT(0 <= fClusterOffset);
  for (int i = 0; i < fGlyphCount; ++i) {
    SkASSERT(fClusters[i] >= (unsigned)fClusterOffset);
    fClusters[i] -= fClusterOffset;
  }
  fCurrentPosition += info.fAdvance;
}

void TextRunHandler::commitLine() {
  fOffset += { fCurrentPosition.fX, 0 };
}

sk_sp<SkTextBlob> TextRunHandler::makeBlob() {
  return fBuilder.make();
}

void TextRunHandler::peeked(size_t bytes) {
  peekedBytes = bytes;
}
