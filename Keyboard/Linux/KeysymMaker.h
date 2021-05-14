#ifndef KEYSYMMAKER_H
#define KEYSYMMAKER_H

#include "x11includes.h"
#include <QObject>
#include <QString>
#include <QThread>
#include <clocale>
#include <optional>

namespace NSApplication {
namespace NSKeyboard {
namespace NSLinux {

class CKeysymMaker {
public:
  CKeysymMaker();
  CKeysymMaker(XkbDescPtr);
  CKeysymMaker(CKeysymMaker&);
  CKeysymMaker& operator=(CKeysymMaker&);
  CKeysymMaker& operator=(CKeysymMaker&&);
  ~CKeysymMaker();
  xkb_keycode_t getKeycode(XIDeviceEvent*);
  xkb_keysym_t feedEvent(XIDeviceEvent*);
  xkb_keysym_t getPlainKeysym(XIDeviceEvent*);
  int getMod(XIDeviceEvent* DeviceEvent, int group_effective);
  int getGroup(XIDeviceEvent*);
  int getWidth(xkb_keycode_t);
  int getKt(xkb_keycode_t, int group_effective);
  int getShiftLevel(XIDeviceEvent*);
  // private:
  xkb_context* XkbContext_ = 0;
  xkb_compose_table* XkbComposeTable_ = 0;
  xkb_compose_state* XkbComposeState_ = 0;
  XkbDescPtr XkbDesc_ = 0;
};

} // namespace NSLinux
} // namespace NSKeyboard
} // namespace NSApplication

#endif // KEYSYMMAKER_H
