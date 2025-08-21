#ifndef ZSTD_DEPS_H
#define ZSTD_DEPS_H
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/string.h>
#include <linux/slab.h>
#include <linux/bug.h>
#include <linux/byteorder/generic.h>
#include <asm/unaligned.h>
#ifndef ZSTD_STATIC_ASSERT
# define ZSTD_STATIC_ASSERT(c) BUILD_BUG_ON(!(c))
#endif
#ifndef DEBUG_STATIC_ASSERT
# define DEBUG_STATIC_ASSERT(c) BUILD_BUG_ON(!(c))
#endif
#define ZSTD_readLE16(p)      get_unaligned_le16(p)
#define ZSTD_readLE24(p)      (get_unaligned_le32(p) & 0xFFFFFFu)
#define ZSTD_readLE32(p)      get_unaligned_le32(p)
#define ZSTD_readLE64(p)      get_unaligned_le64(p)
#define ZSTD_writeLE32(p,v)   put_unaligned_le32((v),(p))
#define ZSTD_malloc(s)        kmalloc((s), GFP_KERNEL)
#define ZSTD_calloc(n,s)      kcalloc((n),(s), GFP_KERNEL)
#define ZSTD_realloc(p,s)     krealloc((p),(s), GFP_KERNEL)
#define ZSTD_free(p)          kfree((p))
#ifndef ZSTD_DEBUG
# define ZSTD_DEBUG 0
#endif
#if ZSTD_DEBUG
# define ZSTD_LOG(...)        pr_debug(__VA_ARGS__)
#else
# define ZSTD_LOG(...)        do { } while (0)
#endif
#define ZSTD_MULTITHREAD      0
#endif
#ifndef INLINE_KEYWORD
#define INLINE_KEYWORD static inline
#endif
