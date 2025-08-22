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
/**
 * ZSTD_getErrorCode() - translates an error function result to a ZSTD_ErrorCode
 * @functionResult: The result of a function for which ZSTD_isError() is true.
 *
 * Return:          The ZSTD_ErrorCode corresponding to the functionResult or 0
 *                  if the functionResult isn't an error.
 */
static __attribute__((unused)) ZSTD_ErrorCode ZSTD_getErrorCode(
	size_t functionResult)
{
	if (!ZSTD_isError(functionResult))
		return (ZSTD_ErrorCode)0;
	return (ZSTD_ErrorCode)(0 - functionResult);
}

#endif  /* ZSTD_H */
