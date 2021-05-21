#ifndef KEYPOSITIONLIN_H
#define KEYPOSITIONLIN_H

#include "Keyboard/KeyPosition.h"

namespace NSApplication {
namespace NSKeyboard {
namespace NSLinux {

class CKeyPositionLin {
public:
  CKeyPositionLin() = default;
  CKeyPosition make(int code);
};

} // namespace NSLinux
} // namespace NSKeyboard
} // namespace NSApplication
#endif // KEYPOSITIONLIN_H
