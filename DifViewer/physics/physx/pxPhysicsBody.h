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

#ifndef pxPhysicsBody_h
#define pxPhysicsBody_h

#include "physicsBody.h"
#include "pxPhysicsEngine.h"

class PxPhysicsBody : public PhysicsBody {
protected:
	physx::PxRigidActor *mActor;

public:
	bool getDynamic();

	PxPhysicsBody() : mActor(nullptr) {};
	PxPhysicsBody(physx::PxRigidActor *actor) : mActor(actor) {};

	virtual const Point3F getPosition();
	virtual const AngAxisF getRotation();
	virtual void setMass(const F32 &mass);
	virtual void setPosition(const Point3F &position);
	virtual void setRotation(const AngAxisF &rotation);

	virtual void applyTorque(const Point3F &torque);
	virtual void applyImpulse(const Point3F &impulse, const Point3F &origin);
	virtual void applyForce(const Point3F &force, const Point3F &origin);

	physx::PxRigidActor *getActor() {
		return mActor;
	}
};


#endif
