#include "KeysymMaker.h"
#include <iostream>

namespace NSApplication {
namespace NSKeyboard {
namespace NSLinux {

CKeysymMakerContext::CKeysymMakerContext()
    : XkbContext_(xkb_context_new(XKB_CONTEXT_NO_FLAGS)) {
}
CKeysymMakerContext::~CKeysymMakerContext() {
  xkb_context_unref(XkbContext_);
}

#ifdef QT_NO_DEBUG
CKeysymMakerTable::CKeysymMakerTable()
    : XkbComposeTable_(xkb_compose_table_new_from_locale(
          XkbContext_, std::setlocale(LC_ALL, ""),
          XKB_COMPOSE_COMPILE_NO_FLAGS)) {
}
#else
CKeysymMakerTable::CKeysymMakerTable()
    : XkbComposeTable_(xkb_compose_table_new_from_locale(
          XkbContext_, std::setlocale(LC_ALL, "ru_RU.UTF-8"),
          XKB_COMPOSE_COMPILE_NO_FLAGS)) {
}
#endif

CKeysymMakerTable::~CKeysymMakerTable() {
  xkb_compose_table_unref(XkbComposeTable_);
}

CKeysymMakerState::CKeysymMakerState()
    : XkbComposeState_(
          xkb_compose_state_new(XkbComposeTable_, XKB_COMPOSE_STATE_NO_FLAGS)) {
}
CKeysymMakerState::~CKeysymMakerState() {
  xkb_compose_state_unref(XkbComposeState_);
}

CKeysymMaker::CKeysymMaker(XkbDescPtr XkbDesc) : XkbDesc_(XkbDesc) {
}

void CKeysymMaker::resetState() {
  xkb_compose_state_reset(XkbComposeState_);
}

xkb_keysym_t CKeysymMaker::getPlainKeysym(XIDeviceEvent* DeviceEvent) const {
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
  if (isIgnored(DeviceEvent)) {
    isLastDead_ = 0;
    return NoSymbol;
  }
  return feedKeysym(keysym);
}

xkb_keysym_t CKeysymMaker::feedKeysym(xkb_keysym_t keysym) {
  LastKeysym_ = keysym;
  auto XkbComposeFeedResult = xkb_compose_state_feed(XkbComposeState_, keysym);
  auto compose_status = xkb_compose_state_get_status(XkbComposeState_);
  isLastDead_ = 0;
  if (XkbComposeFeedResult == XKB_COMPOSE_FEED_ACCEPTED) {
    isLastDead_ = (compose_status == XKB_COMPOSE_COMPOSING);
    std::cout <<"st: "<<compose_status<<std::endl;
    if (compose_status == XKB_COMPOSE_COMPOSING) {
      return LastKeysym_;
    }
    if (compose_status == XKB_COMPOSE_CANCELLED) {
      return LastKeysym_ = NoSymbol;
    }
    if (compose_status == XKB_COMPOSE_COMPOSED) {
      return LastKeysym_ = xkb_compose_state_get_one_sym(XkbComposeState_);
    }
  }
  return LastKeysym_;
}

bool CKeysymMaker::isLastDead() const {
  return isLastDead_;
}

bool CKeysymMaker::isIgnored(XIDeviceEvent* DeviceEvent) const {
  return DeviceEvent->mods.effective & (ControlMask | Mod1Mask);
}

int CKeysymMaker::getMod(XIDeviceEvent* DeviceEvent, int kt) const {
  return DeviceEvent->mods.effective & XkbDesc_->map->types[kt].mods.mask;
}

// returns current group,
// can be not equal to group_effective for sime modifier keys
//(always group 0 for them)
int CKeysymMaker::getGroup(XIDeviceEvent* DeviceEvent) const {
  xkb_keycode_t keycode = getKeycode(DeviceEvent);
  int group_effective = DeviceEvent->group.effective;
  if (group_effective >= (int)XkbDesc_->map->key_sym_map[keycode].group_info)
    group_effective = (int)XkbDesc_->map->key_sym_map[keycode].group_info - 1;
  return group_effective;
}

// returns number of shiftLevels per keycode
int CKeysymMaker::getWidth(xkb_keycode_t keycode) const {
  return XkbDesc_->map->key_sym_map[keycode].width;
}

// returns type of keycode, determining mod combinations for shift level
int CKeysymMaker::getKt(xkb_keycode_t keycode, int group_effective) const {
  return XkbDesc_->map->key_sym_map[keycode].kt_index[group_effective];
}

xkb_keycode_t CKeysymMaker::getKeycode(XIDeviceEvent* DeviceEvent) const {
  return DeviceEvent->detail;
}

int CKeysymMaker::getShiftLevel(XIDeviceEvent* DeviceEvent) const {
  xkb_keycode_t keycode = getKeycode(DeviceEvent);
  int group_effective = getGroup(DeviceEvent);
  int kt = getKt(keycode, group_effective);
  int effective_mods = getMod(DeviceEvent, kt);
  int shift_level = 0;
  // iterates through array of all meaningful mod combinations, unitl finds
  // mods the same as effective
  for (int i = 0; i < XkbDesc_->map->types[kt].map_count; i++) {
    if (XkbDesc_->map->types[kt].map[i].mods.mask == effective_mods) {
      return shift_level = XkbDesc_->map->types[kt].map[i].level;
    }
  }
  return shift_level;
}

} // namespace NSLinux
} // namespace NSKeyboard
} // namespace NSApplication
