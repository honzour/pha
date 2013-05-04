#include "volby.h"
#if Typ_Produktu==Qt_Program
#include <qpopupmenu.h>
#include <qpainter.h>
#include <qmessagebox.h>
#include <stdio.h>
#include <qevent.h>
#include <qstatusbar.h>
#include <qmenubar.h>
#include <qmainwindow.h>
#include <qwidget.h>
#include "qvlakno.h"
#include "qpromena.h"
THlavniOkno::THlavniOkno(){
 setCaption(tr("Honza's Chess 2002"));
// Program
 QPopupMenu *program=new QPopupMenu(this);
 program->insertItem(tr("E&xit"),this,SLOT(konec()));

// Sachovnice
 QPopupMenu *sachovnice=new QPopupMenu(this);
 sachovnice->insertItem(tr("&Flip"),this,SLOT(otoc()));
 sachovnice->insertItem(tr("&Pesec"),this,SLOT(promena()));
// Tah
 QPopupMenu *tah=new QPopupMenu(this);
 tah->insertItem(tr("&Move, program"),this,SLOT(tahni()));

 menuBar()->insertItem(tr("&Program"),program);
 menuBar()->insertItem(tr("&Move"), tah);
 menuBar()->insertItem(tr("&Board"), sachovnice);
 statusBar()->message(tr("Ready"),2000);
 s=new TSachovnice(20,50,this);
}
THlavniOkno::~THlavniOkno(){
 delete s;
}
void THlavniOkno::paintEvent(QPaintEvent *e){
 QPainter *p=new QPainter(this);
 s->paint();
 delete p;
}
void THlavniOkno::mousePressEvent(QMouseEvent *e){
 if(s->naSachovnici(e)) s->mousePressEvent(e);
}
void THlavniOkno::konec(){
 close();
}
void THlavniOkno::promena(){
 s->promena();
}
void THlavniOkno::otoc(){
 s->otoc();
}
void THlavniOkno::tahni(){
 TVlaknoMysleni *vm=new TVlaknoMysleni(s);
 vm->start();
}
#endif

