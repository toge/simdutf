#ifndef SIMDUTF_UTF16_TO_UTF32_H
#define SIMDUTF_UTF16_TO_UTF32_H

namespace simdutf {
namespace scalar {
namespace {
namespace utf16_to_utf32 {

template <endianness big_endian>
inline size_t convert(const char16_t* buf, size_t len, char32_t* utf32_output) {
 const uint16_t *data = reinterpret_cast<const uint16_t *>(buf);
  size_t pos = 0;
  char32_t* start{utf32_output};
  while (pos < len) {
    uint16_t word = big_endian ? utf16::swap_bytes(data[pos]) : data[pos];
    if((word &0xF800 ) != 0xD800) {
      // No surrogate pair, extend 16-bit word to 32-bit word
      *utf32_output++ = char32_t(word);
      pos++;
    } else {
      // must be a surrogate pair
      uint16_t diff = uint16_t(word - 0xD800);
      if(diff > 0x3FF) { return 0; }
      if(pos + 1 >= len) { return 0; } // minimal bound checking
      uint16_t next_word = big_endian ? utf16::swap_bytes(data[pos + 1]) : data[pos + 1];
      uint16_t diff2 = uint16_t(next_word - 0xDC00);
      if(diff2 > 0x3FF) { return 0; }
      uint32_t value = (diff << 10) + diff2 + 0x10000;
      *utf32_output++ = char32_t(value);
      pos += 2;
    }
  }
  return utf32_output - start;
}

template <endianness big_endian>
inline result convert_with_errors(const char16_t* buf, size_t len, char32_t* utf32_output) {
 const uint16_t *data = reinterpret_cast<const uint16_t *>(buf);
  size_t pos = 0;
  char32_t* start{utf32_output};
  while (pos < len) {
    uint16_t word = big_endian ? utf16::swap_bytes(data[pos]) : data[pos];
    if((word &0xF800 ) != 0xD800) {
      // No surrogate pair, extend 16-bit word to 32-bit word
      *utf32_output++ = char32_t(word);
      pos++;
    } else {
      // must be a surrogate pair
      uint16_t diff = uint16_t(word - 0xD800);
      if(diff > 0x3FF) { return result(error_code::SURROGATE, pos); }
      if(pos + 1 >= len) { return result(error_code::SURROGATE, pos); } // minimal bound checking
      uint16_t next_word = big_endian ? utf16::swap_bytes(data[pos + 1]) : data[pos + 1];
      uint16_t diff2 = uint16_t(next_word - 0xDC00);
      if(diff2 > 0x3FF) { return result(error_code::SURROGATE, pos); }
      uint32_t value = (diff << 10) + diff2 + 0x10000;
      *utf32_output++ = char32_t(value);
      pos += 2;
    }
  }
  return result(error_code::SUCCESS, utf32_output - start);
}

} // utf16_to_utf32 namespace
} // unnamed namespace
} // namespace scalar
} // namespace simdutf

#endif