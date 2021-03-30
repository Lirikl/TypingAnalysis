#include "KeyboardListenerLin.h"
#include <xkbcommon/xkbcommon.h>

#include "Keyboard/KeyboardHandler.h"

namespace NSApplication {
namespace NSKeyboard {
namespace NSLinux {

CKeyboardListenerLinImpl::CKeyboardListenerLinImpl(
    CAnyKillerPromise killerPromise, CKeyboardHandler* KeyboardHandler) {
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
  X11EventMask_.deviceid = XIAllDvieces;
  X11EventMask_.mask_len = XIMaskLen(XI_LASTEVENT);
  std::vector<unsigned char> safeArray(X11EventMask_.mask_len);
  X11EventMask_.mask = safeArray.data();
  std::fill(X11EventMask_.mask, X11EventMask_.mask + X11EventMask_.mask_len, 0);
  XISetMask(X11EventMask_.mask, XI_KeyPress); // maybe it should me moved from constructor into Do?
  XISetMask(X11EventMask_.mask, XI_KeyRelease);
  XISelectEvents(X11Display_, X11DefaultWindow, &X11EventMask_, 1);
  XSync(X11Display_, false);
  XkbDesc  = XkbGetKeyboard(X11Display_, XkbAllComponentsMask
                                                    , XkbUseCoreKbd);
  // TO DO
  // Set killerPromise to a non-trivial one
  killerPromise.set_value(CKiller());

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
    extractEventInfo(X11CurrentEventCookie);
  }

  return 0;
}

int CKeyboardListenerLinImpl::extractEventInfo(XGenericEventCookie *X11CurrentEventCookie) {
  auto X11CurrentDeviceEvent = static_cast<XIDeviceEvent*>(X11CurrentEventCookie->data);
  auto time = X11CurrentDeviceEvent->time;
  int effective_group = X11CurrentDeviceEvent->group.effective;
  if (effective_group >= (int)XkbDesc->map->key_sym_map[X11CurrentDeviceEvent->detail].group_info)
    effective_group = (int)XkbDesc->map->key_sym_map[X11CurrentDeviceEvent->detail].group_info - 1;
  int width = (int)XkbDesc->map->key_sym_map[X11CurrentDeviceEvent->detail].width;
  int effective_mods = X11CurrentDeviceEvent->mods.effective;
  int kt = (int)XkbDesc->map->key_sym_map[X11CurrentDeviceEvent->detail].kt_index[effective_group];
  int shift_level = 0;
  effective_mods = effective_mods & XkbDesc->map->types[kt].mods.mask;
  for (int i = 0; i < XkbDesc->map->types[kt].map_count; i++) {
    if (XkbDesc->map->types[kt].map[i].mods.mask == effective_mods) {
      shift_level = XkbDesc->map->types[kt].map[i].level;
      break;
    }
   }
  if (X11CurrentEventCookie->evtype == XI_KeyPress) {

    //   std::cout << "KeyPressed, Symbol: " <<
      //              XKeysymToString(XkbDesc->map->syms[effective_group * width + shift_level +
       //                    (int)XkbDesc->map->key_sym_map[X11CurrentDeviceEvent->detail].offset]) << std::endl;
   }
   if (X11CurrentEventCookie->evtype == XI_KeyRelease) {
     //  std::cout << "KeyReleased, Symbol: " <<
//                        XKeysymToString(XkbDesc->map->syms[effective_group * width + shift_level +
     //               (int)XkbDesc->map->key_sym_map[X11CurrentDeviceEvent->detail].offset]) << std::endl;
   }
   XFreeEventData(X11Display_, X11CurrentEventCookie);
   return 0;
}
// TO DO
// a specific ctor of CKiller
// CKiller::CKiller(...) {
//
//}

void CKiller::stopListener() const {
  // TO DO
  // Implementation details
  // Send message to CKeyboardListenerLinImpl to stop listening
}

} // namespace NSLinux
} // namespace NSKeyboard
} // namespace NSApplication
