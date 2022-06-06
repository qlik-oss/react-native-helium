//
//  UTF8Utils.hpp
//
// Note this was lifted from SKUTF8.cpp from SKia Project
//


#ifndef UTF8Utils_hpp
#define UTF8Utils_hpp

#include <stdio.h>
#include <string>

inline bool isASCII (const std::string& s)
{
  if(s.length() == 0) {
    return true;
  }

  unsigned char c = static_cast<unsigned char>(s[0]);
  if(c > 127) {
    return false;
  }
  return true;
}


static constexpr inline int32_t left_shift(int32_t value, int32_t shift) {
  return (int32_t) ((uint32_t) value << shift);
}

template <typename T>
static SkUnichar next_fail(const T** ptr, const T* end) {
  *ptr = end;
  return -1;
}

static int utf8_byte_type(uint8_t c) {
  if (c < 0x80) {
    return 1;
  } else if (c < 0xC0) {
    return 0;
  } else if (c >= 0xF5 || (c & 0xFE) == 0xC0) { // "octet values c0, c1, f5 to ff never appear"
    return -1;
  } else {
    int value = (((0xe5 << 24) >> ((unsigned)c >> 4 << 1)) & 3) + 1;
    // assert(value >= 2 && value <=4);
    return value;
  }
}
static bool utf8_type_is_valid_leading_byte(int type) { return type > 0; }

static bool utf8_byte_is_continuation(uint8_t c) { return utf8_byte_type(c) == 0; }

static SkUnichar nextUTF8(const char** ptr, const char* end) {
  if (!ptr || !end ) {
    return -1;
  }
  const uint8_t*  p = (const uint8_t*)*ptr;
  if (!p || p >= (const uint8_t*)end) {
    return next_fail(ptr, end);
  }
  int             c = *p;
  int             hic = c << 24;

  if (!utf8_type_is_valid_leading_byte(utf8_byte_type(c))) {
    return next_fail(ptr, end);
  }
  if (hic < 0) {
    uint32_t mask = (uint32_t)~0x3F;
    hic = left_shift(hic, 1);
    do {
      ++p;
      if (p >= (const uint8_t*)end) {
        return next_fail(ptr, end);
      }
      // check before reading off end of array.
      uint8_t nextByte = *p;
      if (!utf8_byte_is_continuation(nextByte)) {
        return next_fail(ptr, end);
      }
      c = (c << 6) | (nextByte & 0x3F);
      mask <<= 5;
    } while ((hic = left_shift(hic, 1)) < 0);
    c &= ~mask;
  }
  *ptr = (char*)p + 1;
  return c;
}

#endif /* UTF8Utils_hpp */
