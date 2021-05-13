#include "KeysymMaker.h"
#include <iostream>

namespace NSApplication {
namespace NSKeyboard {
namespace NSLinux {

CKeysymMaker::CKeysymMaker() {
}
CKeysymMaker::CKeysymMaker(XkbDescPtr XkbDesc) {
  XkbDesc_ = XkbDesc;

  const char* locale;
  locale = setlocale(LC_ALL,
                     "ru_RU.UTF-8"); // for debug because it makes broken locale
  // locale = setlocale(LC_ALL, ""); //for release
  XkbContext_ = xkb_context_new(XKB_CONTEXT_NO_FLAGS);
  XkbComposeTable_ = xkb_compose_table_new_from_locale(
      XkbContext_, locale, XKB_COMPOSE_COMPILE_NO_FLAGS);
  XkbComposeState_ =
      xkb_compose_state_new(XkbComposeTable_, XKB_COMPOSE_STATE_NO_FLAGS);
  xkb_compose_state_reset(XkbComposeState_);
}

CKeysymMaker& CKeysymMaker::operator=(CKeysymMaker& old) {
  if (this != &old) {
    xkb_context_unref(XkbContext_);
    xkb_compose_table_unref(XkbComposeTable_);
    xkb_compose_state_unref(XkbComposeState_);
    XkbContext_ = xkb_context_ref(old.XkbContext_);
    XkbComposeTable_ = xkb_compose_table_ref(old.XkbComposeTable_);
    XkbComposeState_ = xkb_compose_state_ref(old.XkbComposeState_);
    XkbDesc_ = old.XkbDesc_;
  }
  return *this;
}

CKeysymMaker& CKeysymMaker::operator=(CKeysymMaker&& old) {
  if (this != &old) {
    std::swap(XkbContext_, old.XkbContext_);
    std::swap(XkbComposeTable_, old.XkbComposeTable_);
    std::swap(XkbComposeState_, old.XkbComposeState_);
    std::swap(XkbDesc_, old.XkbDesc_);
  }
  return *this;
}
CKeysymMaker::~CKeysymMaker() {
  xkb_context_unref(XkbContext_);
  xkb_compose_table_unref(XkbComposeTable_);
  xkb_compose_state_unref(XkbComposeState_);
}

xkb_keysym_t CKeysymMaker::simpleKeysym(XIDeviceEvent* DeviceEvent) {
  int effective_group = DeviceEvent->group.effective;
  if (effective_group >=
      (int)XkbDesc_->map->key_sym_map[DeviceEvent->detail].group_info)
    effective_group =
        (int)XkbDesc_->map->key_sym_map[DeviceEvent->detail].group_info - 1;
  int width = (int)XkbDesc_->map->key_sym_map[DeviceEvent->detail].width;
  int effective_mods = DeviceEvent->mods.effective;
  int kt = (int)XkbDesc_->map->key_sym_map[DeviceEvent->detail]
               .kt_index[effective_group];
  int shift_level = 0;
  effective_mods = effective_mods & XkbDesc_->map->types[kt].mods.mask;
  for (int i = 0; i < XkbDesc_->map->types[kt].map_count; i++) {
    if (XkbDesc_->map->types[kt].map[i].mods.mask == effective_mods) {
      shift_level = XkbDesc_->map->types[kt].map[i].level;
      break;
    }
  }
  auto keysym =
      XkbDesc_->map
          ->syms[effective_group * width + shift_level +
                 (int)XkbDesc_->map->key_sym_map[DeviceEvent->detail].offset];
  return keysym;
}
xkb_keysym_t CKeysymMaker::feedEvent(XIDeviceEvent* DeviceEvent) {
  auto keysym = simpleKeysym(DeviceEvent);
  auto XkbComposeFeedResult = xkb_compose_state_feed(XkbComposeState_, keysym);
  auto compose_status = xkb_compose_state_get_status(XkbComposeState_);
  if (XkbComposeFeedResult == XKB_COMPOSE_FEED_ACCEPTED) {
    if (compose_status == XKB_COMPOSE_COMPOSING ||
        compose_status == XKB_COMPOSE_CANCELLED) {
      return keysym;
    }
    if (compose_status == XKB_COMPOSE_COMPOSED) {
      return xkb_compose_state_get_one_sym(XkbComposeState_);
    }
  }
  return keysym;
}
} // namespace NSLinux
} // namespace NSKeyboard
} // namespace NSApplication
