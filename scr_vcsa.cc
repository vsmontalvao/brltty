/*
 * BRLTTY - A background process providing access to the Linux console (when in
 *          text mode) for a blind person using a refreshable braille display.
 *
 * Copyright (C) 1995-2001 by The BRLTTY Team. All rights reserved.
 *
 * BRLTTY comes with ABSOLUTELY NO WARRANTY.
 *
 * This is free software, placed under the terms of the
 * GNU General Public License, as published by the Free Software
 * Foundation.  Please see the file COPYING for details.
 *
 * Web Page: http://mielke.cc/brltty/
 *
 * This software is maintained by Dave Mielke <dave@mielke.cc>.
 */

/* scr_vcsa.cc - screen library for use with Linux vcsa devices.
 *
 * Note: Although C++, this code requires no standard C++ library.
 * This is important as BRLTTY *must not* rely on too many
 * run-time shared libraries, nor be a huge executable.
 */

#define SCR_C 1

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <linux/vt.h>
#include <linux/kd.h>

#include "misc.h"
#include "scr.h"
#include "scrdev.h"
#include "scr_vcsa.h"



/* Instanciation of the vcsa driver here */
static VcsaScreen vcsa;

/* Pointer for external reference */
RealScreen *live = &vcsa;

/* Copied from linux-2.2.17/drivers/char/consolemap.c: translations[0]
 * 8-bit Latin-1 mapped to Unicode -- trivial mapping
 */
static const unsigned short iso_latin1_map[0X100] = {
   0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007,
   0x0008, 0x0009, 0x000a, 0x000b, 0x000c, 0x000d, 0x000e, 0x000f,
   0x0010, 0x0011, 0x0012, 0x0013, 0x0014, 0x0015, 0x0016, 0x0017,
   0x0018, 0x0019, 0x001a, 0x001b, 0x001c, 0x001d, 0x001e, 0x001f,
   0x0020, 0x0021, 0x0022, 0x0023, 0x0024, 0x0025, 0x0026, 0x0027,
   0x0028, 0x0029, 0x002a, 0x002b, 0x002c, 0x002d, 0x002e, 0x002f,
   0x0030, 0x0031, 0x0032, 0x0033, 0x0034, 0x0035, 0x0036, 0x0037,
   0x0038, 0x0039, 0x003a, 0x003b, 0x003c, 0x003d, 0x003e, 0x003f,
   0x0040, 0x0041, 0x0042, 0x0043, 0x0044, 0x0045, 0x0046, 0x0047,
   0x0048, 0x0049, 0x004a, 0x004b, 0x004c, 0x004d, 0x004e, 0x004f,
   0x0050, 0x0051, 0x0052, 0x0053, 0x0054, 0x0055, 0x0056, 0x0057,
   0x0058, 0x0059, 0x005a, 0x005b, 0x005c, 0x005d, 0x005e, 0x005f,
   0x0060, 0x0061, 0x0062, 0x0063, 0x0064, 0x0065, 0x0066, 0x0067,
   0x0068, 0x0069, 0x006a, 0x006b, 0x006c, 0x006d, 0x006e, 0x006f,
   0x0070, 0x0071, 0x0072, 0x0073, 0x0074, 0x0075, 0x0076, 0x0077,
   0x0078, 0x0079, 0x007a, 0x007b, 0x007c, 0x007d, 0x007e, 0x007f,
   0x0080, 0x0081, 0x0082, 0x0083, 0x0084, 0x0085, 0x0086, 0x0087,
   0x0088, 0x0089, 0x008a, 0x008b, 0x008c, 0x008d, 0x008e, 0x008f,
   0x0090, 0x0091, 0x0092, 0x0093, 0x0094, 0x0095, 0x0096, 0x0097,
   0x0098, 0x0099, 0x009a, 0x009b, 0x009c, 0x009d, 0x009e, 0x009f,
   0x00a0, 0x00a1, 0x00a2, 0x00a3, 0x00a4, 0x00a5, 0x00a6, 0x00a7,
   0x00a8, 0x00a9, 0x00aa, 0x00ab, 0x00ac, 0x00ad, 0x00ae, 0x00af,
   0x00b0, 0x00b1, 0x00b2, 0x00b3, 0x00b4, 0x00b5, 0x00b6, 0x00b7,
   0x00b8, 0x00b9, 0x00ba, 0x00bb, 0x00bc, 0x00bd, 0x00be, 0x00bf,
   0x00c0, 0x00c1, 0x00c2, 0x00c3, 0x00c4, 0x00c5, 0x00c6, 0x00c7,
   0x00c8, 0x00c9, 0x00ca, 0x00cb, 0x00cc, 0x00cd, 0x00ce, 0x00cf,
   0x00d0, 0x00d1, 0x00d2, 0x00d3, 0x00d4, 0x00d5, 0x00d6, 0x00d7,
   0x00d8, 0x00d9, 0x00da, 0x00db, 0x00dc, 0x00dd, 0x00de, 0x00df,
   0x00e0, 0x00e1, 0x00e2, 0x00e3, 0x00e4, 0x00e5, 0x00e6, 0x00e7,
   0x00e8, 0x00e9, 0x00ea, 0x00eb, 0x00ec, 0x00ed, 0x00ee, 0x00ef,
   0x00f0, 0x00f1, 0x00f2, 0x00f3, 0x00f4, 0x00f5, 0x00f6, 0x00f7,
   0x00f8, 0x00f9, 0x00fa, 0x00fb, 0x00fc, 0x00fd, 0x00fe, 0x00ff
};

/* Copied from linux-2.2.17/drivers/char/consolemap.c: translations[1]
 * VT100 graphics mapped to Unicode
 */
static const unsigned short vt100_graphics_map[0X100] = {
   0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007,
   0x0008, 0x0009, 0x000a, 0x000b, 0x000c, 0x000d, 0x000e, 0x000f,
   0x0010, 0x0011, 0x0012, 0x0013, 0x0014, 0x0015, 0x0016, 0x0017,
   0x0018, 0x0019, 0x001a, 0x001b, 0x001c, 0x001d, 0x001e, 0x001f,
   0x0020, 0x0021, 0x0022, 0x0023, 0x0024, 0x0025, 0x0026, 0x0027,
   0x0028, 0x0029, 0x002a, 0x2192, 0x2190, 0x2191, 0x2193, 0x002f,
   0x2588, 0x0031, 0x0032, 0x0033, 0x0034, 0x0035, 0x0036, 0x0037,
   0x0038, 0x0039, 0x003a, 0x003b, 0x003c, 0x003d, 0x003e, 0x003f,
   0x0040, 0x0041, 0x0042, 0x0043, 0x0044, 0x0045, 0x0046, 0x0047,
   0x0048, 0x0049, 0x004a, 0x004b, 0x004c, 0x004d, 0x004e, 0x004f,
   0x0050, 0x0051, 0x0052, 0x0053, 0x0054, 0x0055, 0x0056, 0x0057,
   0x0058, 0x0059, 0x005a, 0x005b, 0x005c, 0x005d, 0x005e, 0x00a0,
   0x25c6, 0x2592, 0x2409, 0x240c, 0x240d, 0x240a, 0x00b0, 0x00b1,
   0x2591, 0x240b, 0x2518, 0x2510, 0x250c, 0x2514, 0x253c, 0xf800,
   0xf801, 0x2500, 0xf803, 0xf804, 0x251c, 0x2524, 0x2534, 0x252c,
   0x2502, 0x2264, 0x2265, 0x03c0, 0x2260, 0x00a3, 0x00b7, 0x007f,
   0x0080, 0x0081, 0x0082, 0x0083, 0x0084, 0x0085, 0x0086, 0x0087,
   0x0088, 0x0089, 0x008a, 0x008b, 0x008c, 0x008d, 0x008e, 0x008f,
   0x0090, 0x0091, 0x0092, 0x0093, 0x0094, 0x0095, 0x0096, 0x0097,
   0x0098, 0x0099, 0x009a, 0x009b, 0x009c, 0x009d, 0x009e, 0x009f,
   0x00a0, 0x00a1, 0x00a2, 0x00a3, 0x00a4, 0x00a5, 0x00a6, 0x00a7,
   0x00a8, 0x00a9, 0x00aa, 0x00ab, 0x00ac, 0x00ad, 0x00ae, 0x00af,
   0x00b0, 0x00b1, 0x00b2, 0x00b3, 0x00b4, 0x00b5, 0x00b6, 0x00b7,
   0x00b8, 0x00b9, 0x00ba, 0x00bb, 0x00bc, 0x00bd, 0x00be, 0x00bf,
   0x00c0, 0x00c1, 0x00c2, 0x00c3, 0x00c4, 0x00c5, 0x00c6, 0x00c7,
   0x00c8, 0x00c9, 0x00ca, 0x00cb, 0x00cc, 0x00cd, 0x00ce, 0x00cf,
   0x00d0, 0x00d1, 0x00d2, 0x00d3, 0x00d4, 0x00d5, 0x00d6, 0x00d7,
   0x00d8, 0x00d9, 0x00da, 0x00db, 0x00dc, 0x00dd, 0x00de, 0x00df,
   0x00e0, 0x00e1, 0x00e2, 0x00e3, 0x00e4, 0x00e5, 0x00e6, 0x00e7,
   0x00e8, 0x00e9, 0x00ea, 0x00eb, 0x00ec, 0x00ed, 0x00ee, 0x00ef,
   0x00f0, 0x00f1, 0x00f2, 0x00f3, 0x00f4, 0x00f5, 0x00f6, 0x00f7,
   0x00f8, 0x00f9, 0x00fa, 0x00fb, 0x00fc, 0x00fd, 0x00fe, 0x00ff
};

/* Copied from linux-2.2.17/drivers/char/consolemap.c: translations[2]
 * IBM Codepage 437 mapped to Unicode
 */
static const unsigned short ibm_cp437_map[0X100] = {
   0x0000, 0x263a, 0x263b, 0x2665, 0x2666, 0x2663, 0x2660, 0x2022, 
   0x25d8, 0x25cb, 0x25d9, 0x2642, 0x2640, 0x266a, 0x266b, 0x263c,
   0x25b6, 0x25c0, 0x2195, 0x203c, 0x00b6, 0x00a7, 0x25ac, 0x21a8,
   0x2191, 0x2193, 0x2192, 0x2190, 0x221f, 0x2194, 0x25b2, 0x25bc,
   0x0020, 0x0021, 0x0022, 0x0023, 0x0024, 0x0025, 0x0026, 0x0027,
   0x0028, 0x0029, 0x002a, 0x002b, 0x002c, 0x002d, 0x002e, 0x002f,
   0x0030, 0x0031, 0x0032, 0x0033, 0x0034, 0x0035, 0x0036, 0x0037,
   0x0038, 0x0039, 0x003a, 0x003b, 0x003c, 0x003d, 0x003e, 0x003f,
   0x0040, 0x0041, 0x0042, 0x0043, 0x0044, 0x0045, 0x0046, 0x0047,
   0x0048, 0x0049, 0x004a, 0x004b, 0x004c, 0x004d, 0x004e, 0x004f,
   0x0050, 0x0051, 0x0052, 0x0053, 0x0054, 0x0055, 0x0056, 0x0057,
   0x0058, 0x0059, 0x005a, 0x005b, 0x005c, 0x005d, 0x005e, 0x005f,
   0x0060, 0x0061, 0x0062, 0x0063, 0x0064, 0x0065, 0x0066, 0x0067,
   0x0068, 0x0069, 0x006a, 0x006b, 0x006c, 0x006d, 0x006e, 0x006f,
   0x0070, 0x0071, 0x0072, 0x0073, 0x0074, 0x0075, 0x0076, 0x0077,
   0x0078, 0x0079, 0x007a, 0x007b, 0x007c, 0x007d, 0x007e, 0x2302,
   0x00c7, 0x00fc, 0x00e9, 0x00e2, 0x00e4, 0x00e0, 0x00e5, 0x00e7,
   0x00ea, 0x00eb, 0x00e8, 0x00ef, 0x00ee, 0x00ec, 0x00c4, 0x00c5,
   0x00c9, 0x00e6, 0x00c6, 0x00f4, 0x00f6, 0x00f2, 0x00fb, 0x00f9,
   0x00ff, 0x00d6, 0x00dc, 0x00a2, 0x00a3, 0x00a5, 0x20a7, 0x0192,
   0x00e1, 0x00ed, 0x00f3, 0x00fa, 0x00f1, 0x00d1, 0x00aa, 0x00ba,
   0x00bf, 0x2310, 0x00ac, 0x00bd, 0x00bc, 0x00a1, 0x00ab, 0x00bb,
   0x2591, 0x2592, 0x2593, 0x2502, 0x2524, 0x2561, 0x2562, 0x2556,
   0x2555, 0x2563, 0x2551, 0x2557, 0x255d, 0x255c, 0x255b, 0x2510,
   0x2514, 0x2534, 0x252c, 0x251c, 0x2500, 0x253c, 0x255e, 0x255f,
   0x255a, 0x2554, 0x2569, 0x2566, 0x2560, 0x2550, 0x256c, 0x2567,
   0x2568, 0x2564, 0x2565, 0x2559, 0x2558, 0x2552, 0x2553, 0x256b,
   0x256a, 0x2518, 0x250c, 0x2588, 0x2584, 0x258c, 0x2590, 0x2580,
   0x03b1, 0x00df, 0x0393, 0x03c0, 0x03a3, 0x03c3, 0x00b5, 0x03c4,
   0x03a6, 0x0398, 0x03a9, 0x03b4, 0x221e, 0x03c6, 0x03b5, 0x2229,
   0x2261, 0x00b1, 0x2265, 0x2264, 0x2320, 0x2321, 0x00f7, 0x2248,
   0x00b0, 0x2219, 0x00b7, 0x221a, 0x207f, 0x00b2, 0x25a0, 0x00a0
};


static char *vcsaParameters[] = {
  "characterset",
  NULL
};
typedef enum {
  VCSA_CHARACTERSET
} VcsaParameters;
char **VcsaScreen::parameters (void) {
  return vcsaParameters;
}


int VcsaScreen::prepare (char **parameters) {
  {
    static const unsigned short *maps[] = {iso_latin1_map, vt100_graphics_map, ibm_cp437_map, NULL};
    static const unsigned short *map;
    static const char *choices[] = {"latin1", "vt100graphics", "cp437", "application", NULL};
    unsigned int choice;
    if (validateChoice(&choice, "character set", parameters[VCSA_CHARACTERSET], choices)) {
      map = maps[choice];
    } else {
      map = iso_latin1_map;
    }
    if (map) {
      memcpy(characterMap, map, sizeof(characterMap));
      setCharacterMap = NULL;
    } else {
      setCharacterMap = &VcsaScreen::setApplicationCharacterMap;
    }
  }
  if (setScreenDevice()) {
    screenDescriptor = -1;
    if (setConsoleDevice()) {
      consoleDescriptor = -1;
      return 1;
    }
  }
  return 0;
}

static const char *findDevice (char *devices) {
  char *current = NULL;
  int exists = 0;
  const char *delimiters = " ";
  char *device;
  while ((device = strtok(devices, delimiters))) {
    devices = NULL;
    device = strdupWrapper(device);
    LogPrint(LOG_DEBUG, "Checking device '%s'.", device);
    if (access(device, R_OK|W_OK) != -1) {
      if (current) free(current);
      current = device;
      break;
    }
    LogPrint(LOG_DEBUG, "Cannot access device '%s': %s",
             device, strerror(errno));
    if (errno != ENOENT) {
      if (!exists) {
        exists = 1;
        if (current) {
          free(current);
          current = NULL;
        }
      }
    }
    if (current)
      free(device);
    else
      current = device;
  }
  return current;
}

int VcsaScreen::setScreenDevice (void) {
  char *devices = strdupWrapper(VCSADEV);
  LogPrint(LOG_DEBUG, "Screen device list: %s", devices);
  if ((screenDevice = findDevice(devices))) {
    LogPrint(LOG_INFO, "Screen Device: %s", screenDevice);
    return 1;
  } else {
    LogPrint(LOG_ERR, "Screen device not specified.");
  }
  return 0;
}

int VcsaScreen::setConsoleDevice (void) {
  consoleDevice = "/dev/tty0";
  return 1;
}


int VcsaScreen::open (void) {
  int screen = ::open(screenDevice, O_RDWR);
  if (screen != -1) {
    if (rebindConsole()) {
      closeScreen();
      screenDescriptor = screen;
      return 1;
    }
    ::close(screen);
  } else {
    LogPrint(LOG_ERR, "Cannot open screen device '%s': %s",
             screenDevice, strerror(errno));
  }
  return 0;
}

int VcsaScreen::rebindConsole (void) {
  int console = ::open(consoleDevice, O_RDWR|O_NOCTTY);
  if (console != -1) {
    closeConsole();
    consoleDescriptor = console;
    return 1;
  } else {
    LogPrint(LOG_ERR, "Cannot open console device '%s': %s",
             consoleDevice, strerror(errno));
  }
  return 0;
}


int VcsaScreen::setup (void) {
  if (setTranslationTable(1)) {
    return 1;
  }
  return 0;
}


/* 
 * The virtual screen devices return the actual font positions of the glyphs to
 * be drawn on the screen. The problem is that the font may not have been
 * designed for the character set being used. Most PC video cards have a
 * built-in font defined for the CP437 character set, but Linux users often use
 * the ISO-Latin-1 character set. In addition, the PSF format used by the newer
 * font files, which contains an internal unicode to font-position table,
 * allows the actual font positions to be unrelated to any known character set.
 * The kernel translates each character to be written to the screen from the
 * character set being used into unicode, and then from unicode into the
 * position within the font being used of the glyph to be drawn on the screen.
 * We need to reverse this translation in order to get the character code in
 * the expected character set.
 */
int VcsaScreen::setTranslationTable (int opening) {
  int changed = 0;
  if (setCharacterMap && (this->*setCharacterMap)(opening)) changed = 1;
  if (setScreenFontMap(opening)) changed = 1;
  if (changed) {
    int character;
    memset(translationTable, '?', sizeof(translationTable));
    for (character=0XFF; character>=0; --character) {
      unsigned short unicode = characterMap[character];
      int position;
      if ((unicode >> 8) == 0XF0) {
        position = unicode & 0XFF;
      } else {
        int first = 0;
        int last = screenFontMapCount-1;
        position = -1;
        while (first <= last) {
          int current = (first + last) / 2;
          struct unipair *map = &screenFontMapTable[current];
          if (map->unicode < unicode)
            first = current + 1;
          else if (map->unicode > unicode)
            last = current - 1;
          else if (map->fontpos <= 0XFF) {
            position = map->fontpos;
            break;
          }
        }
      }
      if (position < 0) {
        LogPrint(LOG_DEBUG, "No character mapping: char=%2.2X unum=%4.4X", character, unicode);
      } else {
        translationTable[position] = character;
        LogPrint(LOG_DEBUG, "Character mapping: char=%2.2X unum=%4.4X fpos=%2.2X",
                 character, unicode, position);
      }
    }
    return 1;
  }
  return 0;
}

int VcsaScreen::setApplicationCharacterMap (int opening) {
  unsigned short map[0X100];
  if (ioctl(consoleDescriptor, GIO_UNISCRNMAP, &map) != -1) {
    if (opening || (memcmp(characterMap, map, sizeof(characterMap)) != 0)) {
      memcpy(characterMap, map, sizeof(characterMap));
      LogPrint(LOG_DEBUG, "Character map changed.");
      return 1;
    }
  } else {
    LogError("ioctl GIO_UNISCRNMAP");
  }
  return 0;
}

int VcsaScreen::setScreenFontMap (int opening) {
  struct unimapdesc sfm;
  unsigned short size = opening? 0X100: screenFontMapCount;
  while (1) {
    sfm.entry_ct = size;
    if (!(sfm.entries = (struct unipair *)malloc(sfm.entry_ct * sizeof(*sfm.entries)))) {
      LogError("Screen font map allocation");
      return 0;
    }
    if (ioctl(consoleDescriptor, GIO_UNIMAP, &sfm) != -1) break;
    free(sfm.entries);
    if (errno != ENOMEM) {
      LogError("ioctl GIO_UNIMAP");
      return 0;
    }
    if (!(size <<= 1)) {
      LogPrint(LOG_ERR, "Screen font map too big.");
      return 0;
    }
  }
  if (!opening) {
    if (sfm.entry_ct == screenFontMapCount) {
      if (memcmp(sfm.entries, screenFontMapTable, sfm.entry_ct*sizeof(sfm.entries[0])) == 0) {
        if (size == screenFontMapSize) {
          free(sfm.entries);
        } else {
          free(screenFontMapTable);
          screenFontMapTable = sfm.entries;
          screenFontMapSize = size;
        }
        return 0;
      }
    }
    free(screenFontMapTable);
  }
  screenFontMapTable = sfm.entries;
  screenFontMapCount = sfm.entry_ct;
  screenFontMapSize = size;
  LogPrint(LOG_DEBUG, "Screen font map changed: %d %s.",
           screenFontMapCount, (screenFontMapCount==1? "entry": "entries"));
  return 1;
}


void VcsaScreen::getstat (ScreenStatus &stat) {
  struct vt_stat state;
  if (ioctl(consoleDescriptor, VT_GETSTATE, &state) != -1) {
    stat.no = state.v_active;
  } else {
    LogError("ioctl VT_GETSTATE");
  }

  if (lseek(screenDescriptor, 0, SEEK_SET) != -1) {
    unsigned char buffer[4];
    if (read(screenDescriptor, buffer, sizeof(buffer)) != -1) {
      stat.rows = buffer[0];
      stat.cols = buffer[1];
      stat.posx = buffer[2];
      stat.posy = buffer[3];
    } else {
      LogError("Screen read");
    }
  } else {
    LogError("Screen seek");
  }

  /* Periodically recalculate font mapping. I don't know any way to be
   * notified when it changes, and the recalculation is not too
   * long/difficult.
   */
  {
    static int timer = 0;
    if (++timer > 100) {
      setTranslationTable(0);
      timer = 0;
    }
  }
}


unsigned char *VcsaScreen::getscr (winpos pos, unsigned char *buffer, short mode) {
  ScreenStatus stat;
  getstat(stat);
  if ((pos.left >= 0) && (pos.width > 0) && ((pos.left + pos.width) <= stat.cols) &&
      (pos.top >= 0) && (pos.height > 0) && ((pos.top + pos.height) <= stat.rows) &&
      (mode >= 0) && (mode <= 1)) {
    off_t start = 4 + (pos.top * stat.cols + pos.left) * 2 + mode;
    if (lseek(screenDescriptor, start, SEEK_SET) != -1) {
      size_t length = pos.width * 2 - 1;
      unsigned char line[length];
      unsigned char *target = buffer;
      off_t increment = stat.cols * 2 - length;
      int i;
      for (i=0; i<pos.height; ++i) {
        if (i) {
           if (increment) {
             if (lseek(screenDescriptor, increment, SEEK_CUR) == -1) {
               LogError("Screen seek");
               return NULL;
             }
           }
        }
        if (read(screenDescriptor, line, length) == -1) {
          LogError("Screen read");
          return NULL;
        }
        unsigned char *source = line;
        if (mode == SCR_TEXT) {
          int j;
          for (j=0; j<pos.width; ++j) {
            *target++ = translationTable[*source];
            source += 2;
          }
        } else {
          int j;
          for (j=0; j<pos.width; ++j) {
            *target++ = *source;
            source += 2;
          }
        }
      }
      return buffer;
    } else {
      LogError("Screen seek");
    }
  }
  return NULL;
}

 
int VcsaScreen::insert (unsigned short key) {
  int ok = 0;
  LogPrint(LOG_DEBUG, "Insert key: %4.4X", key);
  if (rebindConsole()) {
    long mode;
    if (ioctl(consoleDescriptor, KDGKBMODE, &mode) != -1) {
      switch (mode) {
        case K_RAW:
          if (insertCode(key, 1)) ok = 1;
          break;
        case K_MEDIUMRAW:
          if (insertCode(key, 0)) ok = 1;
          break;
        case K_XLATE:
          if (insertMapped(key, &VcsaScreen::insertByte)) ok = 1;
          break;
        case K_UNICODE:
          if (insertMapped(key, &VcsaScreen::insertUtf8)) ok = 1;
          break;
        default:
          LogPrint(LOG_WARNING, "Unsupported keyboard mode: %d", mode);
          break;
      }
    } else {
      LogError("ioctl KDGKBMODE");
    }
  }
  return ok;
}

int VcsaScreen::insertCode (unsigned short key, int raw) {
  unsigned char prefix = 0X00;
  unsigned char code;
  switch (key) {
    case KEY_ESCAPE:        code = 0X01; break;
    case KEY_FUNCTION +  0: code = 0X3B; break;
    case KEY_FUNCTION +  1: code = 0X3C; break;
    case KEY_FUNCTION +  2: code = 0X3D; break;
    case KEY_FUNCTION +  3: code = 0X3E; break;
    case KEY_FUNCTION +  4: code = 0X3F; break;
    case KEY_FUNCTION +  5: code = 0X40; break;
    case KEY_FUNCTION +  6: code = 0X41; break;
    case KEY_FUNCTION +  7: code = 0X42; break;
    case KEY_FUNCTION +  8: code = 0X43; break;
    case KEY_FUNCTION +  9: code = 0X44; break;
    case KEY_FUNCTION + 10: code = 0X57; break;
    case KEY_FUNCTION + 11: code = 0X58; break;
    case '`':               code = 0X29; break;
    case '1':               code = 0X02; break;
    case '2':               code = 0X03; break;
    case '3':               code = 0X04; break;
    case '4':               code = 0X05; break;
    case '5':               code = 0X06; break;
    case '6':               code = 0X07; break;
    case '7':               code = 0X08; break;
    case '8':               code = 0X09; break;
    case '9':               code = 0X0A; break;
    case '0':               code = 0X0B; break;
    case '-':               code = 0X0C; break;
    case '=':               code = 0X0D; break;
    case KEY_BACKSPACE:     code = 0X0E; break;
    case KEY_TAB:           code = 0X0F; break;
    case 'q':               code = 0X10; break;
    case 'w':               code = 0X11; break;
    case 'e':               code = 0X12; break;
    case 'r':               code = 0X13; break;
    case 't':               code = 0X14; break;
    case 'y':               code = 0X15; break;
    case 'u':               code = 0X16; break;
    case 'i':               code = 0X17; break;
    case 'o':               code = 0X18; break;
    case 'p':               code = 0X19; break;
    case '[':               code = 0X1A; break;
    case ']':               code = 0X1B; break;
    case '\\':              code = 0X2B; break;
    case 'a':               code = 0X1E; break;
    case 's':               code = 0X1F; break;
    case 'd':               code = 0X20; break;
    case 'f':               code = 0X21; break;
    case 'g':               code = 0X22; break;
    case 'h':               code = 0X23; break;
    case 'j':               code = 0X24; break;
    case 'k':               code = 0X25; break;
    case 'l':               code = 0X26; break;
    case ';':               code = 0X27; break;
    case '\'':              code = 0X28; break;
    case KEY_RETURN:        code = 0X1C; break;
    case 'z':               code = 0X2C; break;
    case 'x':               code = 0X2D; break;
    case 'c':               code = 0X2E; break;
    case 'v':               code = 0X2F; break;
    case 'b':               code = 0X30; break;
    case 'n':               code = 0X31; break;
    case 'm':               code = 0X32; break;
    case ',':               code = 0X33; break;
    case '.':               code = 0X34; break;
    case '/':               code = 0X35; break;
    case ' ':               code = 0X39; break;
    default:
      if (raw) {
        prefix = 0XE0;
        switch (key) {
          case KEY_INSERT:       code = 0X52; break;
          case KEY_HOME:         code = 0X47; break;
          case KEY_PAGE_UP:      code = 0X49; break;
          case KEY_DELETE:       code = 0X53; break;
          case KEY_END:          code = 0X4F; break;
          case KEY_PAGE_DOWN:    code = 0X51; break;
          case KEY_CURSOR_UP:    code = 0X48; break;
          case KEY_CURSOR_LEFT:  code = 0X4B; break;
          case KEY_CURSOR_DOWN:  code = 0X50; break;
          case KEY_CURSOR_RIGHT: code = 0X4D; break;
          default:
            LogPrint(LOG_WARNING, "Key %4.4X not suported in scancode mode.", key);
            return 0;
        }
      } else {
        switch (key) {
          case KEY_INSERT:       code = 0X6E; break;
          case KEY_HOME:         code = 0X66; break;
          case KEY_PAGE_UP:      code = 0X68; break;
          case KEY_DELETE:       code = 0X6F; break;
          case KEY_END:          code = 0X6B; break;
          case KEY_PAGE_DOWN:    code = 0X6D; break;
          case KEY_CURSOR_UP:    code = 0X67; break;
          case KEY_CURSOR_LEFT:  code = 0X69; break;
          case KEY_CURSOR_DOWN:  code = 0X6C; break;
          case KEY_CURSOR_RIGHT: code = 0X6A; break;
          default:
            LogPrint(LOG_WARNING, "Key %4.4X not suported in keycode mode.", key);
            return 0;
        }
      }
      break;
  }
  {
    unsigned char buffer[4];
    unsigned short count = 0;
    const unsigned char *byte = buffer;
    if (prefix) buffer[count++] = prefix;
    buffer[count++] = code;
    if (prefix) buffer[count++] = prefix;
    buffer[count++] = code | 0X80;
    while (count--) {
      if (!insertByte(*byte++)) return 0;
    }
  }
  return 1;
}

int VcsaScreen::insertMapped (unsigned short key, int (VcsaScreen::*byteInserter)(unsigned char byte)) {
  if (key < 0X100) {
    unsigned char character = key & 0XFF;
    if (!(this->*byteInserter)(character)) return 0;
  } else {
    const char *sequence;
    switch (key) {
      case KEY_RETURN:
        sequence = "\r";
        break;
      case KEY_TAB:
        sequence = "\t";
        break;
      case KEY_BACKSPACE:
        sequence = "\177";
        break;
      case KEY_ESCAPE:
        sequence = "\033";
        break;
      case KEY_CURSOR_LEFT:
        sequence = "\033[D";
        break;
      case KEY_CURSOR_RIGHT:
        sequence = "\033[C";
        break;
      case KEY_CURSOR_UP:
        sequence = "\033[A";
        break;
      case KEY_CURSOR_DOWN:
        sequence = "\033[B";
        break;
      case KEY_PAGE_UP:
        sequence = "\033[5~";
        break;
      case KEY_PAGE_DOWN:
        sequence = "\033[6~";
        break;
      case KEY_HOME:
        sequence = "\033[1~";
        break;
      case KEY_END:
        sequence = "\033[4~";
        break;
      case KEY_INSERT:
        sequence = "\033[2~";
        break;
      case KEY_DELETE:
        sequence = "\033[3~";
        break;
      case KEY_FUNCTION + 0:
        sequence = "\033OP";
        break;
      case KEY_FUNCTION + 1:
        sequence = "\033OQ";
        break;
      case KEY_FUNCTION + 2:
        sequence = "\033OR";
        break;
      case KEY_FUNCTION + 3:
        sequence = "\033OS";
        break;
      case KEY_FUNCTION + 4:
        sequence = "\033[15~";
        break;
      case KEY_FUNCTION + 5:
        sequence = "\033[17~";
        break;
      case KEY_FUNCTION + 6:
        sequence = "\033[18~";
        break;
      case KEY_FUNCTION + 7:
        sequence = "\033[19~";
        break;
      case KEY_FUNCTION + 8:
        sequence = "\033[20~";
        break;
      case KEY_FUNCTION + 9:
        sequence = "\033[21~";
        break;
      case KEY_FUNCTION + 10:
        sequence = "\033[23~";
        break;
      case KEY_FUNCTION + 11:
        sequence = "\033[24~";
        break;
      case KEY_FUNCTION + 12:
        sequence = "\033[25~";
        break;
      case KEY_FUNCTION + 13:
        sequence = "\033[26~";
        break;
      case KEY_FUNCTION + 14:
        sequence = "\033[28~";
        break;
      case KEY_FUNCTION + 15:
        sequence = "\033[29~";
        break;
      case KEY_FUNCTION + 16:
        sequence = "\033[31~";
        break;
      case KEY_FUNCTION + 17:
        sequence = "\033[32~";
        break;
      case KEY_FUNCTION + 18:
        sequence = "\033[33~";
        break;
      case KEY_FUNCTION + 19:
        sequence = "\033[34~";
        break;
      default:
        LogPrint(LOG_WARNING, "Key %4.4X not suported in ANSI mode.", key);
        return 0;
    }
    while (*sequence) {
      if (!(this->*byteInserter)(*sequence++)) return 0;
    }
  }
  return 1;
}

int VcsaScreen::insertUtf8 (unsigned char byte) {
  if (byte & 0X80) {
    if (!insertByte(0XC0 | (byte >> 6))) return 0;
    byte &= 0XBF;
  }
  if (!insertByte(byte)) return 0;
  return 1;
}

int VcsaScreen::insertByte (unsigned char byte) {
  if (ioctl(consoleDescriptor, TIOCSTI, &byte) != -1) {
    return 1;
  } else {
    LogError("ioctl TIOCSTI");
  }
  return 0;
}


int VcsaScreen::switchvt (int vt) {
  if ((vt >= 1) && (vt <= 0X3F)) {
    if (ioctl(consoleDescriptor, VT_ACTIVATE, vt) != -1) {
      LogPrint(LOG_DEBUG, "Switched to virtual tertminal %d.", vt);
      return 1;
    } else {
      LogError("ioctl VT_ACTIVATE");
    }
  } else {
    LogPrint(LOG_WARNING, "Virtual termianl %d is out of range.", vt);
  }
  return 0;
}


void VcsaScreen::close (void) {
  closeConsole();
  closeScreen();
}

void VcsaScreen::closeScreen (void) {
  if (screenDescriptor != -1) {
    if (::close(screenDescriptor) == -1) {
      LogError("Screen close");
    }
    screenDescriptor = -1;
  }
}

void VcsaScreen::closeConsole (void) {
  if (consoleDescriptor != -1) {
    if (::close(consoleDescriptor) == -1) {
      LogError("Console close");
    }
    consoleDescriptor = -1;
  }
}