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

#include <stdio.h>
#include <termios.h>

/* misc.h - Header file for miscellaneous all-purpose routines
 */

#define MIN(a, b)  (((a) < (b))? (a): (b)) 
#define MAX(a, b)  (((a) > (b))? (a): (b)) 
	
// Process each line of an input text file safely.
// This routine handles the actual reading of the file,
// insuring that the input buffer is always big enough,
// and calls a caller-supplied handler once for each line in the file.
// The caller-supplied data pointer is passed straight through to the handler.
extern int processLines (FILE *file, // The input file.
                    void (*handler) (char *line, void *data), // The input line handler.
		    void *data); // A pointer to caller-specific data.

// Safe wrappers for system calls which handle the various scenarios
// which can occur when signals interrupt them.
// These wrappers, rather than the system calls they wrap, should be
// called from within all drivers since certain tasks, i.e. cursor routing,
// are performed by forked processes which generate a SIGCHLD when they terminate.
// This approach also makes it safe for internal events to be scheduled
// via signals like SIGALRM and SIGIO.
size_t safe_read (int fd, unsigned char *buffer, size_t length);
size_t safe_write (int fd, const unsigned char *buffer, size_t length);

#define __EXTENSIONS__
#include <sys/time.h>
#include <errno.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

extern void setCloseOnExec (int fileDescriptor);

extern void delay (int msec);		/* sleep for `msec' milliseconds */
extern void shortdelay (unsigned msec);
extern unsigned long elapsed_msec (struct timeval *t1, struct timeval *t2);
extern int timeout_yet (int msec);	/* test timeout condition */

/* should syslog be used? may not be portable... */
#define USE_SYSLOG 1
#ifdef USE_SYSLOG
   #include <syslog.h>
#else
   typedef enum {
      LOG_EMERG,
      LOG_ALERT,
      LOG_CRIT,
      LOG_ERR,
      LOG_WARNING,
      LOG_NOTICE,
      LOG_INFO,
      LOG_DEBUG
   } SyslogLevel;
#endif
extern void LogOpen(int toConsole);
extern void LogClose(void);
extern void LogPrint(int level, char *format, ...);
extern void LogError (const char *action);
extern void SetLogLevel(int level);
extern void SetStderrLevel(int level);
extern void SetStderrOff(void);
extern int ProblemCount;

extern int getConsole (void);
extern int writeConsole (const unsigned char *address, size_t count);
extern int ringBell (void);
extern int timedBeep (unsigned short frequency, unsigned short milliseconds);

extern void *mallocWrapper (size_t size);
extern void *reallocWrapper (void *address, size_t size);
extern char *strdupWrapper (const char *string);

extern int validateInteger (int *integer, const char *description, const char *value, const int *minimum, const int *maximum);
extern int validateBaud (speed_t *baud, const char *description, const char *value, const unsigned int *choices);
extern int baud2integer (speed_t baud);
extern int validateChoice (unsigned int *choice, const char *description, const char *value, const char *const *choices);
extern int validateFlag (unsigned int *flag, const char *description, const char *value, const char *on, const char *off);
extern int validateOnOff (unsigned int *flag, const char *description, const char *value);
extern int validateYesNo (unsigned int *flag, const char *description, const char *value);

/* Formatting of status cells. */
extern int landscape_number(int x);
extern int landscape_flag(int number, int on);
extern int seascape_number(int x);
extern int seascape_flag(int number, int on);
extern int portrait_number(int x);
extern int portrait_flag(int number, int on);

#define MAXNSTATCELLS 22
extern unsigned char statcells[MAXNSTATCELLS];	/* status cell buffer */
extern unsigned char texttrans[0X100];	 /* current text to braille translation table */
extern unsigned char untexttrans[0X100]; /* current braille to text translation table */
extern void reverseTable(unsigned char *origtab, unsigned char *revtab);
extern unsigned char attribtrans[0X100]; /* current attributes to braille translation table */

#ifdef __cplusplus
}
#endif /* __cplusplus */