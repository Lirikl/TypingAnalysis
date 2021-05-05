#ifndef NSAPPLICATION_NSKEYBOARD_CKEYPOSITION_H
#define NSAPPLICATION_NSKEYBOARD_CKEYPOSITION_H

namespace NSApplication {
namespace NSKeyboard {

struct CKeyPosEnum {
  using CType = unsigned char;
  enum : CType {
    UNKN = 0,
    IGNR = 1,
    // TO DO
    // Make continuous numeration from 8
    // Add OEM_102 Key
    ESC = 9,
    AE01 = 10,
    AE02 = 11,
    AE03 = 12,
    AE04 = 13,
    AE05 = 14,
    AE06 = 15,
    AE07 = 16,
    AE08 = 17,
    AE09 = 18,
    AE10 = 19,
    AE11 = 20,
    AE12 = 21,
    BKSP = 22,
    AE13 = BKSP,
    TAB = 23,
    AD00 = TAB,
    AD01 = 24,
    AD02 = 25,
    AD03 = 26,
    AD04 = 27,
    AD05 = 28,
    AD06 = 29,
    AD07 = 30,
    AD08 = 31,
    AD09 = 32,
    AD10 = 33,
    AD11 = 34,
    AD12 = 35,
    RTRN = 36,
    AC12 = RTRN,
    LCTL = 37,
    AC01 = 38,
    AC02 = 39,
    AC03 = 40,
    AC04 = 41,
    AC05 = 42,
    AC06 = 43,
    AC07 = 44,
    AC08 = 45,
    AC09 = 46,
    AC10 = 47,
    AC11 = 48,
    TLDE = 49,
    AE00 = TLDE,
    LFSH = 50,
    AB00 = LFSH,
    BKSL = 51,
    AD13 = BKSL,
    AB01 = 52,
    AB02 = 53,
    AB03 = 54,
    AB04 = 55,
    AB05 = 56,
    AB06 = 57,
    AB07 = 58,
    AB08 = 59,
    AB09 = 60,
    AB10 = 61,
    RTSH = 62,
    AB11 = RTSH,
    KPMU = 63,
    LALT = 64,
    SPCE = 65,
    CAPS = 66,
    AC00 = CAPS,
    FK01 = 67,
    FK02 = 68,
    FK03 = 69,
    FK04 = 70,
    FK05 = 71,
    FK06 = 72,
    FK07 = 73,
    FK08 = 74,
    FK09 = 75,
    FK10 = 76,
    NMLK = 77,
    SCLK = 78,
    KP7 = 79,
    KP8 = 80,
    KP9 = 81,
    KPSU = 82,
    KP4 = 83,
    KP5 = 84,
    KP6 = 85,
    KPAD = 86,
    KP1 = 87,
    KP2 = 88,
    KP3 = 89,
    kP0 = 90,
    kPDL = 91,
    FK11 = 95,
    FK12 = 96,
    KPEN = 104,
    RCTL = 105,
    KPDV = 106,
    PRSC = 107,
    RALT = 108,
    HOME = 110,
    UP = 111,
    PGUP = 112,
    LEFT = 113,
    RGHT = 114,
    END = 115,
    DOWN = 116,
    PGDN = 117,
    INS = 118,
    DELE = 119,
    PAUS = 127,
    LWIN = 133,
    RWIN = 134,
    MENU = 135,
  };
};

using CKeyPosition = CKeyPosEnum::CType;

} // namespace NSKeyboard
} // namespace NSApplication

#endif // NSAPPLICATION_NSKEYBOARD_CKEYPOSITION_H
