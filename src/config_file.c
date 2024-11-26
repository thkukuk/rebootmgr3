//SPDX-License-Identifier: GPL-2.0-or-later

/* Copyright (c) 2024 Thorsten Kukuk
   Author: Thorsten Kukuk <kukuk@suse.com>

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License along
   with this program; if not, see <http://www.gnu.org/licenses/>. */

#include "config.h"

#include <string.h>
#include <libeconf.h>

#include "calendarspec.h"
#include "parse-duration.h"
#include "config_file.h"
#include "log_msg.h"
#include "util.h"

#define RM_GROUP "rebootmgr"

int
load_config (RM_CTX *ctx)
{
  econf_file *key_file = NULL;
  int retval = 0;
  econf_err error;

  error = econf_readConfig (&key_file,
                            "rebootmgr",
                            "/usr/share",
                            "rebootmgrd",
                            "conf", "=", "#");
  if (error)
    {
      /* ignore if there is no configuration file at all */
      if (error == ECONF_NOFILE)
	return retval;

      log_msg (LOG_ERR, "econf_readConfig: %s\n",
               econf_errString (error));
      return -1;
    }

  if (key_file == NULL) /* can this happen? */
      log_msg (LOG_ERR, "Cannot load 'rebootmgrd.conf'");
  else
    {
      char *str_start = NULL, *str_duration = NULL, *str_strategy = NULL;

      error = econf_getStringValue (key_file, RM_GROUP, "window-start", &str_start);
      if (error && error != ECONF_NOKEY)
	{
	  log_msg (LOG_ERR, "ERROR (econf): cannot get key 'window-start': %s",
		   econf_errString(error));
	  retval = -1;
	  goto out;
	}
      error = econf_getStringValue (key_file, RM_GROUP, "window-duration", &str_duration);
      if (error && error != ECONF_NOKEY)
	{
	  log_msg (LOG_ERR, "ERROR (econf): cannot get key 'window-duration': %s",
		   econf_errString(error));
	  retval = -1;
	  goto out;
	}
      error = econf_getStringValue (key_file, RM_GROUP, "strategy", &str_strategy);
      if (error && error != ECONF_NOKEY)
	{
	  log_msg (LOG_ERR, "ERROR (econf): cannot get key 'strategy': %s",
		   econf_errString(error));
	  retval = -1;
	  goto out;
	}

      if (str_start == NULL && str_duration != NULL)
	{
	  free (str_duration);
	  str_duration = NULL;
	}
      int r = rm_string_to_strategy (str_strategy, &(ctx->reboot_strategy));
      if (r >= 0)
	{
	  int ret;

	  if ((ret = calendar_spec_from_string (str_start,
						&ctx->maint_window_start)) < 0)
	    log_msg (LOG_ERR, "ERROR: cannot parse window-start (%s): %s",
		     str_start, strerror (-ret));
	  if ((ctx->maint_window_duration =
	       parse_duration (str_duration)) == BAD_TIME)
	    log_msg (LOG_ERR, "ERROR: cannot parse window-duration '%s'",
		     str_duration);
	}
    out:
      econf_free (key_file);
      free (str_start);
      free (str_duration);
      free (str_strategy);
    }
  return retval;
}


/* XXX mark modified variables and write them into reboogmgrd.conf.d directories */
void
save_config (RM_CTX *ctx, int field)
{
#if 0 /* XXX needs a complete rewrite */
  econf_file *file = NULL;
  econf_err error;

  error = econf_readFile (&file, SYSCONFDIR"/rebootmgr.conf", "=", "#");
  if (error)
    {
      if (error != ECONF_NOFILE)
	{
	  log_msg (LOG_ERR, "Cannot load '"SYSCONFDIR"rebootmgr.conf': %s",
		   econf_errString(error));
	  return;
	}
      else
	{
	  if ((error = econf_newKeyFile (&file, '=', '#')))
	    {
	      log_msg (LOG_ERR, "Cannot create new config file: %s",
		       econf_errString(error));
	      return;
	    }
	}
    }

  switch (field)
    {
      char *p;
    case SET_STRATEGY:
      error = econf_setStringValue (file, RM_GROUP, "strategy",
				    strategy_to_string(ctx->reboot_strategy, NULL));
      break;
    case SET_MAINT_WINDOW:
      p = spec_to_string(ctx->maint_window_start);
      error = econf_setStringValue (file, RM_GROUP, "window-start", p);
      free (p);
      if (!error)
	{
	  p = duration_to_string(ctx->maint_window_duration);
	  error = econf_setStringValue (file, RM_GROUP, "window-duration", p);
	  free (p);
	}
      break;
    default:
      log_msg (LOG_ERR, "Error writing config file, unknown field %i", field);
      econf_free (file);
      return;
    }

  if (error)
    {
      log_msg (LOG_ERR, "Error setting variable: %s\n", econf_errString(error));
      econf_free (file);
      return;
    }

  if ((error = econf_writeFile(file, SYSCONFDIR"/", "rebootmgr.conf")))
    log_msg (LOG_ERR, "Error writing "SYSCONFDIR"/rebootmgr.conf: %s",
	     econf_errString(error));

  econf_free (file);
#endif
}
