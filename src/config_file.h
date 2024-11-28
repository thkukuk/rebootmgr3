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

#define SET_STRATEGY 1
#define SET_MAINT_WINDOW 2

int load_config (RM_CTX *ctx);
void save_config (RM_CTX *ctx, int field);
