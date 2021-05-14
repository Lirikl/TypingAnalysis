#include "KeysymMaker.h"
#include <iostream>

namespace NSApplication {
namespace NSKeyboard {
namespace NSLinux {

CKeysymMaker::CKeysymMaker() {
}
CKeysymMaker::CKeysymMaker(XkbDescPtr XkbDesc)
    : XkbDesc_(XkbDesc), XkbContext_(xkb_context_new(XKB_CONTEXT_NO_FLAGS)),
      XkbComposeTable_(xkb_compose_table_new_from_locale(
          XkbContext_, std::setlocale(LC_ALL, ""),
          XKB_COMPOSE_COMPILE_NO_FLAGS)),
      XkbComposeState_(
          xkb_compose_state_new(XkbComposeTable_, XKB_COMPOSE_STATE_NO_FLAGS)) {
}

CKeysymMaker& CKeysymMaker::operator=(CKeysymMaker& old) {
  xkb_context_unref(XkbContext_);
  xkb_compose_table_unref(XkbComposeTable_);
  xkb_compose_state_unref(XkbComposeState_);
  XkbContext_ = xkb_context_ref(old.XkbContext_);
  XkbComposeTable_ = xkb_compose_table_ref(old.XkbComposeTable_);
  XkbComposeState_ = xkb_compose_state_ref(old.XkbComposeState_);
  XkbDesc_ = old.XkbDesc_;
  return *this;
}

CKeysymMaker& CKeysymMaker::operator=(CKeysymMaker&& old) {
  xkb_context_unref(XkbContext_);
  xkb_compose_table_unref(XkbComposeTable_);
  xkb_compose_state_unref(XkbComposeState_);
  XkbContext_ = xkb_context_ref(old.XkbContext_);
  XkbComposeTable_ = xkb_compose_table_ref(old.XkbComposeTable_);
  XkbComposeState_ = xkb_compose_state_ref(old.XkbComposeState_);
  XkbDesc_ = old.XkbDesc_;
  return *this;
}

CKeysymMaker::CKeysymMaker(CKeysymMaker& old) {
  xkb_context_unref(XkbContext_);
  xkb_compose_table_unref(XkbComposeTable_);
  xkb_compose_state_unref(XkbComposeState_);
  XkbContext_ = xkb_context_ref(old.XkbContext_);
  XkbComposeTable_ = xkb_compose_table_ref(old.XkbComposeTable_);
  XkbComposeState_ = xkb_compose_state_ref(old.XkbComposeState_);
  XkbDesc_ = old.XkbDesc_;
}
CKeysymMaker::~CKeysymMaker() {
  xkb_context_unref(XkbContext_);
  xkb_compose_table_unref(XkbComposeTable_);
  xkb_compose_state_unref(XkbComposeState_);
}

void CKeysymMaker::resetState() {
  xkb_compose_state_reset(XkbComposeState_);
}

xkb_keycode_t CKeysymMaker::getKeycode(XIDeviceEvent* DeviceEvent) {
  return DeviceEvent->detail;
}

int CKeysymMaker::getGroup(XIDeviceEvent* DeviceEvent) {
  xkb_keycode_t keycode = getKeycode(DeviceEvent);
  int group_effective = DeviceEvent->group.effective;
  if (group_effective >= (int)XkbDesc_->map->key_sym_map[keycode].group_info)
    group_effective = (int)XkbDesc_->map->key_sym_map[keycode].group_info - 1;
  return group_effective;
}

int CKeysymMaker::getWidth(xkb_keycode_t keycode) {
  return XkbDesc_->map->key_sym_map[keycode].width;
}

int CKeysymMaker::getKt(xkb_keycode_t keycode, int group_effective) {
  return XkbDesc_->map->key_sym_map[keycode].kt_index[group_effective];
}

int CKeysymMaker::getMod(XIDeviceEvent* DeviceEvent, int kt) {
  return DeviceEvent->mods.effective & XkbDesc_->map->types[kt].mods.mask;
}

int CKeysymMaker::getShiftLevel(XIDeviceEvent* DeviceEvent) {
  xkb_keycode_t keycode = getKeycode(DeviceEvent);
  int group_effective = getGroup(DeviceEvent);
  int kt = getKt(keycode, group_effective);
  int effective_mods = getMod(DeviceEvent, kt);
  int shift_level = 0;
  for (int i = 0; i < XkbDesc_->map->types[kt].map_count; i++) {
    if (XkbDesc_->map->types[kt].map[i].mods.mask == effective_mods) {
      shift_level = XkbDesc_->map->types[kt].map[i].level;
      break;
    }
  }
  return shift_level;
}

xkb_keysym_t CKeysymMaker::getPlainKeysym(XIDeviceEvent* DeviceEvent) {
  xkb_keycode_t keycode = getKeycode(DeviceEvent);
  int group_effective = getGroup(DeviceEvent);
  int width = getWidth(keycode);
  int shift_level = getShiftLevel(DeviceEvent);
  xkb_keysym_t keysym =
      XkbDesc_->map->syms[group_effective * width + shift_level +
                          (int)XkbDesc_->map->key_sym_map[keycode].offset];
  return keysym;
}

xkb_keysym_t CKeysymMaker::feedEvent(XIDeviceEvent* DeviceEvent) {
  xkb_keysym_t keysym = getPlainKeysym(DeviceEvent);
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
