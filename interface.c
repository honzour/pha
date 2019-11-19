#include <string.h>
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
        GlobTahni(0, globalUloha, true, tah1);
    }
    else
    {
        std::string str(AMove);
        std::string s = "Invalid move: " + str;
        Callbacks.TellGUIError(s.c_str());
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
    std::string message[20];
    message->append("Total Score: ");
    message->append(std::to_string(HodnotaPozice(globalUloha, -mat, mat)));
    Callbacks.TellGUIInfo(message->c_str());
}

u64 Perft(u8 AValue)
{
    return GetPerft(globalUloha, AValue);
}

void SetCallbacks(TCallbacks& ACallbacks)
{
    Callbacks = ACallbacks;
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
