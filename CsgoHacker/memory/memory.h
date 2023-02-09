#pragma once

namespace memory {
#ifdef _WIN64
#define DWORD_OF_BITNESS DWORD64
#define PTRMAXVAL ((PVOID)0x000F000000000000)
#elif _WIN32
#define DWORD_OF_BITNESS DWORD
#define PTRMAXVAL ((PVOID)0xFFF00000)
#endif

    extern bool is_valid_ptr(PVOID ptr);

    extern HMODULE get_base();

    template <class T>
    T read(const HANDLE process, const DWORD address) noexcept {
        T value{};
        if (ReadProcessMemory(process, reinterpret_cast<LPCVOID>(address), &value, sizeof(T), nullptr) == false)
        {
            return value;
        }
        return value;
    }

    template <class T>
    void write(const HANDLE process, const DWORD address, const T& value) noexcept {
        if (WriteProcessMemory(process, reinterpret_cast<LPVOID>(address), &value, sizeof(T), nullptr) == false)
        {
            return;
        }
    }
}