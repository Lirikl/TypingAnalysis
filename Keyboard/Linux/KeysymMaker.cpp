#include<iostream>
#include "KeysymMaker.h"

namespace NSApplication {
namespace NSKeyboard {
namespace NSLinux {

  CKeysymMaker::CKeysymMaker() {}
  CKeysymMaker::CKeysymMaker(XkbDescPtr XkbDesc_) {
    XkbDesc = XkbDesc_;

    const char* locale;
    locale = setlocale(LC_ALL, "ru_RU.UTF-8"); //for debug because it makes broken locale
    //locale = setlocale(LC_ALL, ""); //for release
    XkbContext =  xkb_context_new(XKB_CONTEXT_NO_FLAGS);
    XkbComposeTable = xkb_compose_table_new_from_locale(XkbContext, locale, XKB_COMPOSE_COMPILE_NO_FLAGS);
    XkbComposeState = xkb_compose_state_new(XkbComposeTable, XKB_COMPOSE_STATE_NO_FLAGS);
    xkb_compose_state_reset(XkbComposeState);
  }

  CKeysymMaker& CKeysymMaker::operator = (CKeysymMaker& old) {
    if (this != &old) {
      xkb_context_unref(XkbContext);
      xkb_compose_table_unref(XkbComposeTable);
      xkb_compose_state_unref(XkbComposeState);
      XkbContext = xkb_context_ref(old.XkbContext);
      XkbComposeTable = xkb_compose_table_ref(old.XkbComposeTable);
      XkbComposeState = xkb_compose_state_ref(old.XkbComposeState);
      XkbDesc = old.XkbDesc;
    }
    return *this;
  }

  CKeysymMaker& CKeysymMaker::operator = (CKeysymMaker&& old) {
    if (this != &old) {
      std::swap(XkbContext, old.XkbContext);
      std::swap(XkbComposeTable, old.XkbComposeTable);
      std::swap(XkbComposeState, old.XkbComposeState);
      std::swap(XkbDesc, old.XkbDesc);
    }
    return *this;
  }
  CKeysymMaker::~CKeysymMaker() {
    xkb_context_unref(XkbContext);
    xkb_compose_table_unref(XkbComposeTable);
    xkb_compose_state_unref(XkbComposeState);
  }

  xkb_keysym_t CKeysymMaker::simpleKeysym(XIDeviceEvent *DeviceEvent) {
    int effective_group = DeviceEvent->group.effective;
    if (effective_group >= (int)XkbDesc->map->key_sym_map[DeviceEvent->detail].group_info)
            effective_group = (int)XkbDesc->map->key_sym_map[DeviceEvent->detail].group_info - 1;
    int width = (int)XkbDesc->map->key_sym_map[DeviceEvent->detail].width;
    int effective_mods = DeviceEvent->mods.effective;
    int kt = (int)XkbDesc->map->key_sym_map[DeviceEvent->detail].kt_index[effective_group];
    int shift_level = 0;
    effective_mods = effective_mods & XkbDesc->map->types[kt].mods.mask;
    for (int i = 0; i < XkbDesc->map->types[kt].map_count; i++) {
        if (XkbDesc->map->types[kt].map[i].mods.mask == effective_mods) {
            shift_level = XkbDesc->map->types[kt].map[i].level;
            break;
        }
    }
    auto keysym =XkbDesc->map->syms[effective_group * width + shift_level +
        (int)XkbDesc->map->key_sym_map[DeviceEvent->detail].offset];
    return keysym;
  }
  std::optional<xkb_keysym_t> CKeysymMaker::feedEvent(XIDeviceEvent* DeviceEvent) {
    auto keysym = simpleKeysym(DeviceEvent);
    auto XkbComposeFeedResult = xkb_compose_state_feed(XkbComposeState, keysym);
    auto compose_status = xkb_compose_state_get_status(XkbComposeState);
    if (XkbComposeFeedResult == XKB_COMPOSE_FEED_ACCEPTED) {
      if (compose_status == XKB_COMPOSE_COMPOSING || compose_status == XKB_COMPOSE_CANCELLED) {
        return std::optional<xkb_keysym_t>();
      }
      if(compose_status == XKB_COMPOSE_COMPOSED) {
        return xkb_compose_state_get_one_sym(XkbComposeState);
      }
    }
    return std::optional<xkb_keysym_t>(keysym);
  }
}
}
}
