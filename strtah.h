/*********************************************************/
/* strtah.h - prevod tahu na retezec a naopak            */
/* 31.12. 2000 Jan Nemec                                 */
/*********************************************************/
#ifndef strtahH
#define strtahH
#include "cotyvar.h"

/* Staci urceni Da1 */
#define J_Nic 0

/* Staci urceni D1a1 */
#define J_Radka 1

/* Staci urceni Dha1 */
#define J_Sloupec 2

/* c5d6ep (6 + 1) */
#define MAX_LONG_MOVE_SIZE 7 

cfunkce int JednoZnacny(u16 tah, TUloha *uloha,int urcen);

/**********************************************************************/
/* StrToTah - musi byt tesne pred provedenim tahu a na zasobniku musi */
/*            byt ty tahy nalezene                                    */
/**********************************************************************/
cfunkce int StrToTah(char *s, TUloha *uloha, u16 *tah1);


cfunkce int LongStrToTah(char *s, TUloha *uloha, u16 *tah1);

/**********************************************************************/
/* TahToStr - musi byt tesne pred provedenim tahu a na zasobniku musi */
/*            byt ty tahy nalezene                                    */
/**********************************************************************/
cfunkce void TahToStr(u16 tah, TUloha *uloha, char *s);


cfunkce void TahToLongStr(u16 tah, char *s);
#endif

