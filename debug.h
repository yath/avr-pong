#ifndef _DEBUG_H_
#define _DEBUG_H_

#ifdef NDEBUG
# define DEBUGGING 0
#endif

#ifndef DEBUGGING
# define DEBUGGING 1
#endif

#if DEBUGGING == 1
# define DEBUG_BLOCK(...) __VA_ARGS__
# include <stdio.h>
# include <avr/pgmspace.h> /* for PSTR() */
# include <util/atomic.h>
  extern FILE debugf;
  void debug_init(void);
# define STR(x) # x
# define ONEZERO(x) ((x)?'1':'0')
# define DEBUG(fmt, ...) DEBUG2(__FILE__, __LINE__, fmt, ##__VA_ARGS__)
# define DUMPBYTE(reg) do { \
    uint8_t __copy = reg; \
    DEBUG("dump of " # reg " (MSB first): %c%c%c%c %c%c%c%c", \
        ONEZERO(GET_BIT(__copy, 7)), ONEZERO(GET_BIT(__copy, 6)), \
        ONEZERO(GET_BIT(__copy, 5)), ONEZERO(GET_BIT(__copy, 4)), \
        ONEZERO(GET_BIT(__copy, 3)), ONEZERO(GET_BIT(__copy, 2)), \
        ONEZERO(GET_BIT(__copy, 1)), ONEZERO(GET_BIT(__copy, 0))); \
    } while(0)

# define DEBUG2(file, line, fmt, ...) do { \
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) { \
        fprintf_P(&debugf, PSTR(file ":" STR(line) ": " fmt "\n"), ##__VA_ARGS__); \
    } \
  } while(0)
#elif DEBUGGING == 0
# ifndef NDEBUG
#  define NDEBUG
# endif
# define DEBUG_BLOCK(x)
# define DEBUG(fmt, args...)
# define debug_init(...)
#else
# error "DEBUGGING must be set to either 0 or 1"
#endif

#endif /* _DEBUG_H_ */
