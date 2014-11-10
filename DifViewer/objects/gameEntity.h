//------------------------------------------------------------------------------
// Copyright (C) 2014 Glenn Smith
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//------------------------------------------------------------------------------

#ifndef gameEntity_h
#define gameEntity_h

#include "types.h"

typedef struct {
	String datablock;
	String gameClass;
	Point3F position;
	Dictionary properties;
} GameEntity;

GameEntity *gameEntity_read_file(FILE *file);
void gameEntity_release(GameEntity *gameEntity);

#endif