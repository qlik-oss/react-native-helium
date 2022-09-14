//
//  TextRunHundler.hpp
//  qlik-oss-react-native-helium
//
//  Created by Vittorio Cellucci on 2022-07-12.
//

#ifndef TextRunHundler_hpp
#define TextRunHundler_hpp

#include <stdio.h>
#include "include/core/SkFontMgr.h"
#include "include/core/SkPoint.h"
#include "include/core/SkRefCnt.h"
#include "include/core/SkScalar.h"
#include "include/core/SkTextBlob.h"
#include "include/core/SkTypes.h"
#include <modules/skshaper/include/SkShaper.h>


class TextRunHandler : public SkShaper::RunHandler {
public:
 
  TextRunHandler(const char* utf8Text, SkPoint offset);
  sk_sp<SkTextBlob> makeBlob();
  SkPoint endPoint() { return fOffset; }
  
  void beginLine() override;
  void runInfo(const RunInfo&) override;
  void commitRunInfo() override;
  Buffer runBuffer(const RunInfo&) override;
  void commitRunBuffer(const RunInfo&) override;
  void commitLine() override;
  void advance(const char* p);
  void peeked(size_t bytes);
  void reset();
  
  void setText(const char* utf) {
    fUtf8Text = utf;
  }
  void newLine() {
    fCurrentPosition.fX = 0;
    fOffset.fX = 0;
    fOffset += {0, fMaxRunDescent + fMaxRunLeading - fMaxRunAscent };
  }
  
private:
  SkTextBlobBuilder fBuilder;
  const char* fUtf8Text;
  uint32_t* fClusters;
  size_t fClusterOffset;
  size_t fGlyphCount;
  SkScalar fMaxRunAscent;
  SkScalar fMaxRunDescent;
  SkScalar fMaxRunLeading;
  SkPoint fCurrentPosition;
  SkPoint fOffset;
  size_t peekedBytes = -1;
};
#endif /* TextRunHundler_hpp */
