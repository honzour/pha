/*********************************************************/
/* volby.h - nastaveni zpusobu prekladu                  */
/* 31.12. 2000 Jan Nemec                                 */
/*********************************************************/
/*Zdrojaky pujde ve vysledku pouzit na vice veci.
Je treba urcit, co ma byt cil.*/
#ifndef volbyH
#define volbyH

#define PHAVerze "2019"

/* Program pro 32/64 bitove Windows */
#define WindowsProgram 1

/* DLL pro 32/64 bitove Windows pro napojení na XBoard/UCI rozhraní */
#define DLL 2

/* Interaktivni program v textovem modu pro Unix nebo Windows */
#define DOS_UNIX 3

/* CGI script */
#define CGI_script 4

/* Součást projektu "Šachove centrum" */
#define Projekt 5

/* Qt Program */
#define Qt_Program 6

#ifdef _WINDLL
#define Typ_Produktu DLL
#endif

#if Typ_Produktu==DOS_UNIX
#define Vypisy
/*#define HTML_VYPISY*/
#ifdef unix
#define ODDELOVAC "/"
#else
#define ODDELOVAC "\\"
#endif
#ifdef unix
#define ODDELOVAC_VARIANT "/"
#else
#define ODDELOVAC_VARIANT "_"
#endif

#define HTML_CESTA ("html" ODDELOVAC)
#endif
#if Typ_Produktu==DOS_UNIX ||Typ_Produktu==WindowsProgram
#define Statistika
#endif
#if Typ_Produktu==WindowsProgram
#define STRINGTAH /*Ukladani retezcu do partie*/
#endif
#ifdef __cplusplus
#define cfunkce extern "C"
#else
#define cfunkce
#endif
#endif
/*od #define volby*/
