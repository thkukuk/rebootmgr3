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

#pragma once

#include "rebootmgr.h"
#include "calendarspec.h"

int mkdir_p(const char *path, mode_t mode);
const char *bool_to_str(bool var);
int rm_duration_to_string(time_t duration, const char **ret);
int rm_string_to_strategy(const char *str_strategy, RM_RebootStrategy *ret);
int rm_strategy_to_str(RM_RebootStrategy strategy, const char **ret);
int rm_status_to_str(RM_RebootStatus status, RM_RebootMethod method,
		     const char **ret);
int rm_method_to_str(RM_RebootMethod method, const char **ret);
