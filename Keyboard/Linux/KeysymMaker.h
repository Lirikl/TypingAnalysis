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
  xkb_context* XkbContext_;
};

class CKeysymMakerTable : public CKeysymMakerContext {
public:
  CKeysymMakerTable();
  ~CKeysymMakerTable();
  xkb_compose_table* XkbComposeTable_;
};
class CKeysymMakerState : public CKeysymMakerTable {
public:
  CKeysymMakerState();
  ~CKeysymMakerState();
  xkb_compose_state* XkbComposeState_;
};

class CKeysymMaker : public CKeysymMakerState {
public:
  CKeysymMaker(XkbDescPtr);
  void resetState();
  xkb_keysym_t getPlainKeysym(XIDeviceEvent*) const;
  xkb_keysym_t feedEvent(XIDeviceEvent*);
  xkb_keysym_t feedKeysym(xkb_keysym_t);
  int LastKeysym_;
  int isLastDead_;
  XkbDescPtr XkbDesc_;

private:
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
