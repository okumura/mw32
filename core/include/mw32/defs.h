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

#ifndef MW32_DEFS_H
#define MW32_DEFS_H

enum {
	MW32_HASH_LOWERW_NTDLL_DLL      = 0x1cde56f9,
	MW32_HASH_LOWERW_KERNEL32_DLL   = 0xc705800d,
	MW32_HASH_LOWERW_KERNELBASE_DLL = 0xedb36c29,

	MW32_HASH_LOWERW_USER32_DLL     = 0xd2584e27,
	MW32_HASH_LOWERW_ADVAPI32_DLL   = 0x10258865,
	MW32_HASH_LOWERW_SHELL32_DLL    = 0x9efe1188,
	MW32_HASH_LOWERW_SHLWAPI_DLL    = 0x351c1a57,

	MW32_HASH_LOWERW_GDI32_DLL      = 0x8bff619c,
	
	MW32_HASH_LOWERW_CRYPT32_DLL    = 0x442054aa,
	MW32_HASH_LOWERW_WS2_32_DLL     = 0x33bb3fa7,
	MW32_HASH_LOWERW_WININET_DLL    = 0x4769bcd9,
	MW32_HASH_LOWERW_WINHTTP_DLL    = 0xb41bc7f7,

	MW32_HASH_LOWERW_SETUPAPI_DLL   = 0x83886cb0,
};

#endif /* MW32_DEFS_H */
