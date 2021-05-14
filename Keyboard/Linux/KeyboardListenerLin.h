#ifndef NSAPPLICATION_NSKEYBOARD_NSLINUX_CKEYBOARDLISTENERLIN_H
#define NSAPPLICATION_NSKEYBOARD_NSLINUX_CKEYBOARDLISTENERLIN_H

#include "Keyboard/AnyKeyboardKiller.h"

#include "KeysymMaker.h"
#include <QObject>
#include <QThread>
#include <future>
#include <iostream>
#include <string>

namespace NSApplication {
namespace NSKeyboard {

struct CKeyPressing;
struct CKeyReleasing;

class CKeyboardHandler;

namespace NSLinux {

class CKeyboardListenerLinImpl : public QObject {
  Q_OBJECT

  friend class CKiller;

public:
  using CAnyKillerPromise = std::promise<CAnyKeyboardKiller>;
  CKeyboardListenerLinImpl(CAnyKillerPromise, CKeyboardHandler*);
  ~CKeyboardListenerLinImpl();

signals:
  void KeyPressing(const CKeyPressing&);
  void KeyReleasing(const CKeyReleasing&);

public:
  int exec();

private:
  Display* X11Display_;
  XkbDescPtr XkbDesc_;
  int xi_opcode_;
  CKeysymMaker KeysymMaker_;

  int extractEventInfo(XGenericEventCookie*);
  int keyPressEvent(XGenericEventCookie*);
  int keyReleaseEvent(XGenericEventCookie*);
  XIDeviceEvent* getXIDeviceEvent(XGenericEventCookie*);
  xkb_keycode_t getKeycode(XIDeviceEvent*);
  QString makeTextFromKeysym(xkb_keysym_t);
  QChar getLabel(xkb_keysym_t);
  // Implementation details
};

// The object provides a way to shut down the listener
class CKiller {
public:
  CKiller() = default;
  // CKiller(...)
  void stopListener() const;

private:
  // Implementation details
};

} // namespace NSLinux
} // namespace NSKeyboard
} // namespace NSApplication

#endif // NSAPPLICATION_NSKEYBOARD_NSLINUX_CKEYBOARDLISTENERLIN_H
