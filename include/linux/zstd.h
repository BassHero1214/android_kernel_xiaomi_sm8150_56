/*
 * Copyright (c) 2016-present, Yann Collet, Facebook, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of https://github.com/facebook/zstd.
 * An additional grant of patent rights can be found in the PATENTS file in the
 * same directory.
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License version 2 as published by the
 * Free Software Foundation. This program is dual-licensed; you may select
 * either version 2 of the GNU General Public License ("GPL") or BSD license
 * ("BSD").
 */

#ifndef ZSTD_H
#define ZSTD_H

/* ======   Dependency   ======*/
#include <linux/types.h>   /* size_t */
#include <linux/zstd_errors.h> /* Error code enum and helpers */


/*-*****************************************************************************
 * Introduction
 * ...（其余注释内容照旧，这里省略）
 ******************************************************************************/

/*======  Helper functions  ======*/

/**
 * Functions that return size_t can be checked for errors using ZSTD_isError()
 * and the ZSTD_ErrorCode can be extracted using ZSTD_getErrorCode().
 * Error code enum is now only defined in <linux/zstd_errors.h>
 */

/**
 * ZSTD_maxCLevel() - maximum compression level available
 *
 * Return: Maximum compression level available.
 */
int ZSTD_maxCLevel(void);
/**
 * ZSTD_compressBound() - maximum compressed size in worst case scenario
 * @srcSize: The size of the data to compress.
 *
 * Return:   The maximum compressed size in the worst case scenario.
 */
size_t ZSTD_compressBound(size_t srcSize);
/**
 * ZSTD_isError() - tells if a size_t function result is an error code
 * @code:  The function result to check for error.
 *
 * Return: Non-zero iff the code is an error.
 */
static __attribute__((unused)) unsigned int ZSTD_isError(size_t code)
{
	return code > (size_t)-ZSTD_error_maxCode;
}

/*--- Zstd types and API declarations ---*/

typedef struct ZSTD_CCtx_s ZSTD_CCtx;
typedef struct ZSTD_DCtx_s ZSTD_DCtx;
typedef struct ZSTD_CDict_s ZSTD_CDict;
typedef struct ZSTD_DDict_s ZSTD_DDict;

/**
 * enum ZSTD_strategy - zstd compression search strategy
 *
 * From faster to stronger.
 */
typedef enum {
	ZSTD_fast,
	ZSTD_dfast,
	ZSTD_greedy,
	ZSTD_lazy,
	ZSTD_lazy2,
	ZSTD_btlazy2,
	ZSTD_btopt,
	ZSTD_btopt2
} ZSTD_strategy;

typedef struct {
	unsigned int windowLog;
	unsigned int chainLog;
	unsigned int hashLog;
	unsigned int searchLog;
	unsigned int searchLength;
	unsigned int targetLength;
	ZSTD_strategy strategy;
} ZSTD_compressionParameters;

typedef struct {
	unsigned int contentSizeFlag;
	unsigned int checksumFlag;
	unsigned int noDictIDFlag;
} ZSTD_frameParameters;

typedef struct {
	ZSTD_compressionParameters cParams;
	ZSTD_frameParameters fParams;
} ZSTD_parameters;

/* Compression parameter helpers */
ZSTD_compressionParameters ZSTD_getCParams(int compressionLevel,
	unsigned long long estimatedSrcSize, size_t dictSize);
ZSTD_parameters ZSTD_getParams(int compressionLevel,
	unsigned long long estimatedSrcSize, size_t dictSize);
size_t ZSTD_CCtxWorkspaceBound(ZSTD_compressionParameters cParams);

/* Compression context management */
ZSTD_CCtx *ZSTD_initCCtx(void *workspace, size_t workspaceSize);

/* Compression API */
size_t ZSTD_compressCCtx(ZSTD_CCtx *ctx, void *dst, size_t dstCapacity,
	const void *src, size_t srcSize, ZSTD_parameters params);

/* Decompression context management */
size_t ZSTD_DCtxWorkspaceBound(void);
ZSTD_DCtx *ZSTD_initDCtx(void *workspace, size_t workspaceSize);

/* Decompression API */
size_t ZSTD_decompressDCtx(ZSTD_DCtx *ctx, void *dst, size_t dstCapacity,
	const void *src, size_t srcSize);

/* Dictionary API */
size_t ZSTD_CDictWorkspaceBound(ZSTD_compressionParameters cParams);
ZSTD_CDict *ZSTD_initCDict(const void *dictBuffer, size_t dictSize,
	ZSTD_parameters params, void *workspace, size_t workspaceSize);
size_t ZSTD_compress_usingCDict(ZSTD_CCtx *cctx, void *dst, size_t dstCapacity,
	const void *src, size_t srcSize, const ZSTD_CDict *cdict);

size_t ZSTD_DDictWorkspaceBound(void);
ZSTD_DDict *ZSTD_initDDict(const void *dictBuffer, size_t dictSize,
	void *workspace, size_t workspaceSize);
size_t ZSTD_decompress_usingDDict(ZSTD_DCtx *dctx, void *dst,
	size_t dstCapacity, const void *src, size_t srcSize,
	const ZSTD_DDict *ddict);

/* Streaming API types */
typedef struct ZSTD_inBuffer_s {
	const void *src;
	size_t size;
	size_t pos;
} ZSTD_inBuffer;

typedef struct ZSTD_outBuffer_s {
	void *dst;
	size_t size;
	size_t pos;
} ZSTD_outBuffer;

typedef struct ZSTD_CStream_s ZSTD_CStream;
typedef struct ZSTD_DStream_s ZSTD_DStream;

/* Streaming API */
size_t ZSTD_CStreamWorkspaceBound(ZSTD_compressionParameters cParams);
ZSTD_CStream *ZSTD_initCStream(ZSTD_parameters params,
	unsigned long long pledgedSrcSize, void *workspace,
	size_t workspaceSize);
ZSTD_CStream *ZSTD_initCStream_usingCDict(const ZSTD_CDict *cdict,
	unsigned long long pledgedSrcSize, void *workspace,
	size_t workspaceSize);
size_t ZSTD_resetCStream(ZSTD_CStream *zcs, unsigned long long pledgedSrcSize);
size_t ZSTD_compressStream(ZSTD_CStream *zcs, ZSTD_outBuffer *output,
	ZSTD_inBuffer *input);
size_t ZSTD_flushStream(ZSTD_CStream *zcs, ZSTD_outBuffer *output);
size_t ZSTD_endStream(ZSTD_CStream *zcs, ZSTD_outBuffer *output);
size_t ZSTD_CStreamInSize(void);
size_t ZSTD_CStreamOutSize(void);

size_t ZSTD_DStreamWorkspaceBound(size_t maxWindowSize);
ZSTD_DStream *ZSTD_initDStream(size_t maxWindowSize, void *workspace,
	size_t workspaceSize);
ZSTD_DStream *ZSTD_initDStream_usingDDict(size_t maxWindowSize,
	const ZSTD_DDict *ddict, void *workspace, size_t workspaceSize);
size_t ZSTD_resetDStream(ZSTD_DStream *zds);
size_t ZSTD_decompressStream(ZSTD_DStream *zds, ZSTD_outBuffer *output,
	ZSTD_inBuffer *input);
size_t ZSTD_DStreamInSize(void);
size_t ZSTD_DStreamOutSize(void);

/* Constants (for static allocation, etc.) */
#define ZSTD_MAGICNUMBER            0xFD2FB528   /* >= v0.8.0 */
#define ZSTD_MAGIC_SKIPPABLE_START  0x184D2A50U

#define ZSTD_CONTENTSIZE_UNKNOWN (0ULL - 1)
#define ZSTD_CONTENTSIZE_ERROR   (0ULL - 2)

#define ZSTD_WINDOWLOG_MAX_32  27
#define ZSTD_WINDOWLOG_MAX_64  27
#define ZSTD_WINDOWLOG_MAX \
	((unsigned int)(sizeof(size_t) == 4 \
		? ZSTD_WINDOWLOG_MAX_32 \
		: ZSTD_WINDOWLOG_MAX_64))
#define ZSTD_WINDOWLOG_MIN 10
#define ZSTD_HASHLOG_MAX ZSTD_WINDOWLOG_MAX
#define ZSTD_HASHLOG_MIN        6
#define ZSTD_CHAINLOG_MAX     (ZSTD_WINDOWLOG_MAX+1)
#define ZSTD_CHAINLOG_MIN      ZSTD_HASHLOG_MIN
#define ZSTD_HASHLOG3_MAX      17
#define ZSTD_SEARCHLOG_MAX    (ZSTD_WINDOWLOG_MAX-1)
#define ZSTD_SEARCHLOG_MIN      1
#define ZSTD_SEARCHLENGTH_MAX   7
#define ZSTD_SEARCHLENGTH_MIN   3
#define ZSTD_TARGETLENGTH_MIN   4
#define ZSTD_TARGETLENGTH_MAX 999

#define ZSTD_FRAMEHEADERSIZE_MAX 18
#define ZSTD_FRAMEHEADERSIZE_MIN  6
static const size_t ZSTD_frameHeaderSize_prefix = 5;
static const size_t ZSTD_frameHeaderSize_min = ZSTD_FRAMEHEADERSIZE_MIN;
static const size_t ZSTD_frameHeaderSize_max = ZSTD_FRAMEHEADERSIZE_MAX;
static const size_t ZSTD_skippableHeaderSize = 8;

/* Block API */
#define ZSTD_BLOCKSIZE_ABSOLUTEMAX (128 * 1024)
size_t ZSTD_getBlockSizeMax(ZSTD_CCtx *cctx);
size_t ZSTD_compressBlock(ZSTD_CCtx *cctx, void *dst, size_t dstCapacity,
	const void *src, size_t srcSize);
size_t ZSTD_decompressBlock(ZSTD_DCtx *dctx, void *dst, size_t dstCapacity,
	const void *src, size_t srcSize);
size_t ZSTD_insertBlock(ZSTD_DCtx *dctx, const void *blockStart,
	size_t blockSize);

#endif  /* ZSTD_H */
