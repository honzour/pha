/* dllmain.cpp : Defines the entry point for the DLL application. */
#include "dllmain.h"
#include "interface.h"

#include "sachy.h"
#include "hashtab.h"
#include "dejtah.h"

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

TUloha* globalUloha;

int APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
/*        GenerujTabulky(); */
        HashInit();
        ChciKnihovnu = 1;
        globalUloha = InitUlohu(0, 17 /* 1 << 17 * 44 = 5.5 MB */, 19 /* 1 << 19 * 8 = 4 MB */);
        SetUlohaHash(&globalUloha, 1024 /* MB */);
        break;
    }
    case DLL_THREAD_ATTACH:
    {
        break;
    }
    case DLL_THREAD_DETACH:
    {
        break;
    }
    case DLL_PROCESS_DETACH:
        DoneUlohu(&globalUloha);
#ifdef _DEBUG
        _CrtDumpMemoryLeaks();
#endif
        break;
    }
    return TRUE;
}
