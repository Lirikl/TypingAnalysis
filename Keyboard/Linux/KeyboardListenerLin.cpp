#include "KeyboardListenerLin.h"

#include "Keyboard/KeyboardHandler.h"


namespace NSApplication {
namespace NSKeyboard {
namespace NSLinux {

CKeyboardListenerLinImpl::CKeyboardListenerLinImpl(
  CAnyKillerPromise killerPromise,
  CKeyboardHandler* KeyboardHandler) {
  // Set the Listener
  X11Display_ = XOpenDisplay(nullptr);
  if (X11Display_ == nullptr) {
      throw std::runtime_error("Cannot open display");
  }
  int queryEvent, queryError;
  if (! XQueryExtension(X11Display_, "XInputExtension", &xi_opcode_, &queryEvent, &queryError)) {
      // need to chage way of using exeptions
      throw std::runtime_error("X Input extension not available\n");
  }
  Window X11DefaultWindow = DefaultRootWindow(X11Display_);
  XIEventMask X11EventMask_;
  X11EventMask_.deviceid = XIAllDevices;
  X11EventMask_.mask_len = XIMaskLen(XI_LASTEVENT);
  std::vector<unsigned char> safeArray(X11EventMask_.mask_len);
  X11EventMask_.mask = safeArray.data();
  std::fill(X11EventMask_.mask, X11EventMask_.mask + X11EventMask_.mask_len, 0);
  XISetMask(X11EventMask_.mask, XI_KeyPress); // maybe it should me moved from constructor into Do?
  XISetMask(X11EventMask_.mask, XI_KeyRelease);
  XISelectEvents(X11Display_, X11DefaultWindow, &X11EventMask_, 1);
  XSync(X11Display_, false);
  XkbDesc  = XkbGetKeyboard(X11Display_, XkbAllComponentsMask, XkbUseCoreKbd);
  //const char* locale;
  //locale = setlocale(LC_ALL,"");
  //XkbContext =  xkb_context_new(XKB_CONTEXT_NO_FLAGS);
  //XkbComposeTable = xkb_compose_table_new_from_locale(XkbContext, locale, XKB_COMPOSE_COMPILE_NO_FLAGS);
  //XkbComposeState = xkb_compose_state_new(XkbComposeTable, XKB_COMPOSE_STATE_NO_FLAGS);

  // TO DO
  // Set killerPromise to a non-trivial one
  killerPromise.set_value(CKiller());

  connect(this, &CKeyboardListenerLinImpl::KeyPressing,
          KeyboardHandler, &CKeyboardHandler::onKeyPressing,
          Qt::ConnectionType::QueuedConnection);
  connect(this, &CKeyboardListenerLinImpl::KeyReleasing,
          KeyboardHandler, &CKeyboardHandler::onKeyReleasing,
          Qt::ConnectionType::QueuedConnection);
}

CKeyboardListenerLinImpl::~CKeyboardListenerLinImpl() {
  disconnect(this, &CKeyboardListenerLinImpl::KeyPressing, nullptr, nullptr);
  disconnect(this, &CKeyboardListenerLinImpl::KeyReleasing, nullptr, nullptr);
  XkbFreeKeyboard(XkbDesc, XkbAllComponentsMask, 1);
}

int CKeyboardListenerLinImpl::exec() {
  // TO DO
  // Message loop
  XEvent X11CurrentEvent;
  XGenericEventCookie *X11CurrentEventCookie = &X11CurrentEvent.xcookie;
 //while (!myThread_->isInterruptionRequested()) {
  while (1) {
    XNextEvent(X11Display_, &X11CurrentEvent);
    if (!XGetEventData(X11Display_, X11CurrentEventCookie)
      || X11CurrentEventCookie->extension != xi_opcode_) {
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

int CKeyboardListenerLinImpl::extractEventInfo(XGenericEventCookie *X11CurrentEventCookie) {
   return 0;
}

int CKeyboardListenerLinImpl::keyPressEvent(XGenericEventCookie *X11CurrentEventCookie) {
  auto X11CurrentDeviceEvent = static_cast<XIDeviceEvent*>(X11CurrentEventCookie->data); 
  auto keysym = KeysymMaker_.feedEvent(X11CurrentDeviceEvent);
  Time cur_time = X11CurrentDeviceEvent->time;
  QString qstr;
  if (keysym.has_value) {
    char result_string[10];
    int result_string_len = xkb_keysym_to_utf8(keysym.value(), result_string, 10);
    QString::fromUtf8(result_string, result_string_len);
  }
  return 0;
}

int CKeyboardListenerLinImpl::keyReleaseEvent(XGenericEventCookie *X11CurrentEventCookie) {
  auto X11CurrentDeviceEvent = static_cast<XIDeviceEvent*>(X11CurrentEventCookie->data);
  auto keysym = KeysymMaker_.feedEvent(X11CurrentDeviceEvent);
  Time cur_time = X11CurrentDeviceEvent->time;

}
// TO DO
// a specific ctor of CKiller
//CKiller::CKiller(...) {
//
//}

void CKiller::stopListener() const {
  // TO DO
  // Implementation details
  // Send message to CKeyboardListenerLinImpl to stop listening
}

} // NSLinux
} // NSKeyboard
} // NSApplication
