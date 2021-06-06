#ifndef NSAPPLICATION_NSKEYBOARD_NSLINUX_CKEYBOARDLISTENERLIN_H
#define NSAPPLICATION_NSKEYBOARD_NSLINUX_CKEYBOARDLISTENERLIN_H

#include "Keyboard/AnyKeyboardKiller.h"
#include "KeysymMaker.h"
#include "TimerAccess.h"
#include "Keyboard/KeyboardHandler.h"
#include "KeyPositionLin.h"
#include <QObject>
#include <future>
#include <iostream>
#include <string.h>
#include <unistd.h>

namespace NSApplication {
namespace NSKeyboard {

struct CKeyPressing;
struct CKeyReleasing;

class CKeyboardHandler;

namespace NSLinux {

class CKeyboardListenerImplDisplay {
public:
  CKeyboardListenerImplDisplay();
  ~CKeyboardListenerImplDisplay();
protected:
  Display* X11Display_;
};

class CKeyboardListenerImplDesc : public CKeyboardListenerImplDisplay {
public:
  CKeyboardListenerImplDesc();
  ~CKeyboardListenerImplDesc();
protected:
  XkbDescPtr XkbDesc_;
};

class CKeyboardListenerLinImpl : public QObject,
                                 public CKeyboardListenerImplDesc {
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
  Window MessageWindow_;
  CKeysymMaker KeysymMaker_;
  CKeysymMaker DeadLabelMaker_;
  CKeyPositionLin PositionMaker_;
  int handleKeyPress(XGenericEventCookie*);
  int handleKeyRelease(XGenericEventCookie*);
  int isInteruptionRequested(XEvent&) const;
  XIDeviceEvent* getXIDeviceEvent(XGenericEventCookie*) const;
  xkb_keycode_t getKeycode(XIDeviceEvent*) const;
  QString makeTextFromKeysym(xkb_keysym_t) const;
  int isLastDead() const;
  QChar getLabel(xkb_keysym_t);
  // Implementation details
};

// The object provides a way to shut down the listener
class CKiller {
  Display* X11Display_;
  Window MessageWindow;

public:
  CKiller(Display*, Window);
  // CKiller(...)
  void stopListener() const;
  XEvent makeClientMessageEvent(const char*) const;

private:
  // Implementation details
};

} // namespace NSLinux
} // namespace NSKeyboard
} // namespace NSApplication

#endif // NSAPPLICATION_NSKEYBOARD_NSLINUX_CKEYBOARDLISTENERLIN_H
