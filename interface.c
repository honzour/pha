#include <string.h>
#include "volby.h"
#include "interface.h"
#include "dejtah.h"
#include "dllmain.h"
#include "strtah.h"
#include "partie.h"
#include "globruti.h"
#include "ohodnoc.h"
#include "sachy.h"
#include "dosio.h"
#include "perft.h"

#pragma warning(disable : 4996)

TCallbacks Callbacks;

void SetStartPos()
{
    globalUloha->pozice = ZakladniPostaveni;
    InitPartie(&globalUloha->prt, &globalUloha->pozice);
}

void SetPositionFromString(char* AFEN)
{
    /* TODO : Implement */
}

void DoMove(char* AMove)
{
    u16 tah1;
    if (LongStrToTah(AMove, globalUloha, &tah1)) {
        GlobTahni(0, globalUloha, 1, tah1);
    }
    else
    {
		char str[256];
		
        sprintf(str, "Invalid move: %s", AMove);
        Callbacks.TellGUIError(str);

    }
}

void Undo()
{
    GlobTahniZpet(globalUloha);
}

void Analyze(u8 AInfinite)
{
    globalUloha->Analyze = AInfinite != 0;
    TTah1 t1;
    int i = DejTah(globalUloha, &t1, 999999999, 99, NULL, NULL);
    if (i == 0)
    {
        Callbacks.TellGUIInfo("No move");
    }

    Callbacks.OnStop();
}

void Stop()
{
    if (globalUloha != NULL)
        globalUloha->StavPropoctu.MamKoncitMysleni = 1;
}

void BookMoves()
{
    if (globalUloha != NULL)
        GetBookMoves(globalUloha);
}

void WriteBoardToConsole()
{
    TiskniSachovnici(&globalUloha->pozice, stdout);
}

void WriteEvalToConsole()
{
	char str[64];
	sprintf(str, "Total Score: %i", (int)HodnotaPozice(globalUloha, -mat, mat));
	Callbacks.TellGUIInfo(str);
}

u64 Perft(u8 AValue)
{
    return GetPerft(globalUloha, AValue);
}

void SetCallbacks(TCallbacks *ACallbacks)
{
    Callbacks = *ACallbacks;
}

void SetOptionOwnBook(u8 AValue)
{
    ChciKnihovnu = AValue;
}

void SetOptionHash(ptrdiff_t AValue)
{
    SetUlohaHash(&globalUloha, AValue);
}

void SetOptionClearHash()
{
    PromazTabulky(globalUloha);
}
