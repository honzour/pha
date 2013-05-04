#ifndef sachovniceH
#define sachovniceH
#include <qobject.h>
#include <qpainter.h>
#include <qwidget.h>
#include <qpixmap.h>
#include <qevent.h>
#include <qmainwindow.h>
#include "volby.h"
#include "cotyvar.h"
class TSachovnice :public QObject{
 protected:
 QPixmap *pole, *figury;
 QMainWindow *okno;
 TPozice pos;//Na ni se kresli
 bool BilyNahoru, BilyClovek, CernyClovek;
 int ozn1, ozn2;
 public:
 TUloha *u;
 int x,y; // posun sachovnice v okne
 int poledx, poledy, // sirka pole
 		polex, poley; //sirka okraje sachovnice
 public:
 TSachovnice(int x=0, int y=0, QMainWindow *okno=NULL);
 void TahniAZobraz(u16 tah);
 virtual void mousePressEvent(QMouseEvent *e);
 virtual bool naSachovnici(QMouseEvent *e);
 virtual void zobrazPole(int i);
 virtual void paint();
 virtual void setPozice(TPozice *pos);
 virtual void otoc();
 virtual void promena();
 ~TSachovnice();
 protected:
 bool eventFilter(QObject *o, QEvent *e);
};
#endif
