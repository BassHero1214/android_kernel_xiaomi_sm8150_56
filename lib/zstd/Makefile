# SPDX-License-Identifier: GPL-2.0
CFLAGS_MODULE := $(filter-out -mlittle-endian,$(CFLAGS_MODULE))
ccflags-y += -I$(src)/common

# 公共
lib-y += common/entropy_common.o common/error_private.o common/fse_decompress.o common/zstd_common.o

# 压缩
lib-$(CONFIG_ZSTD_COMPRESS) += compress/fse_compress.o \
                               compress/huf_compress.o \
                               compress/zstd_compress.o \
                               compress/zstd_compress_literals.o \
                               compress/zstd_compress_sequences.o \
                               compress/zstd_compress_superblock.o \
                               compress/zstd_double_fast.o \
                               compress/zstd_fast.o \
                               compress/zstd_lazy.o \
                               compress/zstd_ldm.o \
                               compress/zstd_opt.o \
                               compress/hist.o
# 如保留预分裂优化，再加上：
# lib-$(CONFIG_ZSTD_COMPRESS) += compress/zstd_preSplit.o

# 解压
lib-$(CONFIG_ZSTD_DECOMPRESS) += decompress/huf_decompress.o \
                                 decompress/zstd_ddict.o \
                                 decompress/zstd_decompress.o \
                                 decompress/zstd_decompress_block.o
