#include "KeyPositionMaker.h"

namespace NSApplication {
namespace NSKeyboard {
namespace NSLinux {

CKeyPosition CKeyPositionMaker::make(int code) {
  CKeyPosition result;
  switch (code) {
  case 0:
    result = CKeyPosEnum::UNKN;
    break;
  case 1:
    result = CKeyPosEnum::IGNR;
    break;
  case 9:
    result = CKeyPosEnum::ESC;
    break;
  case 10:
    result = CKeyPosEnum::AE01;
    break;
  case 11:
    result = CKeyPosEnum::AE02;
    break;
  case 12:
    result = CKeyPosEnum::AE03;
    break;
  case 13:
    result = CKeyPosEnum::AE04;
    break;
  case 14:
    result = CKeyPosEnum::AE05;
    break;
  case 15:
    result = CKeyPosEnum::AE06;
    break;
  case 16:
    result = CKeyPosEnum::AE07;
    break;
  case 17:
    result = CKeyPosEnum::AE08;
    break;
  case 18:
    result = CKeyPosEnum::AE09;
    break;
  case 19:
    result = CKeyPosEnum::AE10;
    break;
  case 20:
    result = CKeyPosEnum::AE11;
    break;
  case 21:
    result = CKeyPosEnum::AE12;
    break;
  case 22:
    result = CKeyPosEnum::BKSP;
    break;
  case 23:
    result = CKeyPosEnum::TAB;
    break;
  case 24:
    result = CKeyPosEnum::AD01;
    break;
  case 25:
    result = CKeyPosEnum::AD02;
    break;
  case 26:
    result = CKeyPosEnum::AD03;
    break;
  case 27:
    result = CKeyPosEnum::AD04;
    break;
  case 28:
    result = CKeyPosEnum::AD05;
    break;
  case 29:
    result = CKeyPosEnum::AD06;
    break;
  case 30:
    result = CKeyPosEnum::AD07;
    break;
  case 31:
    result = CKeyPosEnum::AD08;
    break;
  case 32:
    result = CKeyPosEnum::AD09;
    break;
  case 33:
    result = CKeyPosEnum::AD10;
    break;
  case 34:
    result = CKeyPosEnum::AD11;
    break;
  case 35:
    result = CKeyPosEnum::AD12;
    break;
  case 36:
    result = CKeyPosEnum::RTRN;
    break;
  case 37:
    result = CKeyPosEnum::LCTL;
    break;
  case 38:
    result = CKeyPosEnum::AC01;
    break;
  case 39:
    result = CKeyPosEnum::AC02;
    break;
  case 40:
    result = CKeyPosEnum::AC03;
    break;
  case 41:
    result = CKeyPosEnum::AC04;
    break;
  case 42:
    result = CKeyPosEnum::AC05;
    break;
  case 43:
    result = CKeyPosEnum::AC06;
    break;
  case 44:
    result = CKeyPosEnum::AC07;
    break;
  case 45:
    result = CKeyPosEnum::AC08;
    break;
  case 46:
    result = CKeyPosEnum::AC09;
    break;
  case 47:
    result = CKeyPosEnum::AC10;
    break;
  case 48:
    result = CKeyPosEnum::AC11;
    break;
  case 49:
    result = CKeyPosEnum::TLDE;
    break;
  case 50:
    result = CKeyPosEnum::LFSH;
    break;
  case 51:
    result = CKeyPosEnum::BKSL;
    break;
  case 52:
    result = CKeyPosEnum::AB01;
    break;
  case 53:
    result = CKeyPosEnum::AB02;
    break;
  case 54:
    result = CKeyPosEnum::AB03;
    break;
  case 55:
    result = CKeyPosEnum::AB04;
    break;
  case 56:
    result = CKeyPosEnum::AB05;
    break;
  case 57:
    result = CKeyPosEnum::AB06;
    break;
  case 58:
    result = CKeyPosEnum::AB07;
    break;
  case 59:
    result = CKeyPosEnum::AB08;
    break;
  case 60:
    result = CKeyPosEnum::AB09;
    break;
  case 61:
    result = CKeyPosEnum::AB10;
    break;
  case 62:
    result = CKeyPosEnum::RTSH;
    break;
  case 63:
    result = CKeyPosEnum::KPMU;
    break;
  case 64:
    result = CKeyPosEnum::LALT;
    break;
  case 65:
    result = CKeyPosEnum::SPCE;
    break;
  case 66:
    result = CKeyPosEnum::CAPS;
    break;
  case 67:
    result = CKeyPosEnum::FK01;
    break;
  case 68:
    result = CKeyPosEnum::FK02;
    break;
  case 69:
    result = CKeyPosEnum::FK03;
    break;
  case 70:
    result = CKeyPosEnum::FK04;
    break;
  case 71:
    result = CKeyPosEnum::FK05;
    break;
  case 72:
    result = CKeyPosEnum::FK06;
    break;
  case 73:
    result = CKeyPosEnum::FK07;
    break;
  case 74:
    result = CKeyPosEnum::FK08;
    break;
  case 75:
    result = CKeyPosEnum::FK09;
    break;
  case 76:
    result = CKeyPosEnum::FK10;
    break;
  case 77:
    result = CKeyPosEnum::NMLK;
    break;
  case 78:
    result = CKeyPosEnum::SCLK;
    break;
  case 79:
    result = CKeyPosEnum::KP7;
    break;
  case 80:
    result = CKeyPosEnum::KP8;
    break;
  case 81:
    result = CKeyPosEnum::KP9;
    break;
  case 82:
    result = CKeyPosEnum::KPSU;
    break;
  case 83:
    result = CKeyPosEnum::KP4;
    break;
  case 84:
    result = CKeyPosEnum::KP5;
    break;
  case 85:
    result = CKeyPosEnum::KP6;
    break;
  case 86:
    result = CKeyPosEnum::KPAD;
    break;
  case 87:
    result = CKeyPosEnum::KP1;
    break;
  case 88:
    result = CKeyPosEnum::KP2;
    break;
  case 89:
    result = CKeyPosEnum::KP3;
    break;
  case 90:
    result = CKeyPosEnum::kP0;
    break;
  case 91:
    result = CKeyPosEnum::kPDL;
    break;
  case 95:
    result = CKeyPosEnum::FK11;
    break;
  case 96:
    result = CKeyPosEnum::FK12;
    break;
  case 104:
    result = CKeyPosEnum::KPEN;
    break;
  case 105:
    result = CKeyPosEnum::RCTL;
    break;
  case 106:
    result = CKeyPosEnum::KPDV;
    break;
  case 107:
    result = CKeyPosEnum::PRSC;
    break;
  case 108:
    result = CKeyPosEnum::RALT;
    break;
  case 110:
    result = CKeyPosEnum::HOME;
    break;
  case 111:
    result = CKeyPosEnum::UP;
    break;
  case 112:
    result = CKeyPosEnum::PGUP;
    break;
  case 113:
    result = CKeyPosEnum::LEFT;
    break;
  case 114:
    result = CKeyPosEnum::RGHT;
    break;
  case 115:
    result = CKeyPosEnum::END;
    break;
  case 116:
    result = CKeyPosEnum::DOWN;
    break;
  case 117:
    result = CKeyPosEnum::PGDN;
    break;
  case 118:
    result = CKeyPosEnum::INS;
    break;
  case 119:
    result = CKeyPosEnum::DELE;
    break;
  case 127:
    result = CKeyPosEnum::PAUS;
    break;
  case 133:
    result = CKeyPosEnum::LWIN;
    break;
  case 134:
    result = CKeyPosEnum::RWIN;
    break;
  case 135:
    result = CKeyPosEnum::MENU;
    break;
  default:
    result = CKeyPosEnum::UNKN;
    break;
  }
  return result;
}

} // namespace NSLinux
} // namespace NSKeyboard
} // namespace NSApplication
