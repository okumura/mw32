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

#include "mw32/module.h"
#include "mw32/hash.h"
#include "mw32_private.h"

static LPVOID GetPeb() {
#if defined(_WIN64)
	UINT64 uiPeb = __readgsqword(0x60);
	return (LPVOID) (UINT_PTR) uiPeb;
#else
	UINT32 uiPeb = __readfsdword(0x30);
	return (LPVOID) (UINT_PTR) uiPeb;
#endif
}

static PMW32_PEB_LDR_DATA GetPebLdrData() {
	PBYTE pPeb = (PBYTE) GetPeb();
#if defined(_WIN64)
	PUINT64 ppPebLdrData = (PUINT64) (pPeb + 0x18);
#else
	PUINT32 ppPebLdrData = (PUINT32) (pPeb + 0x0C);
#endif
	return (PMW32_PEB_LDR_DATA) ((UINT_PTR) *ppPebLdrData);
}

static HMODULE_ENTRY CheckedModule(HMODULE_ENTRY hEntry) {
	if (!hEntry) {
		return NULL;
	} else if (!hEntry->BaseAddress) {
		return NULL;
	} else {
		return hEntry;
	}
}

HMODULE_ENTRY mw32ModuleFindFirst() {
	PMW32_PEB_LDR_DATA pPebLdrData = GetPebLdrData();
	PLIST_ENTRY pEntry = pPebLdrData->InLoadOrderModuleList.Flink;
	return CheckedModule((HMODULE_ENTRY) pEntry);
}

HMODULE_ENTRY mw32FindModuleNext(HMODULE_ENTRY hEntry) {
	hEntry = CheckedModule(hEntry);
	if (!hEntry) {
		return NULL;
	} else {
		PLIST_ENTRY pEntry = hEntry->InLoadOrderModuleList.Flink;
		return CheckedModule((HMODULE_ENTRY) pEntry);
	}
}

HMODULE_ENTRY mw32ModuleByHandle(HANDLE hModule) {
	if (!hModule) {
		return NULL;
	} else {
		HMODULE_ENTRY hEntry;

		MW32_FOR_EACH_MODULE(hEntry) {
			if (hEntry->BaseAddress == (LPVOID) hModule) {
				return hEntry;
			}
		}

		return NULL;
	}
}

HMODULE_ENTRY mw32ModuleByBaseName(DWORD dwHashLower) {
	HMODULE_ENTRY hEntry;

	MW32_FOR_EACH_MODULE(hEntry) {
		if (mw32HashStringLowerW(hEntry->BaseDllName.Buffer) == dwHashLower) {
			return hEntry;
		}
	}

	return NULL;
}

HMODULE_ENTRY mw32ModuleByFullName(DWORD dwHashLower) {
	HMODULE_ENTRY hEntry;

	MW32_FOR_EACH_MODULE(hEntry) {
		if (mw32HashStringLowerW(hEntry->FullDllName.Buffer) == dwHashLower) {
			return hEntry;
		}
	}

	return NULL;
}

BOOL mw32ModuleIsValid(HMODULE_ENTRY hEntry) {
	return CheckedModule(hEntry) != NULL;
}

LPVOID mw32ModuleGetBaseAddress(HMODULE_ENTRY hEntry) {
	if (!mw32ModuleIsValid(hEntry)) {
		return NULL;
	} else {
		return hEntry->BaseAddress;
	}
}

HMODULE mw32ModuleGetHandle(HMODULE_ENTRY hEntry) {
	return (HMODULE) mw32ModuleGetBaseAddress(hEntry);
}

static LPCWSTR GetName(PUNICODE_STRING lpString) {
	if (!lpString) {
		return NULL;
	} else if (!lpString->Buffer) {
		return NULL;
	} else if (!lpString->Length >= 0x1000) {
		return NULL;
	} else {
		return lpString->Buffer;
	}
}

LPCWSTR mw32ModuleGetBaseName(HMODULE_ENTRY hEntry) {
	if (!mw32ModuleIsValid(hEntry)) {
		return NULL;
	} else {
		return GetName(&hEntry->BaseDllName);
	}
}

LPCWSTR mw32ModuleGetFullName(HMODULE_ENTRY hEntry) {
	if (!mw32ModuleIsValid(hEntry)) {
		return NULL;
	} else {
		return GetName(&hEntry->FullDllName);
	}
}
