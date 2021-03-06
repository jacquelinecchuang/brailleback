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

#include <dev/wscons/wsconsio.h>

#include "log.h"
#include "device.h"

int
canBeep (void) {
  return !!getConsole();
}

int
synchronousBeep (unsigned short frequency, unsigned short milliseconds) {
  return 0;
}

int
asynchronousBeep (unsigned short frequency, unsigned short milliseconds) {
  FILE *console = getConsole();
  if (console) {
    struct wskbd_bell_data bell;
    if (!(bell.period = milliseconds)) return 1;
    bell.pitch = frequency;
    bell.volume = 100;
    bell.which = WSKBD_BELL_DOALL;
    if (ioctl(fileno(console), WSKBDIO_COMPLEXBELL, &bell) != -1) return 1;
    logSystemError("ioctl WSKBDIO_COMPLEXBELL");
  }
  return 0;
}

int
startBeep (unsigned short frequency) {
  return 0;
}

int
stopBeep (void) {
  FILE *console = getConsole();
  if (console) {
    struct wskbd_bell_data bell;
    bell.which = WSKBD_BELL_DOVOLUME | WSKBD_BELL_DOPERIOD;
    bell.volume = 0;
    bell.period = 0;
    if (ioctl(fileno(console), WSKBDIO_COMPLEXBELL, &bell) != -1) return 1;
    logSystemError("ioctl WSKBDIO_COMPLEXBELL");
  }
  return 0;
}

void
endBeep (void) {
}
