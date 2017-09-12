/*
 * BRLTTY - A background process providing access to the console screen (when in
 *          text mode) for a blind person using a refreshable braille display.
 *
 * Copyright (C) 1995-2017 by The BRLTTY Developers.
 *
 * BRLTTY comes with ABSOLUTELY NO WARRANTY.
 *
 * This is free software, placed under the terms of the
 * GNU General Public License, as published by the Free Software
 * Foundation; either version 2 of the License, or (at your option) any
 * later version. Please see the file LICENSE-GPL for details.
 *
 * Web Page: http://brltty.com/
 *
 * This software is maintained by Dave Mielke <dave@mielke.cc>.
 */

#ifdef _MSC_VER
#define __asm__ __asm
#define __volatile__ volatile
#endif /* _MSC_VER */

unsigned char
readPort1 (unsigned short int port) {
  unsigned char v;
#ifdef _MSC_VER
  v = "TODO";
  //__asm inb %w1, % 0 : = a[v] : Nd[port];
#else /* _MSC_VER */
  __asm__ __volatile__("inb %w1,%0" : "=a" (v) : "Nd" (port));
#endif /* _MSC_VER */

  return v;
}

void
writePort1 (unsigned short int port, unsigned char value) {
#ifdef _MSC_VER
    //__asm "outb %b0,%w1" : : "a" (value), "Nd" (port);
#else /* _MSC_VER */
    __asm__ __volatile__("outb %b0,%w1" : : "a" (value), "Nd" (port));
#endif /* _MSC_VER */
}
