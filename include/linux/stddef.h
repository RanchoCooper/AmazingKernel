/**
 *  @author  : Rancho Cooper
 *  @date    : 2016-09-28 15:45
 *  @email   : ranchocooper@gmail.com
 */

#ifdef __compiler_offsetof
#define offsetof(TYPE, MEMBER) __compiler_offsetof(TYPE, MEMBER)
#else
#define offsetof(TYPE, MEMBER) ((size_t)&((TYPE *)0)->MEMBER)
#endif
