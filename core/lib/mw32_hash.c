/*
 * Copyright (c) 2013 OKUMURA Yoshio <yoshio.okumura@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "mw32/hash.h"

#define FNV1_32_INIT ((DWORD) 0x811c9dc5)
#define FNV_32_PRIME ((DWORD) 0x01000193)

static int ToUpper(int c) {
	if (c >= 'a' && c <= 'z') {
		return 'A' + (c - 'a');
	} else {
		return c;
	}
}

static int ToLower(int c) {
	if (c >= 'A' && c <= 'Z') {
		return 'a' + (c - 'A');
	} else {
		return c;
	}
}


DWORD mw32HashStringA(LPCSTR s) {
	const unsigned char *p = (const unsigned char *) s;
	DWORD h = FNV1_32_INIT;
	unsigned char c;

	while ((c = *p++)) {
		h ^= c;
		h *= FNV_32_PRIME;
	}

	return h;
}

DWORD mw32HashStringW(LPCSTR s) {
	const unsigned short *p = (const unsigned short *) s;
	DWORD h = FNV1_32_INIT;
	unsigned short c;

	while ((c = *p++)) {
		unsigned char c1 = (c & 0x00ff) >> 0;
		unsigned char c2 = (c & 0xff00) >> 8;
		h ^= c1;
		h *= FNV_32_PRIME;
		h ^= c2;
		h *= FNV_32_PRIME;
	}

	return h;
}

DWORD mw32HashStringLowerA(LPCSTR s) {
	const unsigned char *p = (const unsigned char *) s;
	DWORD h = FNV1_32_INIT;
	unsigned char c;

	while ((c = *p++)) {
		h ^= ToLower(c);
		h *= FNV_32_PRIME;
	}

	return h;
}

DWORD mw32HashStringLowerW(LPCWSTR s) {
	const unsigned short *p = (const unsigned short *) s;
	DWORD h = FNV1_32_INIT;
	unsigned short c;

	while ((c = *p++)) {
		unsigned short cl = ToLower(c);
		unsigned char c1 = (cl & 0x00ff) >> 0;
		unsigned char c2 = (cl & 0xff00) >> 8;
		h ^= c1;
		h *= FNV_32_PRIME;
		h ^= c2;
		h *= FNV_32_PRIME;
	}

	return h;
}
