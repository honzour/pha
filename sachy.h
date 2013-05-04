/*********************************************************/
/* sachy.h - obsahuje main a zakladni init a done        */
/* 31.12. 2000 Jan Nemec                                 */
/*********************************************************/
#ifndef sachyH
#define sachyH
cfunkce void InitProgram(TUloha **uloha, char *knihsoubor);
/*********************************************/
/* Provede inicializaci celeho programu      */
/* (srand, knihovna...)                      */
/*********************************************/
cfunkce TUloha *InitUlohu(int DveNaXHash, int DveNaXHashPech, int DveNaXHashNejlepsi);
/*********************************************/
/* Inicializace ulohy (Alokace tabulek atd)  */
/*********************************************/
cfunkce void DoneUlohu(TUloha **uloha);
/**********************************/
/* mazani ulohy a jejich tabulek  */
/**********************************/
/**
  Smaze hodnoty tabulek
*/
cfunkce void PromazTabulky(TUloha *uloha);
#endif
