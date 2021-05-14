#include "KeyboardListenerLin.h"
#include "Keyboard/KeyboardHandler.h"
#include "TimerAccess.h"
#include <iostream>

namespace NSApplication {
namespace NSKeyboard {
namespace NSLinux {

CKeyboardListenerImplDisplay::CKeyboardListenerImplDisplay()
    : X11Display_(XOpenDisplay(nullptr)) {
}

CKeyboardListenerImplDisplay::~CKeyboardListenerImplDisplay() {
  XCloseDisplay(X11Display_);
}

CKeyboardListenerImplDesc::CKeyboardListenerImplDesc()
    : XkbDesc_(
          XkbGetKeyboard(X11Display_, XkbAllComponentsMask, XkbUseCoreKbd)) {
}

CKeyboardListenerImplDesc::~CKeyboardListenerImplDesc() {
  XkbFreeKeyboard(XkbDesc_, XkbAllComponentsMask, 1);
}
CKeyboardListenerLinImpl::CKeyboardListenerLinImpl(
    CAnyKillerPromise killerPromise, CKeyboardHandler* KeyboardHandler)
    : KeysymMaker_(XkbDesc_),
      killer_flag_(std::make_shared<int>(0)) {
  // Set the Listener

  Window X11DefaultWindow = DefaultRootWindow(X11Display_);
  XIEventMask X11EventMask_;
  X11EventMask_.deviceid = XIAllDevices;
  X11EventMask_.mask_len = XIMaskLen(XI_LASTEVENT);
  std::vector<unsigned char> safeArray(X11EventMask_.mask_len);
  X11EventMask_.mask = safeArray.data();
  std::fill(X11EventMask_.mask, X11EventMask_.mask + X11EventMask_.mask_len, 0);
  XISetMask(X11EventMask_.mask,
            XI_KeyPress); // maybe it should me moved from constructor into Do?
  XISetMask(X11EventMask_.mask, XI_KeyRelease);
  XISelectEvents(X11Display_, X11DefaultWindow, &X11EventMask_, 1);
  XSync(X11Display_, false);
  // KeysymMaker_ = CKeysymMaker(XkbDesc_);
  // TO DO
  // Set killerPromise to a non-trivial one
  killerPromise.set_value(CKiller(killer_flag_));

  connect(this, &CKeyboardListenerLinImpl::KeyPressing, KeyboardHandler,
          &CKeyboardHandler::onKeyPressing,
          Qt::ConnectionType::QueuedConnection);
  connect(this, &CKeyboardListenerLinImpl::KeyReleasing, KeyboardHandler,
          &CKeyboardHandler::onKeyReleasing,
          Qt::ConnectionType::QueuedConnection);
}

CKeyboardListenerLinImpl::~CKeyboardListenerLinImpl() {
  disconnect(this, &CKeyboardListenerLinImpl::KeyPressing, nullptr, nullptr);
  disconnect(this, &CKeyboardListenerLinImpl::KeyReleasing, nullptr, nullptr);
}
int CKeyboardListenerLinImpl::isInteruptionRequested() {
  return !*killer_flag_;
}

int CKeyboardListenerLinImpl::exec() {
  // TO DO
  // Message loop
  XEvent X11CurrentEvent;
  XGenericEventCookie* X11CurrentEventCookie = &X11CurrentEvent.xcookie;
  // havent found any way to send message
  while (isInteruptionRequested()) {
    XNextEvent(X11Display_, &X11CurrentEvent);
    if (!XGetEventData(X11Display_, X11CurrentEventCookie)) {
      continue;
    }
    if (X11CurrentEventCookie->evtype == XI_KeyPress) {
      keyPressEvent(X11CurrentEventCookie);
    }
    if (X11CurrentEventCookie->evtype == XI_KeyRelease) {
      keyReleaseEvent(X11CurrentEventCookie);
    }
    XFreeEventData(X11Display_, X11CurrentEventCookie);
  }
  return 0;
}

int CKeyboardListenerLinImpl::keyPressEvent(
    XGenericEventCookie* X11CurrentEventCookie) {
  CKeyPressing key_press = {};
  CTimerAccess Timer;
  key_press.Time = Timer->get();
  XIDeviceEvent* X11CurrentDeviceEvent =
      getXIDeviceEvent(X11CurrentEventCookie);
  xkb_keysym_t keysym = KeysymMaker_.feedEvent(X11CurrentDeviceEvent);
  key_press.KeyText = makeTextFromKeysym(keysym);
  key_press.KeyPosition = getKeycode(X11CurrentDeviceEvent);
  key_press.KeyLabel = getLabel(keysym);
  emit KeyPressing(key_press);
  return 0;
}

int CKeyboardListenerLinImpl::keyReleaseEvent(
    XGenericEventCookie* X11CurrentEventCookie) {
  CKeyReleasing key_release = {};
  CTimerAccess Timer;
  key_release.Time = Timer->get();
  XIDeviceEvent* X11CurrentDeviceEvent =
      getXIDeviceEvent(X11CurrentEventCookie);
  key_release.KeyPosition = getKeycode(X11CurrentDeviceEvent);
  emit KeyReleasing(key_release);
  return 0;
}

XIDeviceEvent* CKeyboardListenerLinImpl::getXIDeviceEvent(
    XGenericEventCookie* X11CurrentEventCookie) {
  return static_cast<XIDeviceEvent*>(X11CurrentEventCookie->data);
}

QString CKeyboardListenerLinImpl::makeTextFromKeysym(xkb_keysym_t keysym) {
  char result_string[33] = "";
  xkb_keysym_to_utf8(keysym, result_string, 33);
  return QString::fromUtf8(result_string, -1);
}

xkb_keycode_t
CKeyboardListenerLinImpl::getKeycode(XIDeviceEvent* X11CurrentDeviceEvent) {
  return X11CurrentDeviceEvent->detail;
}

int CKeyboardListenerLinImpl::isLastDead() {
  return KeysymMaker_.isLastDead;
}

QChar CKeyboardListenerLinImpl::getLabel(xkb_keysym_t keysym) {
  if (isLastDead()) {
    return QChar(0x2620);
  }
  if (std::string(XKeysymToString(keysym)) == "Return")
    return QChar(0x2ba0);
  if (std::string(XKeysymToString(keysym)) == "Control_L")
    return QChar(0x2343);
  if (std::string(XKeysymToString(keysym)) == "Control_R")
    return QChar(0x2344);
  if (std::string(XKeysymToString(keysym)) == "Alt_L")
    return QChar(0x2347);
  if (std::string(XKeysymToString(keysym)) == "Alt_R")
    return QChar(0x2348);
  if (std::string(XKeysymToString(keysym)) == "Shift_L")
    return QChar(0x2B01);
  if (std::string(XKeysymToString(keysym)) == "Shift_R")
    return QChar(0x2B00);
  if (std::string(XKeysymToString(keysym)) == "BackSpace")
    return QChar(0x232B);
  if (std::string(XKeysymToString(keysym)) == "Caps_Lock")
    return QChar(0x2B89);
  if (std::string(XKeysymToString(keysym)) == "Tab")
    return QChar(0x2b7e);
  if (std::string(XKeysymToString(keysym)) == "Escape")
    return QChar(0x2bbe);
  QString str = makeTextFromKeysym(keysym);
  if (str.size() == 0 && str[0].isPrint()) {
    return QChar();
  }
  return str[0];
}

// TO DO
// a specific ctor of CKiller
// CKiller::CKiller(...) {
//
//}
CKiller::CKiller(std::shared_ptr<int> spt) : killer_flag_(spt) {
}

void CKiller::stopListener() const {
  if (auto spt = killer_flag_.lock()) {
    *spt = 1;
  }
  // the only one modification of CKeyListenerLin.killer_flag_, so should be no
  // race condition
}

} // namespace NSLinux
} // namespace NSKeyboard
} // namespace NSApplication
