#pragma once

#ifndef MW32_BASE_H
#define MW32_BASE_H

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

DWORD mw32GetPID();
DWORD mw32GetTID();

#ifdef __cplusplus
}
#endif

#endif /* MW32_BASE_H */
