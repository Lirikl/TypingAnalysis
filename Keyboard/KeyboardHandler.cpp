#include "KeyboardHandler.h"

#include "KeyboardListener.h"
#include "ListenerExceptionHandler.h"
#include "QtLoopException.h"
#include "RawKeyEvent.h"

#include <QDebug>


namespace NSApplication {
namespace NSKeyboard {


CQtMessagesRegistrator::CQtMessagesRegistrator() {
  qRegisterMetaType<CRawKeyEvent>();
}


CKeyboardHandler::CKeyboardHandler() {
  CQtLoopException QtExceptions;
  QtExceptions->connectSignal(this, &CKeyboardHandler::quit);

  CAnyKillerPromise killerPromise;
  CAnyKillerFuture killerFuture = killerPromise.get_future();
  ListenerThread_ = CWorkerThread(run, std::move(killerPromise), this);
  // Need Timeout here
  KeyboardKiller_ = killerFuture.get();
}

CKeyboardHandler::~CKeyboardHandler() {
  stopListener();
}

void CKeyboardHandler::activate() {
  isActive_ = true;
}

void CKeyboardHandler::deactivate() {
  isActive_ = false;
}

void CKeyboardHandler::onKeyboardMessage(const CRawKeyEvent& message) {
  // TO DO
  qDebug() << "slotOnKeyboardMessage";
  if (isActive_)
    qDebug() << "KeyboardMessage() = " << message.Text.c_str()
             << "addr = " << this;
}

void CKeyboardHandler::onKeyboardException(const CQtException& message) {
  emit quit(message);
}

void CKeyboardHandler::stopListener() const noexcept {
  assert(KeyboardKiller_.isDefined());
  KeyboardKiller_->stopListener();
}


// This code is executed on the worker thread
void CKeyboardHandler::run(CAnyKillerPromise killerPromise,
                           CKeyboardHandler* KeyboardHandler) {
  try {
    CKeyboardListener KeyboardListener(std::move(killerPromise),
                                       KeyboardHandler);
    KeyboardListener.exec();
  } catch (...) {
    CListenerExceptionHandler React(KeyboardHandler);
  }
}

} // NSKeyboard
} // NSApplication