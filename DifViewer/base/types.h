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

#ifndef types_h
#define types_h

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//Base types (names stolen from TGE because lazy)
typedef unsigned char      U8;
typedef unsigned short     U16;
typedef unsigned int       U32;
typedef unsigned long long U64;

typedef signed char      S8;
typedef signed short     S16;
typedef signed int       S32;
typedef signed long long S64;

typedef float F32;
typedef double F64;

struct Readable {
public:
	bool read(FILE *file) { return false; };
};

struct Writable {
public:
	bool write(FILE *file) const { return false; };
};

struct String : public Readable, Writable {
	U8 *data;
	U32 length;
	bool allocated;

	inline operator const char *() const {
		return (const char *)data;
	}
	inline operator char *() const {
		return (char *)data;
	}
	inline operator U8 *() const {
		return data;
	}
	inline operator const void *() const {
		return data;
	}
	inline operator void *() const {
		return data;
	}
	inline bool operator==(const char *str) const {
		return strcmp((const char *)data, str) == 0;
	}
	inline bool operator==(const String &str) const {
		return strcmp((const char *)data, (const char *)str.data) == 0;
	}
	inline bool operator!=(const String &str) const {
		return !operator==(str);
	}
	inline String operator+=(const String &str) {
		concat(str);
		return *this;
	}
	inline String operator+(const String &str) const {
		String newStr = String(*this);
		newStr.concat(str);
		return newStr;
	}
	inline String operator+(const char &chr) const {
		String newStr = String(*this);
		newStr.concat(String(&chr));
		return newStr;
	}
	inline String concat(const String &str) {
		U8 *cur = new U8[length + 1];
		memcpy(cur, data, length + 1);
		data = (U8 *)realloc(data, length + str.length + 1);
		memcpy(data, cur, length);
		memcpy(data + length, str, str.length);
		length += str.length;
		memset(data + length, 0, 1);
		delete [] cur;
		return *this;
	}
	String() : data(new U8[1]), length(0) {
		data[length] = 0;
		allocated = true;
	}
	String(const U32 &length) : data(new U8[length + 1]), length(length) {
		data[length] = 0;
		allocated = true;
	}
	String(const char * const &bytes) : data(new U8[(U8)strlen(bytes) + 1]), length((U8)strlen(bytes)) {
		memcpy(data, bytes, length);
		data[length] = 0;
		allocated = true;
	}
	String(U8 * const &bytes, const U32 &length) : data(new U8[length + 1]), length(length) {
		memcpy(data, bytes, length);
		data[length] = 0;
		allocated = true;
	}
	String(const String &other) : data(new U8[other.length + 1]), length(other.length) {
		memcpy(data, other.data, length);
		data[length] = 0;
		allocated = true;
	}
	String(const String &other, const U32 &length) : data(new U8[length + 1]), length(length) {
		memcpy(data, other.data, length);
		data[length] = 0;
		allocated = true;
	}
	~String() {
		if (allocated) {
			delete [] data;
			allocated = false;
		}
	}

	bool read(FILE *file);
	bool write(FILE *file) const;
};

#include "point2.h"
#include "point3.h"
#include "point4.h"
#include "color.h"

typedef Point2<S16> Point2I;
typedef Point2<F32> Point2F;
typedef Point3<S32> Point3I;
typedef Point3<F32> Point3F;
typedef Point3<F64> Point3D;
typedef Point4<F32> Point4F;

typedef Color<U8>  ColorI;
typedef Color<F32> ColorF;

//More names stolen from TGE

class QuatF;

class AngAxisF {
public:
	Point3F axis;
	F32 angle;

	AngAxisF(const Point3F &axis, const F32 &angle) : axis(axis), angle(angle) {};
	AngAxisF(const F32 &angle, const Point3F &axis) : axis(axis), angle(angle) {};
	AngAxisF(const QuatF &quat);
};

class QuatF : public Readable, Writable {
public:
	F32 w;
	F32 x;
	F32 y;
	F32 z;

	QuatF() : x(0), y(0), z(0), w(0) {};
	QuatF(const F32 &x, const F32 &y, const F32 &z, const F32 &w) : x(x), y(y), z(z), w(w) {};
	QuatF(const AngAxisF &ang);

	bool read(FILE *file);
	bool write(FILE *file) const;
};

inline AngAxisF::AngAxisF(const QuatF &quat) {
	angle = acosf(quat.w);
	F32 half = sqrtf(quat.x * quat.x + quat.y * quat.y + quat.z * quat.z);
	if (half != 0.0f) {
		axis = Point3F(quat.x / half, quat.y / half, quat.z / half);
	} else {
		axis = Point3F(1.0f, 0.0f, 0.0f);
	}
}

inline QuatF::QuatF(const AngAxisF &ang) {
	F32 sin2 = sinf(ang.angle / 2);
	w = cosf(ang.angle / 2);
	x = ang.axis.x * sin2;
	y = ang.axis.y * sin2;
	z = ang.axis.z * sin2;
}

class PlaneF : public Readable, Writable {
public:
	F32 x;
	F32 y;
	F32 z;
	F32 d;

	bool read(FILE *file);
	bool write(FILE *file) const;
};

class BoxF : public Readable, Writable {
public:
	F32 minX;
	F32 minY;
	F32 minZ;
	F32 maxX;
	F32 maxY;
	F32 maxZ;

	inline Point3F getMin() const {
		return Point3F(minX, minY, minZ);
	}
	inline Point3F getMax() const {
		return Point3F(maxX, maxY, maxZ);
	}
	inline Point3F getCenter() const {
		return (getMax() + getMin()) / 2;
	}

	bool read(FILE *file);
	bool write(FILE *file) const;
};

class SphereF : public Readable, Writable {
public:
	F32 x;
	F32 y;
	F32 z;
	F32 radius;

	bool read(FILE *file);
	bool write(FILE *file) const;
};

class Dictionary : public Readable, Writable {
public:
	U32 size;
	String *names;
	String *values;

	bool read(FILE *file);
	bool write(FILE *file) const;

	String get(const String &key) const {
		for (U32 i = 0; i < size; i ++) {
			if (names[i] == key)
				return values[i];
		}
		return nullptr;
	}

	~Dictionary() {
		delete [] names;
		delete [] values;
	}
};

class PNG : public Readable, Writable {
public:
	U32 size;
	U8 *data;

	bool read(FILE *file);
	bool write(FILE *file) const;
};

class TriangleF {
public:
	Point3F point0;
	Point3F point1;
	Point3F point2;
};

class TetrahetronF {
public:
	Point3F point0;
	Point3F point1;
	Point3F point2;
	Point3F point3;
};

class MatrixF : public Readable, Writable {
public:
	F32 m[16];

	bool read(FILE *file);
	bool write(FILE *file) const;
};

#include "ray.h"
typedef Ray<F32> RayF;

#endif
