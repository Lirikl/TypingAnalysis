#include "KeyboardListenerLin.h"

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
    : MessageWindow_(XCreateSimpleWindow(
          X11Display_, DefaultRootWindow(X11Display_), 0, 0, 1, 1, 0, 0, 0)),
      KeysymMaker_(XkbDesc_), DeadLabelMaker_(XkbDesc_) {

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

  // KeysymMaker_ = CKeysymMaker(XkbDesc_);
  // TO DO
  // Set killerPromise to a non-trivial one
  killerPromise.set_value(CKiller(X11Display_, MessageWindow_));

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
  XDestroyWindow(X11Display_, MessageWindow_);
}

int CKeyboardListenerLinImpl::isInteruptionRequested(XEvent& ev) const {
  return (ev.type == ClientMessage) && !strcmp(ev.xclient.data.b, "kill");
}

int CKeyboardListenerLinImpl::exec(){
  // TO DO
  // Message loop
  XEvent X11CurrentEvent;
  XGenericEventCookie* X11CurrentEventCookie = &X11CurrentEvent.xcookie;
  while (1) {
    XNextEvent(X11Display_, &X11CurrentEvent);
    if (isInteruptionRequested(X11CurrentEvent)) {
      break;
    }

    if (!XGetEventData(X11Display_, X11CurrentEventCookie)) {
      continue;
    }

    switch (X11CurrentEventCookie->evtype) {
    case XI_KeyPress:
      handleKeyPress(X11CurrentEventCookie);
      break;
    case XI_KeyRelease:
      handleKeyRelease(X11CurrentEventCookie);
      break;
    default:
      break;
    }
    XFreeEventData(X11Display_, X11CurrentEventCookie);
  }
  return 0;
}

int CKeyboardListenerLinImpl::handleKeyPress(
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

int CKeyboardListenerLinImpl::handleKeyRelease(
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
    XGenericEventCookie* X11CurrentEventCookie) const{
  return static_cast<XIDeviceEvent*>(X11CurrentEventCookie->data);
}

QString CKeyboardListenerLinImpl::makeTextFromKeysym(xkb_keysym_t keysym) const {
  char result_string[33] = "";
  xkb_keysym_to_utf8(keysym, result_string, 33);
  return QString::fromUtf8(result_string, -1);
}

xkb_keycode_t
CKeyboardListenerLinImpl::getKeycode(XIDeviceEvent* X11CurrentDeviceEvent) const {
  return X11CurrentDeviceEvent->detail;
}

int CKeyboardListenerLinImpl::isLastDead() const {
  return KeysymMaker_.isLastDead_;
}

QChar CKeyboardListenerLinImpl::getLabel(xkb_keysym_t keysym){
  if (isLastDead()) {
    DeadLabelMaker_.resetState();
    DeadLabelMaker_.feedKeysym(keysym);
    keysym = DeadLabelMaker_.feedKeysym(keysym);
    QString str = makeTextFromKeysym(keysym);
    if (str.size() == 0 && str[0].isPrint()) {
      return QChar();
    }
    return str[0];
    // return QChar(0x2620);
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

XEvent CKiller::makeClientMessageEvent(const char* text) const {
  XEvent evt;
  evt.xclient.type = ClientMessage;
  evt.xclient.serial = 0;
  evt.xclient.send_event = 1;
  evt.xclient.format = 32;
  evt.xclient.window = MessageWindow;
  strncpy(evt.xclient.data.b, text, sizeof(evt.xclient.data.b));
  return evt;
}

CKiller::CKiller(Display* dpy, Window wnd)
    : X11Display_(dpy), MessageWindow(wnd) {
}

void CKiller::stopListener() const {
  XEvent evt = makeClientMessageEvent("killer");
  XSendEvent(X11Display_, MessageWindow, false, NoEventMask, &evt);
}

} // namespace NSLinux
} // namespace NSKeyboard
} // namespace NSApplication
