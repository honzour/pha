/*********************************************************/
/* generato.h - generator tahu                           */
/* 31.12. 2000 Jan Nemec                                 */
/*********************************************************/
#ifndef generatoH
#define generatoH
#include "volby.h"
#include "cotyvar.h"

/* Kolik stoji napr. jezdec */
extern const s16 StdCenyFigur[7]; 

/* Jak figurky skacou */
extern const s8 ofsety[16]; 

/*******************************************************************************/
/* NalezTahy - generuje pseudolegalni (jako by se kral smel octnout v sachu)   */
/* tahy na zasobnik. Zacne na hranice[pos], pos nezvysi, ale na zaver          */
/* spravne nastavi hranice[pos+1]. Takze pokud to zavolam 2* po sobe,          */
/* ma to tentyz efekt, jako bych to zavolal 1*. Nastavi cenu tahu podle        */
/* zmeny materialu                                                             */
/*******************************************************************************/
cfunkce void NalezTahy(TUloha *u);

/*******************************************************************************/
/* NalezBrani                                                                  */
/* Generuje vsechny pseudolegalni (jako kdyby kral mohl jit do sachu) brani    */
/* a promeny pesce v damu. Jinak obdoba NalezTahy                              */
/*******************************************************************************/
cfunkce void NalezBrani(TUloha *u);

/*****************************************************/
/* Nalezne legalni tahy, je-li hrac na tahu v sachu  */
/*****************************************************/
cfunkce void NalezTahyZeSachu(TUloha *u);

/************************************************************************/
/* Setridi aktualni uroven tahu na zasobniku podle cena+hitor. heur     */
/************************************************************************/
cfunkce void SetridTahy(TUloha *uloha);

/****************************************/
/* Kolikrat je dane policko ohrozeno    */
/****************************************/
cfunkce int PocetOhrozeni(s8 *p, int bilym);

/*********************************************************/
/* Nalezne vsechny kameny bileho, ktere jsou             */
/* ve vazbe vuci poli pole. (Na poli je napr. bily kral) */
/*********************************************************/
cfunkce void NalezBileVazby(TUloha *u,int pole);
#endif
