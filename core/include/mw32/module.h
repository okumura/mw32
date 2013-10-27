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

#pragma once

#ifndef MW32_MODULE_H
#define MW32_MODULE_H

#include "mw32/types.h"

typedef struct MW32MODULE MW32MODULE;
typedef MW32MODULE *HMW32MODULE;

typedef struct MW32PROC {
	DWORD dwIndex;
	DWORD dwOrdinal;
	LPCSTR lpszName;
	FARPROC lpAddress;
} MW32PROC, *LPMW32PROC;

#ifdef __cplusplus
extern "C" {
#endif

HMW32MODULE mw32ModuleFirst();
HMW32MODULE mw32ModuleNext(HMW32MODULE hEntry);
HMW32MODULE mw32ModuleFromHandle(HANDLE hModule);
HMW32MODULE mw32ModuleFromBaseName(DWORD dwLowerHash);
HMW32MODULE mw32ModuleFromFullName(DWORD dwLowerHash);

BOOL    mw32ModuleIsValid(HMW32MODULE hEntry);
LPVOID  mw32ModuleGetBaseAddress(HMW32MODULE hEntry);
HMODULE mw32ModuleGetHandle(HMW32MODULE hEntry);
LPCWSTR mw32ModuleGetBaseName(HMW32MODULE hEntry);
LPCWSTR mw32ModuleGetFullName(HMW32MODULE hEntry);

LPMW32PROC
mw32ModuleProcFirst(
		HMW32MODULE hEntry,
		LPMW32PROC lpProc);

LPMW32PROC
mw32ModuleProcNext(
		HMW32MODULE hEntry,
		LPMW32PROC lpProc);

DWORD
mw32ModuleGetProcs(
		HMW32MODULE hEntry,
		FARPROC *lpDst,
		const DWORD *lpdwHashes,
		DWORD dwCount);

FARPROC
mw32ModuleGetProc(
		HMW32MODULE hEntry,
		DWORD dwHash);

#ifdef __cplusplus
}
#endif

#define MW32_FOR_EACH_MODULE(varname) \
	for (varname = mw32ModuleFirst();\
			varname; varname = mw32ModuleNext(varname))\

#define MW32_MODULE_GET_PROCS(hEntry, lpDst, Src) \
	(mw32ModuleGetProcs(hEntry, lpDst, Src, _countof(Src)) == _countof(Src))

#define MW32_MODULE_FOR_EACH_PROC(hEntry, lpProc) \
	for (lpProc = mw32ModuleProcFirst(hEntry, lpProc);\
			lpProc; lpProc = mw32ModuleProcNext(hEntry, lpProc))\

#endif /* MW32_MODULE_H */
