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

#include <stdio.h>
#include <stdlib.h>
#include "interior.h"
#include "math.h"

void Interior::render() {
#ifdef GL_33
	if (!renderInfo.generated) {
		Triangle **perMaterialTriangles = new Triangle*[numMaterials];

		U32 *materialTriangles = new U32[numMaterials];
		renderInfo.numMaterialTriangles = new U32[numMaterials];

		U32 numTriangles = 0;

		for (U32 i = 0; i < numMaterials; i ++) {
			renderInfo.numMaterialTriangles[i] = 0;
		}

		//Generate our triangle mesh for rendering
		for (U32 i = 0; i < numSurfaces; i ++) {
			Surface surface = this->surface[i];
			renderInfo.numMaterialTriangles[surface.textureIndex] += surface.windingCount - 2;
			numTriangles += surface.windingCount - 2;
		}

		//Load all the textures
		for (U32 i = 0; i < numMaterials; i ++) {
			perMaterialTriangles[i] = new Triangle[renderInfo.numMaterialTriangles[i]];
			materialTriangles[i] = 0;

			Material *mat = material[i];
			if (mat) {
				mat->generate();
			}
		}
		noise->generate();

		for (U32 i = 0; i < numSurfaces; i ++) {
			Surface surface = this->surface[i];

			//New and improved rendering with actual Triangle Strips this time
			for (U32 j = surface.windingStart + 2; j < surface.windingStart + surface.windingCount; j ++) {
				Point3F n = normal[plane[surface.planeIndex].normalIndex];
				if (surface.planeFlipped) {
					n *= -1;
				}
				Point3F u0, u1, u2;

				if ((j - (surface.windingStart + 2)) % 2 == 0) {
					u0 = point[index[j]];
					u1 = point[index[j - 1]];
					u2 = point[index[j - 2]];
				} else {
					u0 = point[index[j - 2]];
					u1 = point[index[j - 1]];
					u2 = point[index[j]];
				}

				TexGenEq texGenEq = this->texGenEq[surface.texGenIndex];

				Point2F uv0 = Point2F(planeF_distance_to_point(texGenEq.planeX, u0), planeF_distance_to_point(texGenEq.planeY, u0));
				Point2F uv1 = Point2F(planeF_distance_to_point(texGenEq.planeX, u1), planeF_distance_to_point(texGenEq.planeY, u1));
				Point2F uv2 = Point2F(planeF_distance_to_point(texGenEq.planeX, u2), planeF_distance_to_point(texGenEq.planeY, u2));

				perMaterialTriangles[surface.textureIndex][materialTriangles[surface.textureIndex]].verts[0].point = u0;
				perMaterialTriangles[surface.textureIndex][materialTriangles[surface.textureIndex]].verts[1].point = u1;
				perMaterialTriangles[surface.textureIndex][materialTriangles[surface.textureIndex]].verts[2].point = u2;
				perMaterialTriangles[surface.textureIndex][materialTriangles[surface.textureIndex]].verts[0].uv = uv0;
				perMaterialTriangles[surface.textureIndex][materialTriangles[surface.textureIndex]].verts[1].uv = uv1;
				perMaterialTriangles[surface.textureIndex][materialTriangles[surface.textureIndex]].verts[2].uv = uv2;
				perMaterialTriangles[surface.textureIndex][materialTriangles[surface.textureIndex]].verts[0].normal = n;
				perMaterialTriangles[surface.textureIndex][materialTriangles[surface.textureIndex]].verts[1].normal = n;
				perMaterialTriangles[surface.textureIndex][materialTriangles[surface.textureIndex]].verts[2].normal = n;
				materialTriangles[surface.textureIndex] ++;
			}
		}
		Triangle *triangle = new Triangle[numTriangles];

		//Load all the textures
		numTriangles = 0;
		for (U32 i = 0; i < numMaterials; i ++) {
			for (U32 j = 0; j < materialTriangles[i]; j ++) {
				triangle[numTriangles] = perMaterialTriangles[i][j];
				numTriangles ++;
			}
			delete [] perMaterialTriangles[i];
		}
		delete [] perMaterialTriangles;

		//Generate us a vertex buffer
		glGenBuffers(1, &renderInfo.vertexBuffer);

		//Use the vertex buffer
		glBindBuffer(GL_ARRAY_BUFFER, renderInfo.vertexBuffer);

		//Upload the buffer data to OpenGL
		glBufferData(GL_ARRAY_BUFFER, sizeof(Triangle) * numTriangles, triangle, GL_STATIC_DRAW);

		delete [] triangle;

		renderInfo.generated = true;
	}

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, renderInfo.vertexBuffer);

	//0th array - vertices
	glVertexAttribPointer(0, //Attribute 0
						  3, //3 components
						  GL_FLOAT, //Type
						  GL_FALSE, //Normalized
						  sizeof(Vertex), //Stride
						  (void *)offsetof(Vertex, point)); //Offset

	//1st array - uvs
	glVertexAttribPointer(1, //Attribute 1
						  2, //2 components
						  GL_FLOAT, //Type
						  GL_FALSE, //Normalized
						  sizeof(Vertex), //Stride
						  (void *)offsetof(Vertex, uv)); //Offset

	//2nd array - normals
	glVertexAttribPointer(2, //Attribute 2
						  3, //3 components
						  GL_FLOAT, //Type
						  GL_FALSE, //Normalized
						  sizeof(Vertex), //Stride
						  (void *)offsetof(Vertex, normal)); //Offset

	noise->activate();
	U32 start = 0;
	for (U32 i = 0; i < numMaterials; i ++) {
		if (renderInfo.numMaterialTriangles[i] > 0) {
			if (material[i]) {
				material[i]->activate();
			}
			glDrawArrays(GL_TRIANGLES, start * 3, renderInfo.numMaterialTriangles[i] * 3);
			start += renderInfo.numMaterialTriangles[i];
			if (material[i]) {
				material[i]->deactivate();
			}
		}
	}
	noise->deactivate();

	//Disable arrays
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

#else
	//Actual rendering is here (GL 1.1 in a 2.1 context. Take THAT, good practice!)

	glEnable(GL_TEXTURE_2D);
	Texture *currentTexture = NULL;

	//Draw all the surfaces
	glBegin(GL_TRIANGLES);
	for (U32 i = 0; i < numSurfaces; i ++) {
		Surface surface = this->surface[i];

		if (this->texture) {
			Texture *texture = this->texture[surface.textureIndex];
			//Make sure our texture is active before drawing
			if (texture && texture != currentTexture) {
				glEnd();
				if (currentTexture)
					currentTexture->deactivate();

				if (surface.textureIndex < numMaterials) {
					//Generate if needed
					if (!texture->generated) {
						texture->generateBuffer();
					}
					texture->activate();
				} else {
					glDisable(GL_LIGHTING);
				}

				currentTexture = texture;
				glBegin(GL_TRIANGLES);
			} else {
				glEnd();
				if (currentTexture)
					currentTexture->deactivate();
				currentTexture = nullptr;
				glBegin(GL_TRIANGLES);
			}
		}

		//New and improved rendering with actual Triangle Strips this time
		for (U32 j = surface.windingStart + 2; j < surface.windingStart + surface.windingCount; j ++) {
			Point3F n = normal[plane[surface.planeIndex].normalIndex];
			if (surface.planeFlipped) {
				n *= -1;
			}
			Point3F u0, u1, u2;

			if ((j - (surface.windingStart + 2)) % 2 == 0) {
				u0 = point[index[j]];
				u1 = point[index[j - 1]];
				u2 = point[index[j - 2]];
			} else {
				u0 = point[index[j - 2]];
				u1 = point[index[j - 1]];
				u2 = point[index[j]];
			}

			TexGenEq texGenEq = this->texGenEq[surface.texGenIndex];

			//Reference: TGE InteriorRender.cc
			glNormal3f(n.x, n.y, n.z);
			glTexCoord2f(planeF_distance_to_point(texGenEq.planeX, u0), planeF_distance_to_point(texGenEq.planeY, u0));
			glVertex3f(u0.x, u0.y, u0.z);

			glTexCoord2f(planeF_distance_to_point(texGenEq.planeX, u1), planeF_distance_to_point(texGenEq.planeY, u1));
			glVertex3f(u1.x, u1.y, u1.z);

			glTexCoord2f(planeF_distance_to_point(texGenEq.planeX, u2), planeF_distance_to_point(texGenEq.planeY, u2));
			glVertex3f(u2.x, u2.y, u2.z);
		}
	}
	glEnd();

	//Don't forget to deactivate the last texture
	if (currentTexture)
		currentTexture->deactivate();

	glDisable(GL_TEXTURE_2D);
#endif
}
