#include "../includes.h"

namespace memory {
	static HMODULE base;

	HMODULE get_base()
	{
		if (!base)
			base = GetModuleHandle(NULL);
		return base;
	}

	DWORD_OF_BITNESS get_base_address()
	{
		return (DWORD_OF_BITNESS)get_base();
	}

	bool is_valid_ptr(PVOID ptr)
	{
		return (ptr >= (PVOID)0x10000) && (ptr < PTRMAXVAL) && ptr != nullptr && !IsBadReadPtr(ptr, sizeof(ptr));
	}
}
