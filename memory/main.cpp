#include "memory.h"
#include "iostream"
#pragma comment(lib, "onecore.lib")
#include <Windows.h>

int main() {
    //HANDLE token;
    //if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &token))
    //    return false;

    //LUID luid;
    //if (!LookupPrivilegeValueW(nullptr, SE_LOCK_MEMORY_NAME, &luid))
    //    return false;

    //TOKEN_PRIVILEGES privileges;
    //privileges.PrivilegeCount = 1;
    //privileges.Privileges[0].Luid = luid;
    //privileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    //BOOL result = AdjustTokenPrivileges(token, false, &privileges, sizeof(TOKEN_PRIVILEGES), nullptr, nullptr);
    //if (1300 == GetLastError())
    //    printf("error");
    ////CloseHandle(hToken);
    ////ERROR_SUCCESS


    ////MEM_ADDRESS_REQUIREMENTS requirement;
    ////requirement.LowestStartingAddress = NULL;
    ////requirement.HighestEndingAddress = NULL;
    ////requirement.Alignment = 4 * 1024 * 1024 * 1024; // align to 4GB boundary

    //MEM_EXTENDED_PARAMETER xp;
    ////xp[0].Type = MemExtendedParameterAddressRequirements;
    ////xp[0].Pointer = &requirement;

    //xp.Type = MemExtendedParameterAttributeFlags;
    //xp.ULong64 = MEM_EXTENDED_PARAMETER_NONPAGED_HUGE; // 1GB pages required

    ////auto page = GetLargePageMinimum();

    //for (;;) {
    //    int size = 0;
    //    std::cin >> size;
    //    //auto a = VirtualAlloc(nullptr, GetLargePageMinimum() * size, MEM_LARGE_PAGES | MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    //    auto b = VirtualAlloc2(nullptr, nullptr, GetLargePageMinimum() * size, MEM_LARGE_PAGES | MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE, nullptr, 0);
    //    printf("%p\n", b);
    //}
    //auto c = GetLastError();
    //system("pause");

    library::allocate<int>();
    library::allocate<int>(1);

}