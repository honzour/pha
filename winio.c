/*********************************************************/
/* winio.c - IO interaktivni verze pro Windows           */
/* 31.12. 2000 Jan Nemec                                 */
/*********************************************************/
#include "volby.h"
#if Typ_Produktu==Win32_Program  || Typ_Produktu==Win32_DLL
#include <windows.h>
#include <math.h>
#include <stdarg.h>
#include "winmain.h"
#include "cotyvar.h"
#include "globruti.h"
#include "generato.h"
#include "lokruti.h"
#include "myslitel.h"
#include "pole.h"
s8 Oznaceno=0; /*Které políèko je oznaèené. Je-li 0, tak žádné.*/
TSch KSachovnice;
volatile int Myslim=0;

void WinTiskniFiguru(HDC KamDC,HDC BMPDC,int f,int x,int y)
 {int x0,y0;
   if(f && f<7 && f>-7)
    {
     x0=(abs(f)-1)*72;
     y0= f>0 ? 72 : 144;
     BitBlt(KamDC,x,y,72,72,BMPDC,x0,0,SRCAND);
     BitBlt(KamDC,x,y,72,72,BMPDC,x0,y0,SRCPAINT);
    }
 }

void WinTiskniPole(HDC KamDC, HDC BMPDC, int cerne, int oznacene, int x, int y) {
  BitBlt(KamDC, x, y, VelPole, VelPole, BMPDC, cerne * VelPole + oznacene * 2 * VelPole, 0, SRCCOPY);
}

/* TiskniNPoli - Tiskne pole i s figurami aktuální pozice. Ukonèeno 0.
   Pole jsou typu int.*/
void TiskniNPoli(HWND hwnd,...) {
  HDC FDC,PDC,dc;
  va_list p;
  int pole,x,y,c;


   dc=GetDC(hwnd);
   FDC=CreateCompatibleDC(dc);
   SelectObject(FDC,hFigury);
   PDC=CreateCompatibleDC(dc);
   SelectObject(PDC,hPole);
   va_start(p, hwnd);
   while ((pole = va_arg(p,int)) != 0)
    {
     x=(pole % 10)-1; if(x<0||x>7) {MessageBox(NULL,"pøi zobrazení tahu","Chyba",MB_OK);continue;}
     y=(pole / 10)-2; if(y<0||y>7) {MessageBox(NULL,"pøi zobrazení tahu","Chyba",MB_OK);continue;}
     c=!((x+y)&1);
     if (Nastaveni.BilyHrajeNahoru)
      {x=Nastaveni.PosunSchX+x*VelPole;
       y=Nastaveni.PosunSchY+(7-y)*VelPole;}
      else
      {x=Nastaveni.PosunSchX+(7-x)*VelPole;
       y=Nastaveni.PosunSchY+y*VelPole;}
     WinTiskniPole(dc,PDC,c,pole==Oznaceno,x,y);
     WinTiskniFiguru(dc,FDC,
		 (Myslim ? KSachovnice:uloha->pozice.sch)[pole],x+2,y+2);
    }
   va_end(p);
   ReleaseDC(hwnd,dc);
   DeleteDC(FDC);
   DeleteDC(PDC);
 }

void WinTiskniSachovnici(HDC DC, int x, int y, TSch *sch)
 {int i,j,x0,y0;
  HDC FDC,PDC;

  FDC=CreateCompatibleDC(DC);
  SelectObject(FDC,hFigury);
  PDC=CreateCompatibleDC(DC);
  SelectObject(PDC,hPole);
  for(i=0;i<=7;i++)
   for(j=0;j<=7;j++)
/*Cyklus dìlám pøes políèka struktury šachovnice (ne pøes políèka na obrazovce)*/
    {
     x0=(Nastaveni.BilyHrajeNahoru ? x+i*VelPole : x+(7-i)*VelPole);
     y0=(Nastaveni.BilyHrajeNahoru ? y+(7-j)*VelPole : y+j*VelPole);
     WinTiskniPole(DC,PDC,!((i+j)&1),21+i+10*j==Oznaceno,x0,y0);
     WinTiskniFiguru(DC,FDC,(*sch)[21+i+10*j],x0+2,y0+2);
    }
  DeleteDC(FDC);
  DeleteDC(PDC);
 }


/* Tiskne tah poté, co už byl zahrán */
void WinTiskniTah(HWND hwnd, u16 tah) {
  int odkud;
  int kam;

  if(!(tah>>14)) /* Normalni tah*/
   {kam=tah&127;
    odkud=tah>>7;
    TiskniNPoli(hwnd,odkud,kam,0);
	/*MessageBeep(-1);*/
    return;
 }
/*  Mala bila rosada*/
  if (tah==MBRoch)
   {TiskniNPoli(hwnd,e1,g1,f1,h1,0); return;}
  /*Velka bila rosada*/
  if (tah==VBRoch)
   {TiskniNPoli(hwnd,e1,d1,c1,a1,0); return;}
 /*Mala cerna rosada*/
  if (tah==MCRoch)
   {TiskniNPoli(hwnd,e8,g8,f8,h8,0); return;}
  /*Velka cerna rosada*/
  if (tah==VCRoch)
   {TiskniNPoli(hwnd,e8,d8,c8,a8,0); return;}
  /*Promena bileho pesce*/
 if ((tah>>12)==12)
  {
   TiskniNPoli(hwnd,a7+((tah>>7)&7),a8+((tah>>4)&7),0); return;
  }
  /*Promena cerneho pesce*/
 if ((tah>>12)==13)
  {
   TiskniNPoli(hwnd,a2+((tah>>7)&7),a1+((tah>>4)&7),0); return;
  }
 /* Brani mimochodem (nic jineho to uz byt nemuze)*/
 tah&=0x3fff; /* odstraneni prvnich dvou bitu, aby se lepe siftovalo*/
 kam=tah&127;
 odkud=tah>>7;
 TiskniNPoli(hwnd,odkud,kam,odkud<kam ? kam-10 : kam+10,0);
 return;
 }
 void ZobrazTahy(HWND hwnd, TUloha *uloha)
  {TTah1 *p,*k;
    NalezTahy(uloha);
    JenPripustne(uloha);
    p=&uloha->zasobnik.tahy[uloha->zasobnik.hranice[uloha->zasobnik.pos]];
    k=&uloha->zasobnik.tahy[uloha->zasobnik.hranice[uloha->zasobnik.pos+1]];
    UlozStav(uloha);
    while(p<k)
     {Tahni(p->data,uloha);
      WinTiskniTah(hwnd, p->data);
      Sleep(100);
      TahniZpet(p->data,uloha);
      WinTiskniTah(hwnd, p->data);
      Sleep(300);
      p++;
     }
  }
#endif
