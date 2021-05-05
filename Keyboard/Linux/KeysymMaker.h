#ifndef KEYSYMMAKER_H
#define KEYSYMMAKER_H


#include <QString>
#include <optional>
#include <QObject>
#include <QThread>
#include <locale.h>
#include <xkbcommon/xkbcommon.h>
#include <xkbcommon/xkbcommon-compose.h>
#include <X11/XKBlib.h>
#include <X11/extensions/XInput2.h>
#include <X11/keysym.h>
#include <X11/Xlib.h>

namespace NSApplication {
namespace NSKeyboard {
namespace NSLinux {


class CKeysymMaker {
public:
  CKeysymMaker();
  CKeysymMaker(XkbDescPtr);
  CKeysymMaker& operator = (CKeysymMaker&);
  CKeysymMaker& operator = (CKeysymMaker&&);
  ~CKeysymMaker();
  std::optional<xkb_keysym_t>feedEvent(XIDeviceEvent*);

//private:
  struct xkb_context* XkbContext = 0;
  struct xkb_compose_table* XkbComposeTable = 0;
  struct xkb_compose_state* XkbComposeState = 0;
  XkbDescPtr XkbDesc = 0;
};

} // NSLinux
} // NSKeyboard
} // NSApplication


#endif // KEYSYMMAKER_H
