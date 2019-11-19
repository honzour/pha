/*********************************************************/
/* sachy.h - obsahuje main a zakladni init a done        */
/* 31.12. 2000 Jan Nemec                                 */
/*********************************************************/
#ifndef sachyH
#define sachyH

/*********************************************/
/* Provede inicializaci celeho programu      */
/* (srand, knihovna...)                      */
/*********************************************/
cfunkce void InitProgram(TUloha **uloha, const char *knihsoubor);

/*********************************************/
/* Inicializace ulohy (Alokace tabulek atd)  */
/*********************************************/
cfunkce TUloha *InitUlohu(int DveNaXHash, int DveNaXHashPech, int DveNaXHashNejlepsi);

cfunkce void SetUlohaHash(TUloha** AUloha, ptrdiff_t AHashSize);

/**********************************/
/* mazani ulohy a jejich tabulek  */
/**********************************/
cfunkce void DoneUlohu(TUloha **uloha);

/**
  Smaze hodnoty tabulek
*/
cfunkce void PromazTabulky(TUloha *uloha);
#endif
