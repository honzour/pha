#include "volby.h"
#include "cotyvar.h"
#include "partie.h"

void test(TUloha *uloha, TPozice *pozice, u16* tahy, int pocet, int *slozitost)
{
	int iterace;
	*slozitost = 0;
	for (iterace = 0; iterace < pocet; iterace++)
	{
		InitPartie(&(uloha->prt), pozice + iterace);
	}
}
