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

class CKeysymMakerContext {
public:
  CKeysymMakerContext();
  ~CKeysymMakerContext();
  xkb_context* XkbContext_;
};

class CKeysymMakerTable: public CKeysymMakerContext {
public:
  CKeysymMakerTable();
  ~CKeysymMakerTable();
  xkb_compose_table* XkbComposeTable_;
};
class CKeysymMakerState: public CKeysymMakerTable {
public:
  CKeysymMakerState();
  ~CKeysymMakerState();
  xkb_compose_state* XkbComposeState_;
};

class CKeysymMaker: public CKeysymMakerState {
public:
  CKeysymMaker(XkbDescPtr);
  xkb_keysym_t feedEvent(XIDeviceEvent*);
  void resetState();
  xkb_keycode_t getKeycode(XIDeviceEvent*);
  xkb_keysym_t getPlainKeysym(XIDeviceEvent*);
  int getMod(XIDeviceEvent* DeviceEvent, int group_effective);
  int getGroup(XIDeviceEvent*);
  int getWidth(xkb_keycode_t);
  int getKt(xkb_keycode_t, int group_effective);
  int getShiftLevel(XIDeviceEvent*);
  int isLastDead = 0;
  // private:
  XkbDescPtr XkbDesc_;
};

} // namespace NSLinux
} // namespace NSKeyboard
} // namespace NSApplication

#endif // KEYSYMMAKER_H
