//------------------------------------------------------------------------------
// Copyright (c) 2014 Glenn Smith
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//  * Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//  * Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//  * Neither the name of the <organization> nor the
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

#ifndef forceField_h
#define forceField_h

#include "types.h"

typedef struct {
	U32 normalIndex;
	F32 planeDistance;
} Plane_FF;

typedef struct {
	U16 planeIndex;
	U16 frontIndex;
	U16 backIndex;
} BSPNode_FF;

typedef struct {
	U32 surfaceIndex;
	U16 surfaceCount;
} BSPSolidLeaf_FF;

typedef struct {
	U32 windingStart;
	U8 windingCount;
	U16 planeIndex;
	U8 surfaceFlags;
	U32 fanMask;
} Surface_FF;

typedef struct {
	U32 forceFieldFileVersion;
	String name;

	U32 numTriggers;
	String *trigger;

	BoxF boundingBox;
	SphereF boundingSphere;

	U32 numNormals;
	Point3F *normal;

	U32 numPlanes;
	Plane_FF *plane;

	U32 numBSPNodes;
	BSPNode_FF *BSPNode;

	U32 numBSPSolidLeaves;
	BSPSolidLeaf_FF *BSPSolidLeaf;

	U32 numWindings;
	U32 *index;

	U32 numSurfaces;
	Surface_FF *surface;

	U32 numSolidLeafSurfaces;
	U32 *solidLeafSurface;

	ColorI color;
} ForceField;

/**
 Reads a ForceField from a FILE
 @arg file - The FILE to read from (updates position)
 @return A ForceField
 */
ForceField *forceField_read_file(FILE *file);

/**
 Frees the ForceField and all memory contained within it
 @arg forceField - The ForceField to release
 */
void forceField_release(ForceField *forceField);

#endif
