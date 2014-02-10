/*
 * BRLTTY - A background process providing access to the console screen (when in
 *          text mode) for a blind person using a refreshable braille display.
 *
 * Copyright (C) 1995-2014 by The BRLTTY Developers.
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

#include "prologue.h"

#include <stdio.h>
#include <string.h>

#include "log.h"
#include "params.h"

char *
getBootParameters (const char *name) {
  size_t nameLength = strlen(name);
  char *parameters;

  if ((parameters = strdup(""))) {
    const char *path = "/proc/cmdline";
    FILE *file;

    if ((file = fopen(path, "r"))) {
      char buffer[0X1000];
      char *line = fgets(buffer, sizeof(buffer), file);

      if (line) {
        char *token;

        while ((token = strtok(line, " \n"))) {
          line = NULL;

          if ((strncmp(token, name, nameLength) == 0) &&
              (token[nameLength] == '=')) {
            char *newParameters = strdup(token + nameLength + 1);

            if (newParameters) {
              free(parameters);
              parameters = newParameters;
            } else {
              logMallocError();
            }
          }
        }
      }

      fclose(file);
    }
  } else {
    logMallocError();
  }

  return parameters;
}