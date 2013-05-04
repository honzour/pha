#ifndef qvlaknoH
#define qvlaknoH
#include <qthread.h>
#include "qhlokno.h"

class TVlaknoMysleni : public QThread{
public:
 TSachovnice *s;
 TVlaknoMysleni(TSachovnice *s);
 virtual void run();
// ~TVlaknoMysleni(){};
};
#endif
