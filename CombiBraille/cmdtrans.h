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

/* Note: this table has three distinct sections:
 *   1. offsets 0x00-0x3f - the Braille dot keys:
 *           bit  5  4  3  2  1  0
 *           dot  3  2  1  4  5  6
 *   2. offsets 0x60-0x7f - the thumb keys:
 *           bit  4  3  2  1  0
 *           key  E  D  C  B  A
 *      (labelling A-E left to right)
 *   3. offsets 0x80 upwards - the cursor routing keys: 0x80 + keynumber
 *      (0 at the left, max. 85)
 */

/* 0X00 */ CMD_NOOP,                       // no dots
/* 0X01 */ CMD_FWINRT,                     // dots 6
/* 0X02 */ CMD_CSRVIS,                     // dots 5
/* 0X03 */ CMD_LNEND,                      // dots 5,6
/* 0X04 */ CMD_LNDN,                       // dots 4
/* 0X05 */ CMD_CHRRT,                      // dots 4,6
/* 0X06 */ CMD_WINDN,                      // dots 4,5
/* 0X07 */ CMD_BOT,                        // dots 4,5,6
/* 0X08 */ CMD_LNUP,                       // dots 1
/* 0X09 */ CMD_MUTE,                       // dots 1,6
/* 0X0A */ CMD_NOOP,                       // dots 1,5
/* 0X0B */ CMD_SAY,                        // dots 1,5,6
/* 0X0C */ CMD_CSRTRK,                     // dots 1,4
/* 0X0D */ CMD_CAPBLINK,                   // dots 1,4,6
/* 0X0E */ CMD_NOOP,                       // dots 1,4,5
/* 0X0F */ CMD_NOOP,                       // dots 1,4,5,6
/* 0X10 */ CMD_CSRBLINK,                   // dots 2
/* 0X11 */ CMD_NOOP,                       // dots 2,6
/* 0X12 */ CMD_CSRSIZE,                    // dots 2,5
/* 0X13 */ CMD_NOOP,                       // dots 2,5,6
/* 0X14 */ CMD_NOOP,                       // dots 2,4
/* 0X15 */ CMD_HWINLT,                     // dots 2,4,6
/* 0X16 */ CMD_SKPIDLNS,                   // dots 2,4,5
/* 0X17 */ CMD_SLIDEWIN,                   // dots 2,4,5,6
/* 0X18 */ CMD_WINUP,                      // dots 1,2
/* 0X19 */ CMD_NOOP,                       // dots 1,2,6
/* 0X1A */ CMD_HELP,                       // dots 1,2,5
/* 0X1B */ CMD_NOOP,                       // dots 1,2,5,6
/* 0X1C */ CMD_FREEZE,                     // dots 1,2,4
/* 0X1D */ CMD_NOOP,                       // dots 1,2,4,6
/* 0X1E */ CMD_CSRJMP,                     // dots 1,2,4,5
/* 0X1F */ CMD_NOOP,                       // dots 1,2,4,5,6
/* 0X20 */ CMD_FWINLT,                     // dots 3
/* 0X21 */ CMD_HOME,                       // dots 3,6
/* 0X22 */ CMD_NOOP,                       // dots 3,5
/* 0X23 */ CMD_CUT_END,                    // dots 3,5,6
/* 0X24 */ CMD_INFO,                       // dots 3,4
/* 0X25 */ CMD_NOOP,                       // dots 3,4,6
/* 0X26 */ CMD_NOOP,                       // dots 3,4,5
/* 0X27 */ CMD_NOOP,                       // dots 3,4,5,6
/* 0X28 */ CMD_CHRLT,                      // dots 1,3
/* 0X29 */ CMD_NOOP,                       // dots 1,3,6
/* 0X2A */ CMD_HWINRT,                     // dots 1,3,5
/* 0X2B */ CMD_NOOP,                       // dots 1,3,5,6
/* 0X2C */ CMD_DISPMD,                     // dots 1,3,4
/* 0X2D */ CMD_PREFMENU,                   // dots 1,3,4,6
/* 0X2E */ CMD_NOOP,                       // dots 1,3,4,5
/* 0X2F */ CMD_NOOP,                       // dots 1,3,4,5,6
/* 0X30 */ CMD_LNBEG,                      // dots 2,3
/* 0X31 */ CMD_CUT_BEG,                    // dots 2,3,6
/* 0X32 */ CMD_SIXDOTS,                    // dots 2,3,5
/* 0X33 */ CMD_NOOP,                       // dots 2,3,5,6
/* 0X34 */ CMD_SND,                        // dots 2,3,4
/* 0X35 */ CMD_NOOP,                       // dots 2,3,4,6
/* 0X36 */ CMD_NOOP,                       // dots 2,3,4,5
/* 0X37 */ CMD_NOOP,                       // dots 2,3,4,5,6
/* 0X38 */ CMD_TOP,                        // dots 1,2,3
/* 0X39 */ CMD_NOOP,                       // dots 1,2,3,6
/* 0X3A */ CMD_PREFLOAD,                   // dots 1,2,3,5
/* 0X3B */ CMD_NOOP,                       // dots 1,2,3,5,6
/* 0X3C */ CMD_PASTE,                      // dots 1,2,3,4
/* 0X3D */ CMD_NOOP,                       // dots 1,2,3,4,6
/* 0X3E */ CMD_NOOP,                       // dots 1,2,3,4,5
/* 0X3F */ CMD_PREFSAVE,                   // dots 1,2,3,4,5,6
/* 0X40 */ CMD_NOOP,                       // unexpected
/* 0X41 */ CMD_NOOP,                       // unexpected
/* 0X42 */ CMD_NOOP,                       // unexpected
/* 0X43 */ CMD_NOOP,                       // unexpected
/* 0X44 */ CMD_NOOP,                       // unexpected
/* 0X45 */ CMD_NOOP,                       // unexpected
/* 0X46 */ CMD_NOOP,                       // unexpected
/* 0X47 */ CMD_NOOP,                       // unexpected
/* 0X48 */ CMD_NOOP,                       // unexpected
/* 0X49 */ CMD_NOOP,                       // unexpected
/* 0X4A */ CMD_NOOP,                       // unexpected
/* 0X4B */ CMD_NOOP,                       // unexpected
/* 0X4C */ CMD_NOOP,                       // unexpected
/* 0X4D */ CMD_NOOP,                       // unexpected
/* 0X4E */ CMD_NOOP,                       // unexpected
/* 0X4F */ CMD_NOOP,                       // unexpected
/* 0X50 */ CMD_NOOP,                       // unexpected
/* 0X51 */ CMD_NOOP,                       // unexpected
/* 0X52 */ CMD_NOOP,                       // unexpected
/* 0X53 */ CMD_NOOP,                       // unexpected
/* 0X54 */ CMD_NOOP,                       // unexpected
/* 0X55 */ CMD_NOOP,                       // unexpected
/* 0X56 */ CMD_NOOP,                       // unexpected
/* 0X57 */ CMD_NOOP,                       // unexpected
/* 0X58 */ CMD_NOOP,                       // unexpected
/* 0X59 */ CMD_NOOP,                       // unexpected
/* 0X5A */ CMD_NOOP,                       // unexpected
/* 0X5B */ CMD_NOOP,                       // unexpected
/* 0X5C */ CMD_NOOP,                       // unexpected
/* 0X5D */ CMD_NOOP,                       // unexpected
/* 0X5E */ CMD_NOOP,                       // unexpected
/* 0X5F */ CMD_NOOP,                       // unexpected
/* 0X60 */ CMD_NOOP,                       // no thumb keys
/* 0X61 */ CMD_FWINLT,                     // thumb keys A
/* 0X62 */ CMD_LNUP,                       // thumb keys B
/* 0X63 */ CMD_TOP_LEFT,                   // thumb keys A,B
/* 0X64 */ CMD_CSRTRK,                     // thumb keys C
/* 0X65 */ CMD_LNBEG,                      // thumb keys A,C
/* 0X66 */ CMD_TOP,                        // thumb keys B,C
/* 0X67 */ CMD_NOOP,                       // thumb keys A,B,C
/* 0X68 */ CMD_LNDN,                       // thumb keys D
/* 0X69 */ CMD_HWINLT,                     // thumb keys A,D
/* 0X6A */ CMD_MUTE,                       // thumb keys B,D
/* 0X6B */ CMD_NOOP,                       // thumb keys A,B,D
/* 0X6C */ CMD_BOT,                        // thumb keys C,D
/* 0X6D */ CMD_NOOP,                       // thumb keys A,C,D
/* 0X6E */ CMD_NOOP,                       // thumb keys B,C,D
/* 0X6F */ CMD_NOOP,                       // thumb keys A,B,C,D
/* 0X70 */ CMD_FWINRT,                     // thumb keys E
/* 0X71 */ CMD_SAY,                        // thumb keys A,E
/* 0X72 */ CMD_HWINRT,                     // thumb keys B,E
/* 0X73 */ CMD_NOOP,                       // thumb keys A,B,E
/* 0X74 */ CMD_LNEND,                      // thumb keys C,E
/* 0X75 */ CMD_NOOP,                       // thumb keys A,C,E
/* 0X76 */ CMD_NOOP,                       // thumb keys B,C,E
/* 0X77 */ CMD_NOOP,                       // thumb keys A,B,C,E
/* 0X78 */ CMD_BOT_LEFT,                   // thumb keys D,E
/* 0X79 */ CMD_NOOP,                       // thumb keys A,D,E
/* 0X7A */ CMD_NOOP,                       // thumb keys B,D,E
/* 0X7B */ CMD_NOOP,                       // thumb keys A,B,D,E
/* 0X7C */ CMD_NOOP,                       // thumb keys C,D,E
/* 0X7D */ CMD_NOOP,                       // thumb keys A,C,D,E
/* 0X7E */ CMD_NOOP,                       // thumb keys B,C,D,E
/* 0X7F */ CMD_NOOP,                       // thumb keys A,B,C,D,E
/* 0X80 */ CR_BEGBLKOFFSET + VAL_ARG_MASK, // status key 1
/* 0X81 */ CR_ENDBLKOFFSET + VAL_ARG_MASK, // status key 2
/* 0X82 */ CMD_PREFMENU,                   // status key 3
/* 0X83 */ CMD_PREFLOAD,                   // status key 4
/* 0X84 */ CMD_FREEZE,                     // status key 5
/* 0X85 */ CMD_HELP,                       // status key 6
/* 0X86 */ CR_ROUTEOFFSET +   0,           // routing key 1
/* 0X87 */ CR_ROUTEOFFSET +   1,           // routing key 2
/* 0X88 */ CR_ROUTEOFFSET +   2,           // routing key 3
/* 0X89 */ CR_ROUTEOFFSET +   3,           // routing key 4
/* 0X8A */ CR_ROUTEOFFSET +   4,           // routing key 5
/* 0X8B */ CR_ROUTEOFFSET +   5,           // routing key 6
/* 0X8C */ CR_ROUTEOFFSET +   6,           // routing key 7
/* 0X8D */ CR_ROUTEOFFSET +   7,           // routing key 8
/* 0X8E */ CR_ROUTEOFFSET +   8,           // routing key 9
/* 0X8F */ CR_ROUTEOFFSET +   9,           // routing key 10
/* 0X90 */ CR_ROUTEOFFSET +  10,           // routing key 11
/* 0X91 */ CR_ROUTEOFFSET +  11,           // routing key 12
/* 0X92 */ CR_ROUTEOFFSET +  12,           // routing key 13
/* 0X93 */ CR_ROUTEOFFSET +  13,           // routing key 14
/* 0X94 */ CR_ROUTEOFFSET +  14,           // routing key 15
/* 0X95 */ CR_ROUTEOFFSET +  15,           // routing key 16
/* 0X96 */ CR_ROUTEOFFSET +  16,           // routing key 17
/* 0X97 */ CR_ROUTEOFFSET +  17,           // routing key 18
/* 0X98 */ CR_ROUTEOFFSET +  18,           // routing key 19
/* 0X99 */ CR_ROUTEOFFSET +  19,           // routing key 20
/* 0X9A */ CR_ROUTEOFFSET +  20,           // routing key 21
/* 0X9B */ CR_ROUTEOFFSET +  21,           // routing key 22
/* 0X9C */ CR_ROUTEOFFSET +  22,           // routing key 23
/* 0X9D */ CR_ROUTEOFFSET +  23,           // routing key 24
/* 0X9E */ CR_ROUTEOFFSET +  24,           // routing key 25
/* 0X9F */ CR_ROUTEOFFSET +  25,           // routing key 26
/* 0XA0 */ CR_ROUTEOFFSET +  26,           // routing key 27
/* 0XA1 */ CR_ROUTEOFFSET +  27,           // routing key 28
/* 0XA2 */ CR_ROUTEOFFSET +  28,           // routing key 29
/* 0XA3 */ CR_ROUTEOFFSET +  29,           // routing key 30
/* 0XA4 */ CR_ROUTEOFFSET +  30,           // routing key 31
/* 0XA5 */ CR_ROUTEOFFSET +  31,           // routing key 32
/* 0XA6 */ CR_ROUTEOFFSET +  32,           // routing key 33
/* 0XA7 */ CR_ROUTEOFFSET +  33,           // routing key 34
/* 0XA8 */ CR_ROUTEOFFSET +  34,           // routing key 35
/* 0XA9 */ CR_ROUTEOFFSET +  35,           // routing key 36
/* 0XAA */ CR_ROUTEOFFSET +  36,           // routing key 37
/* 0XAB */ CR_ROUTEOFFSET +  37,           // routing key 38
/* 0XAC */ CR_ROUTEOFFSET +  38,           // routing key 39
/* 0XAD */ CR_ROUTEOFFSET +  39,           // routing key 40
/* 0XAE */ CR_ROUTEOFFSET +  40,           // routing key 41
/* 0XAF */ CR_ROUTEOFFSET +  41,           // routing key 42
/* 0XB0 */ CR_ROUTEOFFSET +  42,           // routing key 43
/* 0XB1 */ CR_ROUTEOFFSET +  43,           // routing key 44
/* 0XB2 */ CR_ROUTEOFFSET +  44,           // routing key 45
/* 0XB3 */ CR_ROUTEOFFSET +  45,           // routing key 46
/* 0XB4 */ CR_ROUTEOFFSET +  46,           // routing key 47
/* 0XB5 */ CR_ROUTEOFFSET +  47,           // routing key 48
/* 0XB6 */ CR_ROUTEOFFSET +  48,           // routing key 49
/* 0XB7 */ CR_ROUTEOFFSET +  49,           // routing key 50
/* 0XB8 */ CR_ROUTEOFFSET +  50,           // routing key 51
/* 0XB9 */ CR_ROUTEOFFSET +  51,           // routing key 52
/* 0XBA */ CR_ROUTEOFFSET +  52,           // routing key 53
/* 0XBB */ CR_ROUTEOFFSET +  53,           // routing key 54
/* 0XBC */ CR_ROUTEOFFSET +  54,           // routing key 55
/* 0XBD */ CR_ROUTEOFFSET +  55,           // routing key 56
/* 0XBE */ CR_ROUTEOFFSET +  56,           // routing key 57
/* 0XBF */ CR_ROUTEOFFSET +  57,           // routing key 58
/* 0XC0 */ CR_ROUTEOFFSET +  58,           // routing key 59
/* 0XC1 */ CR_ROUTEOFFSET +  59,           // routing key 60
/* 0XC2 */ CR_ROUTEOFFSET +  60,           // routing key 61
/* 0XC3 */ CR_ROUTEOFFSET +  61,           // routing key 62
/* 0XC4 */ CR_ROUTEOFFSET +  62,           // routing key 63
/* 0XC5 */ CR_ROUTEOFFSET +  63,           // routing key 64
/* 0XC6 */ CR_ROUTEOFFSET +  64,           // routing key 65
/* 0XC7 */ CR_ROUTEOFFSET +  65,           // routing key 66
/* 0XC8 */ CR_ROUTEOFFSET +  66,           // routing key 67
/* 0XC9 */ CR_ROUTEOFFSET +  67,           // routing key 68
/* 0XCA */ CR_ROUTEOFFSET +  68,           // routing key 69
/* 0XCB */ CR_ROUTEOFFSET +  69,           // routing key 70
/* 0XCC */ CR_ROUTEOFFSET +  70,           // routing key 71
/* 0XCD */ CR_ROUTEOFFSET +  71,           // routing key 72
/* 0XCE */ CR_ROUTEOFFSET +  72,           // routing key 73
/* 0XCF */ CR_ROUTEOFFSET +  73,           // routing key 74
/* 0XD0 */ CR_ROUTEOFFSET +  74,           // routing key 75
/* 0XD1 */ CR_ROUTEOFFSET +  75,           // routing key 76
/* 0XD2 */ CR_ROUTEOFFSET +  76,           // routing key 77
/* 0XD3 */ CR_ROUTEOFFSET +  77,           // routing key 78
/* 0XD4 */ CR_ROUTEOFFSET +  78,           // routing key 79
/* 0XD5 */ CR_ROUTEOFFSET +  79,           // routing key 80
/* 0XD6 */ CR_ROUTEOFFSET +  80,           // routing key 81
/* 0XD7 */ CR_ROUTEOFFSET +  81,           // routing key 82
/* 0XD8 */ CR_ROUTEOFFSET +  82,           // routing key 83
/* 0XD9 */ CR_ROUTEOFFSET +  83,           // routing key 84
/* 0XDA */ CR_ROUTEOFFSET +  84,           // routing key 85
/* 0XDB */ CR_ROUTEOFFSET +  85,           // routing key 86
/* 0XDC */ CR_ROUTEOFFSET +  86,           // routing key 87
/* 0XDD */ CR_ROUTEOFFSET +  87,           // routing key 88
/* 0XDE */ CR_ROUTEOFFSET +  88,           // routing key 89
/* 0XDF */ CR_ROUTEOFFSET +  89,           // routing key 90
/* 0XE0 */ CR_ROUTEOFFSET +  90,           // routing key 91
/* 0XE1 */ CR_ROUTEOFFSET +  91,           // routing key 92
/* 0XE2 */ CR_ROUTEOFFSET +  92,           // routing key 93
/* 0XE3 */ CR_ROUTEOFFSET +  93,           // routing key 94
/* 0XE4 */ CR_ROUTEOFFSET +  94,           // routing key 95
/* 0XE5 */ CR_ROUTEOFFSET +  95,           // routing key 96
/* 0XE6 */ CR_ROUTEOFFSET +  96,           // routing key 97
/* 0XE7 */ CR_ROUTEOFFSET +  97,           // routing key 98
/* 0XE8 */ CR_ROUTEOFFSET +  98,           // routing key 99
/* 0XE9 */ CR_ROUTEOFFSET +  99,           // routing key 100
/* 0XEA */ CR_ROUTEOFFSET + 100,           // routing key 101
/* 0XEB */ CR_ROUTEOFFSET + 101,           // routing key 102
/* 0XEC */ CR_ROUTEOFFSET + 102,           // routing key 103
/* 0XED */ CR_ROUTEOFFSET + 103,           // routing key 104
/* 0XEE */ CR_ROUTEOFFSET + 104,           // routing key 105
/* 0XEF */ CR_ROUTEOFFSET + 105,           // routing key 106
/* 0XF0 */ CR_ROUTEOFFSET + 106,           // routing key 107
/* 0XF1 */ CR_ROUTEOFFSET + 107,           // routing key 108
/* 0XF2 */ CR_ROUTEOFFSET + 108,           // routing key 109
/* 0XF3 */ CR_ROUTEOFFSET + 109,           // routing key 110
/* 0XF4 */ CR_ROUTEOFFSET + 110,           // routing key 111
/* 0XF5 */ CR_ROUTEOFFSET + 111,           // routing key 112
/* 0XF6 */ CR_ROUTEOFFSET + 112,           // routing key 113
/* 0XF7 */ CR_ROUTEOFFSET + 113,           // routing key 114
/* 0XF8 */ CR_ROUTEOFFSET + 114,           // routing key 115
/* 0XF9 */ CR_ROUTEOFFSET + 115,           // routing key 116
/* 0XFA */ CR_ROUTEOFFSET + 116,           // routing key 117
/* 0XFB */ CR_ROUTEOFFSET + 117,           // routing key 118
/* 0XFC */ CR_ROUTEOFFSET + 118,           // routing key 119
/* 0XFD */ CR_ROUTEOFFSET + 119,           // routing key 120
/* 0XFE */ CR_ROUTEOFFSET + 120,           // routing key 121
/* 0XFF */ CR_ROUTEOFFSET + 121            // routing key 122