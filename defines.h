#pragma once
#include <cstdint>

using s64 = int64_t;
using u64 = uint64_t;
using s32 = int32_t;
using u32 = uint32_t;
using s16 = int16_t;
using u16 = uint16_t;
using s8 = int8_t;
using u8 = uint8_t;

using f64 = double;
using f32 = float;

enum class key : u8 {
  none = 0,
  ctrl = 1,
  alt = 2,
  enter = '\r',
  esc = '\e',
  f1,
  f2,
  f3,
  f4,
  f5,
  f6,
  f7,
  f8,
  f9,
  f10,
  f11,
  f12,
  _1 = '1',
  _2 = '2',
  _3 = '3',
  _4 = '4',
  _5 = '5',
  _6 = '6',
  _7 = '7',
  _8 = '8',
  _9 = '9',
  a = 'a',
  b = 'b',
  c = 'c',
  d = 'd',
  e = 'e',
  f = 'f',
  g = 'g',
  h = 'h',
  i = 'i',
  j = 'j',
  k = 'k',
  l = 'l',
  m = 'm',
  n = 'n',
  o = 'o',
  p = 'p',
  q = 'q',
  r = 'r',
  s = 's',
  t = 't',
  u = 'u',
  v = 'v',
  w = 'w',
  x = 'x',
  y = 'y',
  z = 'z',
};
