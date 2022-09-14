//
//  RunIterators.h
//  qlik-oss-react-native-helium
//
//  Created by Vittorio Cellucci on 2022-07-16.
//

#ifndef RunIterators_h
#define RunIterators_h
#include <modules/skshaper/include/SkShaper.h>

class CustomBdiRunIterator : public SkShaper::TrivialBiDiRunIterator {
public:
  CustomBdiRunIterator(uint8_t bidiLevel, size_t utf8Bytes) : TrivialBiDiRunIterator(bidiLevel, utf8Bytes) {}
 
};


#endif /* RunIterators_h */
