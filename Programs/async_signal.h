/*
 * BRLTTY - A background process providing access to the console screen (when in
 *          text mode) for a blind person using a refreshable braille display.
 *
 * Copyright (C) 1995-2013 by The BRLTTY Developers.
 *
 * BRLTTY comes with ABSOLUTELY NO WARRANTY.
 *
 * This is free software, placed under the terms of the
 * GNU General Public License, as published by the Free Software
 * Foundation; either version 2 of the License, or (at your option) any
 * later version. Please see the file LICENSE-GPL for details.
 *
 * Web Page: http://mielke.cc/brltty/
 *
 * This software is maintained by Dave Mielke <dave@mielke.cc>.
 */

#ifndef BRLTTY_INCLUDED_ASYNC_SIGNAL
#define BRLTTY_INCLUDED_ASYNC_SIGNAL

#include "prologue.h"

#undef ASYNC_CAN_HANDLE_SIGNALS
#ifdef HAVE_SIGNAL_H
#define ASYNC_CAN_HANDLE_SIGNALS
#include <signal.h>
#endif /* HAVE_SIGNAL_H */

#include "async.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifdef ASYNC_CAN_HANDLE_SIGNALS
typedef struct {
  int signalNumber;
  void *data;
} AsyncSignalHandlerParameters;

typedef void AsyncSignalHandler (const AsyncSignalHandlerParameters *parameters);

extern int asyncHandleSignal (
  AsyncHandle *handle, int signalNumber,
  AsyncSignalHandler *handler, void *data
);

extern int asyncSetSignalHandler (int signalNumber, sighandler_t newHandler, sighandler_t *oldHandler);
extern int asyncSetSignalBlocked (int signalNumber, int state);
extern int asyncIsSignalBlocked (int signalNumber);
#endif /* ASYNC_CAN_HANDLE_SIGNALS */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* BRLTTY_INCLUDED_ASYNC_SIGNAL */
