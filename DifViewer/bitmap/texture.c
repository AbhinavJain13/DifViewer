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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <OpenGL/glu.h>
#include "texture.h"

#define TEXTURE_MAX_SIZE 1024

Texture *texture_create_from_pixels(U8 *pixels, Point2I extent) {
	if (extent.x > TEXTURE_MAX_SIZE || extent.y > TEXTURE_MAX_SIZE) {
		printf("Texture too large! (%d, %d) > (%d, %d). Bug HiGuy to make textures larger.", extent.x, extent.y, TEXTURE_MAX_SIZE, TEXTURE_MAX_SIZE);
		return NULL;
	}

	Texture *texture = malloc(sizeof(Texture));

	//Set some fields
	texture->extent = extent;
	texture->generated = false;

	//Load pixels into texture->pixels (assume RGBA)
	texture->pixels = malloc(sizeof(U8) * extent.x * extent.y * 4);
	memcpy(texture->pixels, pixels, sizeof(U8) * extent.x * extent.y * 4);

	return texture;
}

void texture_generate_buffer(Texture *texture) {
	//Just in case
	glEnable(GL_TEXTURE_2D);

	//Set mode
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	//Generate the texture buffer
	glGenTextures(1, &texture->buffer);
	glBindTexture(GL_TEXTURE_2D, texture->buffer);

	//Set some flags
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	//Actually create the texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->extent.x, texture->extent.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture->pixels);

	texture->generated = true;
}

void texture_release(Texture *texture) {
	//Clean up
	if (texture->generated)
		glDeleteTextures(1, &texture->buffer);

	free(texture->pixels);
	free(texture);
}

void texture_activate(Texture *texture) {
	//Error check
	if (!texture->generated)
		return;
	//Activate and bind the buffer
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glBindTexture(GL_TEXTURE_2D, texture->buffer);
}

void texture_deactivate(Texture *texture) {
	//Haha, this method is just BS. Fooled you.
	glDisable(GL_TEXTURE_2D);
}