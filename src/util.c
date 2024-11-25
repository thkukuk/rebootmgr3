/* Copyright (c) 2016 Daniel Molkentin
   Author: Daniel Molkentin <dmolkentin@suse.com>

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation in version 2 of the License.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License along
   with this program; if not, see <http://www.gnu.org/licenses/>. */


#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

#include "calendarspec.h"
#include "parse-duration.h"

#include "util.h"

#define DURATION_LEN 10

const char *
bool_to_str (bool var)
{
  return var?"true":"false";
}

/* XXX */
char *
duration_to_string (time_t duration)
{
  char buf[DURATION_LEN];
  char *p;
  if (strftime(buf, DURATION_LEN, "%Hh%Mm", gmtime(&duration)) == 0) {
    return 0;
  }
  p = malloc(DURATION_LEN);
  strncpy(p, buf, DURATION_LEN);

  return p;
}

RM_RebootStrategy
string_to_strategy(const char *str_strategy, int *error)
{
  if (error) {
    *error=0;
  }
  if (!str_strategy) {
    if (error) {
      *error=1;
    }
    return RM_REBOOTSTRATEGY_BEST_EFFORT;
  }

  if (strcasecmp (str_strategy, "best-effort") == 0 ||
      strcasecmp (str_strategy, "best_effort") == 0)
    return RM_REBOOTSTRATEGY_BEST_EFFORT;
  else if (strcasecmp (str_strategy, "instantly") == 0)
    return RM_REBOOTSTRATEGY_INSTANTLY;
  else if (strcasecmp (str_strategy, "maint_window") == 0 ||
     strcasecmp (str_strategy, "maint-window") == 0)
    return RM_REBOOTSTRATEGY_MAINT_WINDOW;
  else if (strcasecmp (str_strategy, "off") == 0)
    return RM_REBOOTSTRATEGY_OFF;

  if (error)
    *error=1;

  return RM_REBOOTSTRATEGY_BEST_EFFORT;
}

int
rm_status_to_str (RM_RebootStatus status, RM_RebootMethod method,
		  const char **ret)
{
  switch (status)
    {
    case RM_REBOOTSTATUS_NOT_REQUESTED:
      *ret = _("Reboot not requested");
      break;
    case RM_REBOOTSTATUS_REQUESTED:
      if (method == RM_REBOOTMETHOD_SOFT)
	*ret = _("Soft-reboot requested");
      else
	*ret = _("Reboot requested");
      break;
    case RM_REBOOTSTATUS_WAITING_WINDOW:
      if (method == RM_REBOOTMETHOD_SOFT)
	*ret = _("Soft-reboot requested, waiting for maintenance window");
      else
	*ret = _("Reboot requested, waiting for maintenance window");
      break;
    default:
      *ret = NULL;
      return -EINVAL;
    }
  return 0;
}

int
rm_strategy_to_str (RM_RebootStrategy strategy, const char **ret)
{
  switch (strategy) {
  case RM_REBOOTSTRATEGY_BEST_EFFORT:
    *ret = "best-effort";
    break;
  case RM_REBOOTSTRATEGY_INSTANTLY:
    *ret = "instantly";
    break;
  case RM_REBOOTSTRATEGY_MAINT_WINDOW:
    *ret = "maint-window";
    break;
  case RM_REBOOTSTRATEGY_OFF:
    *ret = "off";
    break;
  case RM_REBOOTSTRATEGY_UNKNOWN:
    /* fall through, not a valid entry, too */
  default:
    *ret = "unknown";
    return -EINVAL;
  }
  return 0;
}

int
rm_method_to_str (RM_RebootMethod method, const char **ret)
{
  switch (method) {
  case RM_REBOOTMETHOD_HARD:
    *ret = "reboot";
    break;
  case RM_REBOOTMETHOD_SOFT:
    *ret = "soft-reboot";
    break;
  case RM_REBOOTMETHOD_UNKNOWN:
  default:
    *ret = "unknown";
    return -EINVAL;
  }
  return 0;
}
