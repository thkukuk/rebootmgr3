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

#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "rebootmgr.h"
#include "util.h"

static int
try_mkdir(const char* path, mode_t mode)
{
  struct stat st;

  /* Try to make the directory */
  if (mkdir(path, mode) == 0)
    return 0;

  /* If it fails for any reason but EEXIST, fail */
  if (errno != EEXIST)
    return -errno;

  /* Check if the existing path is a directory */
  if (stat(path, &st) != 0)
    return -errno;

  /* If not, fail with ENOTDIR */
  if (!S_ISDIR(st.st_mode))
    return -ENOTDIR;

  return 0;
}

int
mkdir_p(const char *path, mode_t mode)
{
  _cleanup_(freep) char *_path = NULL;
  char *p;
  int r;

  /* Copy string so it's mutable */
  _path = strdup(path);
  if (_path == NULL)
    return -EINVAL;

  /* Iterate the string */
  for (p = _path + 1; *p; p++)
    {
      if (*p == '/')
	{
	  /* Temporarily truncate */
	  *p = '\0';

	  r = try_mkdir(_path, mode);
	  if (r < 0)
	    return r;

	  *p = '/';
        }
    }

  r = try_mkdir(_path, mode);
  if (r < 0)
    return r;

  return 0;
}
