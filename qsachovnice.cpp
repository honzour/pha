#include "volby.h"
#if Typ_Produktu==Qt_Program
#include <qpixmap.h>
#include <qpainter.h>
#include <qpaintdevice.h>
#include <qevent.h>
#include <qframe.h>
#include <stdlib.h>
#include <qmainwindow.h>
#include <qstatusbar.h>
#include "cotyvar.h"
#include "sachy.h"
#include "generato.h"
#include "globruti.h"
#include "qsachovnice.h"
#include "qtahevent.h"
#include "qtsachy.h"
#include "qpromena.h"
#include "pole.h"

TSachovnice::TSachovnice(int x, int y,QMainWindow *okno){
 BilyNahoru=true;
 polex=0, poley=0;
 poledx=40, poledy=40;
 this->x=x; this->y=y;
 this->okno=okno;
 ozn1=ozn2=0;
 BilyClovek=CernyClovek=true;
 pos=ZakladniPostaveni;
 InitProgram(&u,"knihovna");
 NalezTahy(u);
 JenPripustne(u);
 figury=new QPixmap(CestaKBin+QString("figury.bmp"));
 pole=new QPixmap(CestaKBin+QString("pole.bmp"));
 installEventFilter(this);
 }
TSachovnice::~TSachovnice(){
 DoneUlohu(&u);
 if(!!figury)delete figury;
 if(!!pole)delete pole;
}
bool TSachovnice::eventFilter(QObject *o, QEvent *e){
 if(e->type() ==
  QEvent::User){
  TahniAZobraz(((TahEvent *)e)->getTah());
  return TRUE;
 }
 return QObject::eventFilter(o,e);
}

void TSachovnice::setPozice(TPozice *pos){
 (this->pos)=*pos;
 paint();
}
void TSachovnice::otoc(){
 BilyNahoru=!BilyNahoru;
 paint();
}
void TSachovnice::promena(){
 TDialogPromeny *d;

 d=new TDialogPromeny();
 d->show();
}
bool TSachovnice::naSachovnici(QMouseEvent *e){
 int mx,my;
 mx=e->x(); my=e->y();
 return mx>=x && mx<x+2*polex+8*poledx && my>=y && my<y+2*poley+8*poledy;
}
void TSachovnice::mousePressEvent(QMouseEvent *e){
 int o1,o2,mx,my;
 if(!naSachovnici(e))return;
 if(e->button()!=QFrame::LeftButton)
 {okno->statusBar()->message(tr("Click with left mouse button"),2000);
  return;}
 if(pos.bily && !BilyClovek || !pos.bily &&!CernyClovek){
 okno->statusBar()->message(tr("Compute move"),2000);
  return;}
 mx=e->x()-x-polex;
 if(mx<0 || mx>=8*poledx)return;
 my=e->y()-y-poley;
 if(my<0 || my>=8*poledy)return;
 mx/=poledx;
 my/=poledy;
 if(BilyNahoru)o1=a1+mx+(7-my)*10;else o1=a1+7-mx+my*10;
 // kliknul na o1
 // kliknul na kamen vlastni barvy
 if(pos.sch[o1]<0 && !pos.bily || pos.sch[o1]>0 && pos.bily){
 if(!JeTam1(o1,u)){
  okno->statusBar()->message(tr("No move for this figure"),2000);
  return;
  }
  o2=ozn1; ozn1=o1;
  zobrazPole(o2);
  zobrazPole(o1);
  }
 else
 // kliknul na prazdne pole nebo kamen soupere
 {
  if(!ozn1)
  {okno->statusBar()->message(
  (pos.sch[o1]==0) ? tr("You can not move with empty field") :
  ((pos.sch[o1]<0)? tr("It is white's move") : tr("It is black's move"))
  ,5000);
   return;}
   else{ // zadava druhe policko tahu
    if(!JeTam2(ozn1,o1,u)){
     okno->statusBar()->message(tr("Illegal move"),2000);
     return;
    }
    else {
     u16 tah;
     tah=DoplnTah(ozn1,o1,u);
     ozn1=0;
     TahniAZobraz(tah);
    }
   }
 }
}
void TSachovnice::TahniAZobraz(u16 tah){
   GlobTahni(true, u, true, tah);
   pos=u->pozice;
   paint();
}
void TSachovnice::zobrazPole(int i){
 int a,b,z,px,posx,posy;

  if(i<a1||i>h8)return;
  a=(i-a1)%10;if(a>7) return;
  a=7-a;
  b=(i-a1)/10;
//  printf("[%i, %i, %i, %i]\n",a,b,a1,h8);
  z=(i==ozn1 || i==ozn2) ? 2*poledx : 0;
  px=((a+b)&1) ? poledx : 0;
  if(BilyNahoru)
{
  bitBlt (okno, x+polex+(7-a)*poledx, y+poley+(7-b)*poledy, pole, px+z,  0, poledx, poledy, Qt::CopyROP);
  posx=abs(pos.sch[i])-1;
  if(posx<0)return;
  posx*=36;
  posy=(pos.sch[i]<0 ? 72 : 36);
  bitBlt (okno, x+polex+(7-a)*poledx+2, y+poley+(7-b)*poledy+2, figury, posx, 0, 36, 36, Qt::AndROP);
  bitBlt (okno, x+polex+(7-a)*poledx+2, y+poley+(7-b)*poledy+2, figury, posx, posy, 36, 36, Qt::OrROP);
 }
else
{

  bitBlt (okno, x+polex+a*poledx, y+poley+b*poledy, pole, px+z,  0, poledx, poledy, Qt::CopyROP);
  posx=abs(pos.sch[i])-1;
  if(posx<0)return;
  posx*=36;
  posy=(pos.sch[/*a1+b*10+a*/i]<0 ? 72 : 36);
  bitBlt (okno, x+polex+a*poledx+2, y+poley+b*poledx+2, figury, posx, 0, 36, 36, Qt::AndROP);
  bitBlt (okno, x+polex+a*poledx+2, y+poley+b*poledx+2, figury, posx, posy, 36, 36, Qt::OrROP);
 }

}
void TSachovnice::paint(){
 int i;

 for(i=a1;i<=h8;i++)zobrazPole(i);
/*
 int a,b,posx,posy,px;
if(BilyNahoru)
for(a=0;a<=7;a++)
  for(b=0;b<=7;b++){
  px=((a+b)&1) ? 0 : 40;
  bitBlt (okno, x+(7-a)*40, y+(7-b)*40, pole, px,  0, 40, 40, Qt::CopyROP);
  posx=abs(pos.sch[a1+b*10+a])-1;
  if(posx<0)continue;
  posx*=36;
  posy=(pos.sch[a1+b*10+a]<0 ? 72 : 36);
  bitBlt (okno, x+(7-a)*40+2, y+(7-b)*40+2, figury, posx, 0, 36, 36, Qt::AndROP);
  bitBlt (okno, x+(7-a)*40+2, y+(7-b)*40+2, figury, posx, posy, 36, 36, Qt::OrROP);
 }
else
for(a=0;a<=7;a++)
  for(b=0;b<=7;b++){
  px=((a+b)&1) ? 0 : 40;
  bitBlt (okno, x+a*40, y+b*40, pole, px,  0, 40, 40, Qt::CopyROP);
  posx=abs(pos.sch[a1+b*10+a])-1;
  if(posx<0)continue;
  posx*=36;
  posy=(pos.sch[a1+b*10+a]<0 ? 72 : 36);
  bitBlt (okno, x+a*40+2, y+b*40+2, figury, posx, 0, 36, 36, Qt::AndROP);
  bitBlt (okno, x+a*40+2, y+b*40+2, figury, posx, posy, 36, 36, Qt::OrROP);
 }*/
}
#endif

