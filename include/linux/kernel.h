/**
 *  @author  : Rancho Cooper
 *  @date    : 2016-09-28 15:44
 *  @email   : ranchocooper@gmail.com
 */

#define container_of(ptr, type, member) ({                  \
    const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
    (type *) ( (char*)__mptr - offsetoff(type, member) );   \
})
