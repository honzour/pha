#ifndef hloknoH
#define hloknoH
#include <qwidget.h>
#include <qmenubar.h>
#include <qevent.h>
#include <qstatusbar.h>
#include <qmainwindow.h>
#include "qsachovnice.h"
 class THlavniOkno:public QMainWindow{
 Q_OBJECT
 public:
 TSachovnice *s;
 public:
 void paintEvent(QPaintEvent *e);
 public slots:
// reakce na menu
 void konec();
 void otoc();
 void tahni();
 void promena();
 public:
 THlavniOkno();
 ~THlavniOkno();
 protected:
 virtual void mousePressEvent(QMouseEvent *e);
};
#endif
