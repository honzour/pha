#include "volby.h"
#if Typ_Produktu==Qt_Program
#include <qapplication.h>
#include <qpushbutton.h>
#include <qpainter.h>
#include <qwidget.h>
#include <qmenubar.h>
#include <qstatusbar.h>
#include <qmainwindow.h>
#include <qtranslator.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "volby.h"
#include "cotyvar.h"
#include "sachy.h"
#include "qhlokno.h"

char *CestaKBin;
THlavniOkno *HlavniOkno;
QApplication *Aplikace;

int main( int argc, char **argv )
{int i;

  CestaKBin=strdup(argv[0]);
  i=strlen(CestaKBin);
  for(i>0;CestaKBin[i]!='/' && CestaKBin[i]!='\\';i--);
  CestaKBin[++i]=0;
//  puts(CestaKBin);
  Aplikace=new QApplication( argc, argv );
  QTranslator t(0);
  t.load("../../s.qm",CestaKBin);
  Aplikace->installTranslator(&t);
  HlavniOkno=new THlavniOkno();
  Aplikace->setMainWidget(HlavniOkno);
  HlavniOkno->showMaximized();
  i=Aplikace->exec();
  delete Aplikace;
  free(CestaKBin);
  return i;
}
#endif

