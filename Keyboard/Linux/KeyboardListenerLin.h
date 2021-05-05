#ifndef NSAPPLICATION_NSKEYBOARD_NSLINUX_CKEYBOARDLISTENERLIN_H
#define NSAPPLICATION_NSKEYBOARD_NSLINUX_CKEYBOARDLISTENERLIN_H

#include "Keyboard/AnyKeyboardKiller.h"



#include <iostream>
#include <future>
#include <QObject>
#include <QThread>
#include "KeysymMaker.h"

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
  CKeysymMaker KeysymMaker_;


  int extractEventInfo(XGenericEventCookie*);
  int keyPressEvent(XGenericEventCookie*);
  int keyReleaseEvent(XGenericEventCookie*);

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
