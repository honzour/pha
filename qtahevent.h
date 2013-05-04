#ifndef qtaheventH
#define qtaheventH
#include <qevent.h>
#include "cotyvar.h"
class TahEvent : public QEvent{
 protected:
  u16 tah;
 public:
  u16 getTah(){return tah;}
  TahEvent(u16 tah):QEvent(QEvent::User){this->tah=tah;}
};
#endif
