#include "perft.h"

#include "generato.h"
#include "myslitel.h"
#include "globruti.h"
#include "lokruti.h"

u64 GetPerftInternal(TUloha* uloha, int ADepth) {
    NalezTahy(uloha);
    JenPripustne(uloha);
    int zanor = uloha->zasobnik.pos;
    if (ADepth <= 0)
    {
        return uloha->zasobnik.hranice[zanor + 1] - uloha->zasobnik.hranice[zanor];
    }
    else
    {
        UlozStav(uloha);
        TTah1* pt1, * pt2;
        NASTAVOKRAJE(pt1, pt2);
        u64 count = 0;
        for (; pt1 < pt2; pt1++)
        {
            Tahni(pt1->data, uloha);
            count += GetPerftInternal(uloha, ADepth - 1);
            TahniZpet(pt1->data, uloha);
            if (uloha->StavPropoctu.MamKoncitMysleni)
                break;                
        }
        return count;
    }
}

u64 GetPerft(TUloha* uloha, u8 ADepth) {
    uloha->StavPropoctu.MamKoncitMysleni = 0;
    InitVypocet(uloha);
    return GetPerftInternal(uloha, ADepth - 1);
}
