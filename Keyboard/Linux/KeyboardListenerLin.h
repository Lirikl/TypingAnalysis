#ifndef NSAPPLICATION_NSKEYBOARD_NSLINUX_CKEYBOARDLISTENERLIN_H
#define NSAPPLICATION_NSKEYBOARD_NSLINUX_CKEYBOARDLISTENERLIN_H

#include "Keyboard/AnyKeyboardKiller.h"

#include "KeysymMaker.h"
#include <QObject>
#include <QThread>
#include <future>
#include <iostream>
#include <memory>
#include <string>

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
  Display* X11Display_;
};

class CKeyboardListenerImplDesc : public CKeyboardListenerImplDisplay {
public:
  CKeyboardListenerImplDesc();
  ~CKeyboardListenerImplDesc();
  XkbDescPtr XkbDesc_;
};

class CKeyboardListenerLinImpl : public QObject, public CKeyboardListenerImplDesc {
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
  CKeysymMaker KeysymMaker_;
  CKeysymMaker DeadLabelMaker_;
  std::shared_ptr<int> killer_flag_;
  int handleKeyPress(XGenericEventCookie*);
  int handleKeyRelease(XGenericEventCookie*);
  int isInteruptionRequested();
  XIDeviceEvent* getXIDeviceEvent(XGenericEventCookie*);
  xkb_keycode_t getKeycode(XIDeviceEvent*);
  QString makeTextFromKeysym(xkb_keysym_t);
  QChar getLabel(xkb_keysym_t);
  int isLastDead();
  xkb_keysym_t getLastKeysym();
  // Implementation details
};

// The object provides a way to shut down the listener
class CKiller {
  std::weak_ptr<int> killer_flag_;

public:
  CKiller(std::shared_ptr<int>);
  // CKiller(...)
  void stopListener() const;

private:
  // Implementation details
};

} // namespace NSLinux
} // namespace NSKeyboard
} // namespace NSApplication

#endif // NSAPPLICATION_NSKEYBOARD_NSLINUX_CKEYBOARDLISTENERLIN_H
