#include "volby.h"
#include "cotyvar.h"
#include "partie.h"
#include "dejtah.h"

void test(TUloha *uloha, TPozice *pozice, u16* tahy, int pocet, int *slozitost)
{
	int iterace;
	*slozitost = 0;
	for (iterace = 0; iterace < pocet; iterace++)
	{
		TTah1 t1;
		
		InitPartie(&(uloha->prt), pozice + iterace);
		PromazTabulky(uloha);
		DejTah(uloha, &t1, 1000 * 60 * 60 * 24, MaxHloubkaPropoctu, NULL, NULL);
	}
}
