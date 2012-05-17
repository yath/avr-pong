#ifndef _BITS_H_
#define _BITS_H_
#include <avr/sfr_defs.h> /* for _BV */

#define BV(x) _BV(x)

/* from <dqqj9j$31l$1@rzsun03.rrz.uni-hamburg.de> */
#define _BITS_H_NARG(...) \
         _BITS_H_NARG_(__VA_ARGS__,_BITS_H_RSEQ_N())
#define _BITS_H_NARG_(...) \
         _BITS_H_ARG_N(__VA_ARGS__)
#define _BITS_H_ARG_N( \
          _1, _2, _3, _4, _5, _6, _7, _8, _9,_10, \
         _11,_12,_13,_14,_15,_16,_17,_18,_19,_20, \
         _21,_22,_23,_24,_25,_26,_27,_28,_29,_30, \
         _31,_32,_33,_34,_35,_36,_37,_38,_39,_40, \
         _41,_42,_43,_44,_45,_46,_47,_48,_49,_50, \
         _51,_52,_53,_54,_55,_56,_57,_58,_59,_60, \
         _61,_62,_63,N,...) N
#define _BITS_H_RSEQ_N() \
         63,62,61,60,                   \
         59,58,57,56,55,54,53,52,51,50, \
         49,48,47,46,45,44,43,42,41,40, \
         39,38,37,36,35,34,33,32,31,30, \
         29,28,27,26,25,24,23,22,21,20, \
         19,18,17,16,15,14,13,12,11,10, \
         9,8,7,6,5,4,3,2,1,0

#define _BITS_H_OR_BITS_1(a)       (BV(a))
#define _BITS_H_OR_BITS_2(a, b)    (BV(a)|BV(b))
#define _BITS_H_OR_BITS_3(b, ...)  _BITS_H_OR_BITS_2(__VA_ARGS__)|BV(b)
#define _BITS_H_OR_BITS_4(b, ...)  _BITS_H_OR_BITS_3(__VA_ARGS__)|BV(b)
#define _BITS_H_OR_BITS_5(b, ...)  _BITS_H_OR_BITS_4(__VA_ARGS__)|BV(b)
#define _BITS_H_OR_BITS_6(b, ...)  _BITS_H_OR_BITS_5(__VA_ARGS__)|BV(b)
#define _BITS_H_OR_BITS_7(b, ...)  _BITS_H_OR_BITS_6(__VA_ARGS__)|BV(b)
#define _BITS_H_OR_BITS_8(b, ...)  _BITS_H_OR_BITS_7(__VA_ARGS__)|BV(b)
#define _BITS_H_OR_BITS_9(b, ...)  _BITS_H_OR_BITS_8(__VA_ARGS__)|BV(b)
#define _BITS_H_OR_BITS_10(b, ...) _BITS_H_OR_BITS_9(__VA_ARGS__)|BV(b)
#define _BITS_H_OR_BITS_11(b, ...) _BITS_H_OR_BITS_10(__VA_ARGS__)|BV(b)
#define _BITS_H_OR_BITS_12(b, ...) _BITS_H_OR_BITS_11(__VA_ARGS__)|BV(b)
#define _BITS_H_OR_BITS_13(b, ...) _BITS_H_OR_BITS_12(__VA_ARGS__)|BV(b)
#define _BITS_H_OR_BITS_14(b, ...) _BITS_H_OR_BITS_13(__VA_ARGS__)|BV(b)
#define _BITS_H_OR_BITS_15(b, ...) _BITS_H_OR_BITS_14(__VA_ARGS__)|BV(b)
#define _BITS_H_OR_BITS_16(b, ...) _BITS_H_OR_BITS_15(__VA_ARGS__)|BV(b)
#define _BITS_H_OR_BITS_17(b, ...) _BITS_H_OR_BITS_16(__VA_ARGS__)|BV(b)
#define _BITS_H_OR_BITS_18(b, ...) _BITS_H_OR_BITS_17(__VA_ARGS__)|BV(b)
#define _BITS_H_OR_BITS_19(b, ...) _BITS_H_OR_BITS_18(__VA_ARGS__)|BV(b)
#define _BITS_H_OR_BITS_20(b, ...) _BITS_H_OR_BITS_19(__VA_ARGS__)|BV(b)
#define _BITS_H_OR_BITS_21(b, ...) _BITS_H_OR_BITS_20(__VA_ARGS__)|BV(b)
#define _BITS_H_OR_BITS_22(b, ...) _BITS_H_OR_BITS_21(__VA_ARGS__)|BV(b)
#define _BITS_H_OR_BITS_23(b, ...) _BITS_H_OR_BITS_22(__VA_ARGS__)|BV(b)
#define _BITS_H_OR_BITS_24(b, ...) _BITS_H_OR_BITS_23(__VA_ARGS__)|BV(b)
#define _BITS_H_OR_BITS_25(b, ...) _BITS_H_OR_BITS_24(__VA_ARGS__)|BV(b)
#define _BITS_H_OR_BITS_26(b, ...) _BITS_H_OR_BITS_25(__VA_ARGS__)|BV(b)
#define _BITS_H_OR_BITS_27(b, ...) _BITS_H_OR_BITS_26(__VA_ARGS__)|BV(b)
#define _BITS_H_OR_BITS_28(b, ...) _BITS_H_OR_BITS_27(__VA_ARGS__)|BV(b)
#define _BITS_H_OR_BITS_29(b, ...) _BITS_H_OR_BITS_28(__VA_ARGS__)|BV(b)
#define _BITS_H_OR_BITS_30(b, ...) _BITS_H_OR_BITS_29(__VA_ARGS__)|BV(b)
#define _BITS_H_OR_BITS_31(b, ...) _BITS_H_OR_BITS_30(__VA_ARGS__)|BV(b)
#define _BITS_H_OR_BITS_32(b, ...) _BITS_H_OR_BITS_31(__VA_ARGS__)|BV(b)

#define _BITS_H_CAT(a, b)   a ## b
#define _BITS_H_XCAT(a, b)  _BITS_H_CAT(a, b)

#define OR_BITS(...) (_BITS_H_XCAT(_BITS_H_OR_BITS_, _BITS_H_NARG(__VA_ARGS__))(__VA_ARGS__))

#define SET_BITS(reg, ...) (reg) |= OR_BITS(__VA_ARGS__)
#define CLR_BITS(reg, ...) (reg) &= ~(OR_BITS(__VA_ARGS__))
#define TGL_BITS(reg, ...) (reg) ^= OR_BITS(__VA_ARGS__)
#define GET_BITS(reg, ...) ((reg) & (OR_BITS(__VA_ARGS__)))
#define TST_BITS(reg, ...) (GET_BITS(reg, __VA_ARGS__) == OR_BITS(__VA_ARGS__))

#define SET_BIT SET_BITS
#define CLR_BIT CLR_BITS
#define TGL_BIT TGL_BITS
#define GET_BIT GET_BITS
#define TST_BIT TST_BITS

#ifndef CHAR_BIT
#define CHAR_BIT 8
#endif

#define MSB(x) (sizeof(x)*CHAR_BIT-1)
#define LSB(x) 0

#define MSBV(x) BV(MSB(x))
#define LSBV(x) BV(LSB(x))

#endif /* _BITS_H_ */
