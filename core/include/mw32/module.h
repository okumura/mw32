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

typedef struct MODULE_ENTRY MODULE_ENTRY;
typedef MODULE_ENTRY *HMODULE_ENTRY;

#ifdef __cplusplus
extern "C" {
#endif

HMODULE_ENTRY mw32ModuleFindFirst();
HMODULE_ENTRY mw32ModuleFindNext(HMODULE_ENTRY hEntry);

HMODULE_ENTRY mw32ModuleByHandle(HANDLE hModule);
HMODULE_ENTRY mw32ModuleByBaseName(DWORD dwLowerHash);
HMODULE_ENTRY mw32ModuleByFullName(DWORD dwLowerHash);

BOOL    mw32ModuleIsValid(HMODULE_ENTRY hEntry);
LPVOID  mw32ModuleGetBaseAddress(HMODULE_ENTRY hEntry);
HMODULE mw32ModuleGetHandle(HMODULE_ENTRY hEntry);
LPCWSTR mw32ModuleGetBaseName(HMODULE_ENTRY hEntry);
LPCWSTR mw32ModuleGetFullName(HMODULE_ENTRY hEntry);

#ifdef __cplusplus
}
#endif

#define MW32_FOR_EACH_MODULE(varname) \
	for (varname = mw32ModuleFindFirst();\
			varname; varname = mw32ModuleFindNext(varname))\

#endif /* MW32_MODULE_H */
