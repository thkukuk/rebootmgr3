/* Copyright (c) 2016, 2017, 2024 Thorsten Kukuk
   Author: Thorsten Kukuk <kukuk@suse.com>

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation in version 2 of the License.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License along
   with this program; if not, see <http://www.gnu.org/licenses/>. */

#include "config.h"

#include <time.h>
#include <stdarg.h>
#include <stdio.h>
#include <syslog.h>
#include <unistd.h>
#include "log_msg.h"

int debug_flag = 0;

void
log_msg (int type, const char *fmt,...)
{
  char string[400];
  va_list ap;

  va_start (ap, fmt);
  vsnprintf (string, sizeof (string), fmt, ap);
  va_end (ap);

  if (debug_flag)
    fprintf (stderr, "%s\n", string);

  if (type != LOG_DEBUG)
    syslog (type, "%s", string); // XXX print to journal
}
