#ifndef KEYSYMMAKER_H
#define KEYSYMMAKER_H

#include "x11includes.h"
#include <clocale>

namespace NSApplication {
namespace NSKeyboard {
namespace NSLinux {
class CKeysymMakerContext {
public:
  CKeysymMakerContext();
  ~CKeysymMakerContext();

protected:
  xkb_context* XkbContext_;
};

class CKeysymMakerTable : protected CKeysymMakerContext {
public:
  CKeysymMakerTable();
  ~CKeysymMakerTable();

protected:
  xkb_compose_table* XkbComposeTable_;
};

class CKeysymMakerState : protected CKeysymMakerTable {
public:
  CKeysymMakerState();
  ~CKeysymMakerState();

  xkb_compose_state* XkbComposeState_;
};

class CKeysymMaker : protected CKeysymMakerState {
public:
  CKeysymMaker(XkbDescPtr);
  void resetState();
  xkb_keysym_t getPlainKeysym(XIDeviceEvent*) const;
  xkb_keysym_t feedEvent(XIDeviceEvent*);
  xkb_keysym_t feedKeysym(xkb_keysym_t);
  int isLastDead() const;

private:
  int LastKeysym_;
  int isLastDead_;
  XkbDescPtr XkbDesc_;
  int getMod(XIDeviceEvent* DeviceEvent, int group_effective) const;
  int getGroup(XIDeviceEvent*) const;
  int getWidth(xkb_keycode_t) const;
  int getKt(xkb_keycode_t, int group_effective) const;
  xkb_keycode_t getKeycode(XIDeviceEvent*) const;
  int getShiftLevel(XIDeviceEvent*) const;
};

} // namespace NSLinux
} // namespace NSKeyboard
} // namespace NSApplication

#endif // KEYSYMMAKER_H
