//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//  * Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//  * Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//  * Neither the name of the project nor the
//    names of its contributors may be used to endorse or promote products
//    derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//------------------------------------------------------------------------------


#ifndef scene_h
#define scene_h

#include <SDL2/SDL.h>
#include <stdio.h>
#include "math.h"
#include "types.h"
#include "io.h"
#include "dif.h"
#include "texture.h"
#include "sphere.h"
#include "physics.h"
#include "shader.h"
#include "scene.h"

#include <SDL2/SDL.h>
#include <OpenGL/gl.h>
#include <OpenGL/gl3.h>
#include <OpenGL/glu.h>

#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/vec3.hpp>

class Scene {
protected:
	SDL_Window *window;
	SDL_GLContext context;

	void (*loopCallback)();

	bool running;
	bool printFPS = false;

	F32 maxFPS = 60.0f;

	GLfloat angle;

	F32 yaw;
	F32 pitch;
	glm::vec3 cameraPosition;

	GLuint mvpMatrix;
	glm::mat4x4 projectionMatrix, modelviewMatrix;

	struct {
		bool hasSelection;
		U32 surfaceIndex;
		Interior *interior;
	} selection;

	GLuint displayList;
	bool listNeedsDisplay = true;

	bool captureMouse = false;
	bool mouseButtons[3] = {false, false, false};
	bool movement[9] = {false, false, false, false, false, false, false, false, false};

	const float cameraSpeed = 0.3f;
	const float keyCameraSpeed = 3.f;
	const float movementSpeed = 0.2f;

	const F32 lightColor[4]     = {1.100000f, 1.100000f, 0.900000f, 1.000000f};
	const F32 lightDirection[4] = {0.60f, 0.40f, 1.0f, 0.0f};
	const F32 ambientColor[4]   = {0.600000f, 0.600000f, 0.800000f, 1.000000f};
	const F32 diffuseColor[4]   = {0.800000f, 0.800000f, 1.000000f, 1.000000f};
public:
	U32 difCount;
	DIF **difs;
	String **filenames;
	Sphere *sphere;

	static Scene *getSingleton() {
		static Scene *singleton;
		if (!singleton)
			singleton = new Scene();
		return singleton;
	}
	void setLoopCallback(void (*callback)()) {
		loopCallback = callback;
	}

	void render();
	void loop();
	bool initGL();
	bool init();
	void cleanup();
	void run();
	void performClick(S32 mouseX, S32 mouseY);
	void handleEvent(SDL_Event *event);
};

#endif