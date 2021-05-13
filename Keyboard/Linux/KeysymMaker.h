#ifndef KEYSYMMAKER_H
#define KEYSYMMAKER_H

#include "x11includes.h"
#include <QObject>
#include <QString>
#include <QThread>
#include <locale.h>
#include <optional>

namespace NSApplication {
namespace NSKeyboard {
namespace NSLinux {

class CKeysymMaker {
public:
  CKeysymMaker();
  CKeysymMaker(XkbDescPtr);
  CKeysymMaker& operator=(CKeysymMaker&);
  CKeysymMaker& operator=(CKeysymMaker&&);
  ~CKeysymMaker();
  xkb_keysym_t feedEvent(XIDeviceEvent*);
  xkb_keysym_t simpleKeysym(XIDeviceEvent*);

  // private:
  struct xkb_context* XkbContext = 0;
  struct xkb_compose_table* XkbComposeTable = 0;
  struct xkb_compose_state* XkbComposeState = 0;
  XkbDescPtr XkbDesc = 0;
};

} // namespace NSLinux
} // namespace NSKeyboard
} // namespace NSApplication

#endif // KEYSYMMAKER_H
