/**
 *  @author  : Rancho Cooper
 *  @date    : 2016-09-28 08:08
 *  @email   : ranchocooper@gmail.com
 */

/**
 *  @note: change two pointing by size-bytes
 *  to avoid compiler optimizing use volatile specifier
 */
static __always_inline void __write_once_size(volatile void *p, void *res, int size) {
    switch (size) {
        case 1: *(volatile __u8 *)p = *(__u8 *)res;
        case 2: *(volatile __u16 *)p = *(__u16 *)res;
        case 4: *(volatile __u32 *)p = *(__u32 *)res;
        case 8: *(volatile __u64 *)p = *(__u64 *)res;
}
/**
 *  @note: just the terse version of WRITE_ONCE
 *  how to know when we should use it?
 *  also confused me a lot!
 *  <linux/kernel.h> too complicated!
 */
#define WRITE_ONCE(x, val) x = (val)
