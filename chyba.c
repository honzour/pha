/*********************************************************/
/* chyba.c - ukonceni programu po kriticke chybe         */
/* 10.3. 2001 Jan Nemec                                  */
/*********************************************************/
#include "volby.h"
#if Typ_Produktu==WindowsProgram
#include <windows.h>
#else
#include <stdio.h>
#endif
#include <stdlib.h>

#if (Typ_Produktu==Projekt)
#include "scio.h"
#endif
#include "chyba.h"

/***********************************************************
 * Nasilne a nestandardne ukonci program a napise chybovou *
 * hlasku. Volat jen po katastrofalni chybe                *
 ***********************************************************/
void Chyba(const char* str)
#if (Typ_Produktu==DOS_UNIX || Typ_Produktu==Qt_Program || Typ_Produktu==DLL)
{
  printf("Chyba: %s\n", str);
  exit(1);
}
#elif (Typ_Produktu==CGI_script)
{
  printf("Content-type: text/plain\n\nChyba: %s\n",str);
  exit(1);
}
#elif Typ_Produktu==WindowsProgram
{ 
  MessageBox(0,(LPCWSTR)str, (LPCWSTR)"Chyba programu",MB_OK | MB_TASKMODAL);
  abort();
}
#elif Typ_Produktu==Projekt
{ 
  fprintf(stderr,"Chyba: %s\n",str); 
  exit(1);
}
#endif
