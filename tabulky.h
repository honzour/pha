/*********************************************************/
/* tabulky.h - prace s databazi koncovek                 */
/* 8.3. 2002 Jan Nemec                                 */
/*********************************************************/
#ifndef tabulkyH
#define tabulkyH
#include "cotyvar.h"
cfunkce void GenerujTabulky();
cfunkce int GenerujTabulku(const char *typ);
cfunkce u16 TabDejTah(TUloha *u);
cfunkce void TSchTypStr(s8 *sch, char *s);
#endif

