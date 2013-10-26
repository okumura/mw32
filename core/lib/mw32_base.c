#include "mw32/base.h"

DWORD mw32GetPID() {
#if defined(_M_X64)
	return __readgsdword(0x40);
#else
	return __readfsdword(0x20);
#endif
}

DWORD mw32GetTID() {
#if defined(_M_X64)
	return __readgsdword(0x48);
#else
	return __readfsdword(0x24);
#endif
}
