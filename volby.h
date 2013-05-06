/*********************************************************/
/* volby.h - nastaveni zpusobu prekladu                  */
/* 31.12. 2000 Jan Nemec                                 */
/*********************************************************/
/*Zdrojaky pujde ve vysledku pouzit na vice veci.
Je treba urcit, co ma byt cil.*/
#ifndef volbyH
#define volbyH
#define PHAVerze "26.2.2001"
#define Win32_Program 0
/* Program pro 32 bitove Windows */
#define Win32_DLL 1
/*DLL pro 32 bitove Windows  - tato varianta není implementována*/
#define DOS_UNIX 2
/*interaktivni program v textovem modu pro UNIX
nebo dvaatricetibitove WINDOWS*/
#define CGI_script 3
/* CGI script */
#define Projekt 4
/* Souèást projektu šachové centrum */
#define Qt_Program 5
#if 0
#ifdef _WINDOWS
#define Typ_Produktu Win32_Program 
#else
#define Typ_Produktu  DOS_UNIX
/*#define Typ_Produktu Qt_Program   Projekt  DOS_UNIX CGI_script
tohle provedem v Makefile pomoci napr. -DTyp_Produktu=5
*/
#endif
#endif
#ifdef WIN_DOS_HACK
#define Typ_Produktu  DOS_UNIX
#endif
#if Typ_Produktu==DOS_UNIX
#define Vypisy
//#define HTML_VYPISY
#ifdef unix
#define ODDELOVAC "/"
#else
#define ODDELOVAC "\\"
#endif
#define HTML_CESTA ("html" ODDELOVAC)
#endif
#if Typ_Produktu==DOS_UNIX ||Typ_Produktu==Win32_Program
#define Statistika
#endif
#if Typ_Produktu==Win32_Program
#define STRINGTAH /*Ukladani retezcu do partie*/
#endif
#ifdef __cplusplus
#define cfunkce extern "C"
#else
#define cfunkce
#endif
#endif
/*od #define volby*/
