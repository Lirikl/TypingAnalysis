#ifndef NSAPPLICATION_NSKEYBOARD_NSLINUX_CKEYBOARDLISTENERLIN_H
#define NSAPPLICATION_NSKEYBOARD_NSLINUX_CKEYBOARDLISTENERLIN_H

#include "Keyboard/AnyKeyboardKiller.h"

#include <future>
#include <QObject>
#include <QThread>
#include <locale.h>
#include <X11/XKBlib.h>
#include <X11/extensions/XInput2.h>
#include <X11/keysym.h>
#include <X11/Xlib.h>

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
  Display *X11Display_;
  XkbDescPtr XkbDesc;
  int xi_opcode_;
  int extractEventInfo(XGenericEventCookie *);
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

} // NSLinux
} // NSKeyboard
} // NSApplication

#endif // NSAPPLICATION_NSKEYBOARD_NSLINUX_CKEYBOARDLISTENERLIN_H
