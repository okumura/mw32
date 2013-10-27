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

/* private implementations. */
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

static HMW32MODULE CheckedModule(HMW32MODULE hEntry) {
	if (!hEntry) {
		return NULL;
	} else if (!hEntry->BaseAddress) {
		return NULL;
	} else {
		return hEntry;
	}
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

static PVOID GetImageDataDirectory(LPBYTE pModuleBase, DWORD dwDirectory) {
	PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER) pModuleBase;
	PIMAGE_NT_HEADERS pNtHeader = (PIMAGE_NT_HEADERS) (pModuleBase + pDosHeader->e_lfanew);

	DWORD dwRvaAndSizes = pNtHeader->OptionalHeader.NumberOfRvaAndSizes;
	PIMAGE_DATA_DIRECTORY pDataDir = pNtHeader->OptionalHeader.DataDirectory;
	if (!dwRvaAndSizes) {
		return NULL;
	} else if (dwDirectory >= dwRvaAndSizes) {
		return NULL;
	} else if (!pDataDir[dwDirectory].Size) {
		return NULL;
	} else {
		return (PVOID) (pModuleBase + pDataDir[dwDirectory].VirtualAddress);
	}
}

static PIMAGE_EXPORT_DIRECTORY GetImageExportDirectory(LPBYTE pModuleBase) {
	return (PIMAGE_EXPORT_DIRECTORY) GetImageDataDirectory(pModuleBase, IMAGE_DIRECTORY_ENTRY_EXPORT);
}

/* public implementations. */
HMW32MODULE mw32ModuleFirst() {
	PMW32_PEB_LDR_DATA pPebLdrData = GetPebLdrData();
	PLIST_ENTRY pEntry = pPebLdrData->InLoadOrderModuleList.Flink;
	return CheckedModule((HMW32MODULE) pEntry);
}

HMW32MODULE mw32FindModuleNext(HMW32MODULE hEntry) {
	hEntry = CheckedModule(hEntry);
	if (!hEntry) {
		return NULL;
	} else {
		PLIST_ENTRY pEntry = hEntry->InLoadOrderModuleList.Flink;
		return CheckedModule((HMW32MODULE) pEntry);
	}
}

HMW32MODULE mw32ModuleFromHandle(HANDLE hModule) {
	if (!hModule) {
		return NULL;
	} else {
		HMW32MODULE hEntry;

		MW32_FOR_EACH_MODULE(hEntry) {
			if (hEntry->BaseAddress == (LPVOID) hModule) {
				return hEntry;
			}
		}

		return NULL;
	}
}

HMW32MODULE mw32ModuleFromBaseName(DWORD dwHashLower) {
	HMW32MODULE hEntry;

	MW32_FOR_EACH_MODULE(hEntry) {
		if (mw32HashStringLowerW(hEntry->BaseDllName.Buffer) == dwHashLower) {
			return hEntry;
		}
	}

	return NULL;
}

HMW32MODULE mw32ModuleFromFullName(DWORD dwHashLower) {
	HMW32MODULE hEntry;

	MW32_FOR_EACH_MODULE(hEntry) {
		if (mw32HashStringLowerW(hEntry->FullDllName.Buffer) == dwHashLower) {
			return hEntry;
		}
	}

	return NULL;
}

BOOL mw32ModuleIsValid(HMW32MODULE hEntry) {
	return CheckedModule(hEntry) != NULL;
}

LPVOID mw32ModuleGetBaseAddress(HMW32MODULE hEntry) {
	if (!mw32ModuleIsValid(hEntry)) {
		return NULL;
	} else {
		return hEntry->BaseAddress;
	}
}

HMODULE mw32ModuleGetHandle(HMW32MODULE hEntry) {
	return (HMODULE) mw32ModuleGetBaseAddress(hEntry);
}

LPCWSTR mw32ModuleGetBaseName(HMW32MODULE hEntry) {
	if (!mw32ModuleIsValid(hEntry)) {
		return NULL;
	} else {
		return GetName(&hEntry->BaseDllName);
	}
}

LPCWSTR mw32ModuleGetFullName(HMW32MODULE hEntry) {
	if (!mw32ModuleIsValid(hEntry)) {
		return NULL;
	} else {
		return GetName(&hEntry->FullDllName);
	}
}

LPMW32PROC
mw32ModuleProcFirst(
		HMW32MODULE hEntry,
		LPMW32PROC lpProc) {
	if (!CheckedModule(hEntry)) {
		return NULL;
	} else if (!lpProc) {
		return NULL;
	} else {
		lpProc->dwIndex = 0;
		return mw32ModuleProcNext(hEntry, lpProc);
	}
}

LPMW32PROC
mw32ModuleProcNext(
		HMW32MODULE hEntry,
		LPMW32PROC lpProc) {
	LPBYTE lpModuleBase;
	PIMAGE_EXPORT_DIRECTORY pExportDir;
	DWORD dwNumNames;
	PWORD  pOrdinals;
	PDWORD pNameRVAs;
	PDWORD pFuncRVAs;
	DWORD i;

	if (!CheckedModule(hEntry)) {
		return NULL;
	} else if (!lpProc) {
		return NULL;
	}

	lpModuleBase = (LPBYTE) mw32ModuleGetBaseAddress(hEntry);
	if (!lpModuleBase) {
		return NULL;
	}

	pExportDir = GetImageExportDirectory(lpModuleBase);
	if (!pExportDir) {
		return NULL;
	}

	dwNumNames = pExportDir->NumberOfNames;
	if (!dwNumNames) {
		return NULL;
	} else if (lpProc->dwIndex >= dwNumNames) {
		return NULL;
	}

	pOrdinals = (PWORD)  (lpModuleBase + pExportDir->AddressOfNameOrdinals);
	pNameRVAs = (PDWORD) (lpModuleBase + pExportDir->AddressOfNames);
	pFuncRVAs = (PDWORD) (lpModuleBase + pExportDir->AddressOfFunctions);
	
	i = lpProc->dwIndex;
	lpProc->dwIndex++;
	lpProc->dwOrdinal = pOrdinals[i];
	lpProc->lpszName = (LPCSTR) (lpModuleBase + pNameRVAs[i]);
	lpProc->lpAddress = (FARPROC) (lpModuleBase + pFuncRVAs[pOrdinals[i]]);

	return lpProc;
}

DWORD
mw32ModuleGetProcs(
		HMW32MODULE hEntry,
		FARPROC *lpDst,
		const DWORD *lpdwHashes,
		DWORD dwCount) {
	DWORD i;
	DWORD dwMatched;
	MW32PROC Proc;
	LPMW32PROC lpProc;

	if (!CheckedModule(hEntry)) {
		return 0;
	} else if (!lpDst) {
		return 0;
	} else if (!lpdwHashes) {
		return 0;
	} else if (!dwCount) {
		return 0;
	}

	for (i = 0; i < dwCount; i++) {
		lpDst[i] = NULL;
	}

	dwMatched = 0;
	lpProc = &Proc;
	MW32_MODULE_FOR_EACH_PROC(hEntry, lpProc) {
		DWORD dwHash = mw32HashStringA(lpProc->lpszName);
		for (i = 0; i < dwCount; i++) {
			if (!lpDst[i]) {
				if (lpdwHashes[i] == dwHash) {
					lpDst[i] = lpProc->lpAddress;
					dwMatched++;
					if (dwMatched == dwCount) {
						return dwCount;
					} else {
						break;
					}
				}
			}
		}
	}

	return dwMatched;
}

FARPROC
mw32ModuleGetProc(
		HMW32MODULE hEntry,
		DWORD dwHash) {
	FARPROC Proc;

	if (mw32ModuleGetProcs(hEntry, &Proc, &dwHash, 1) != 1) {
		return NULL;
	} else {
		return Proc;
	}
}
