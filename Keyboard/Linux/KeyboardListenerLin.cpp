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
}

int CKeyboardListenerLinImpl::exec() {
  // TO DO
  // Message loop
  return 0;
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
