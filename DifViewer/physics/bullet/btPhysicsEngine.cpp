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

#ifdef BUILD_PHYSICS
#include "btPhysicsEngine.h"
#include "btPhysicsBody.h"
#include "btPhysicsInterior.h"
#include "btPhysicsSphere.h"
#include <BulletCollision/CollisionShapes/btTriangleShape.h>

bool contactAdded(btManifoldPoint& cp,
				  const btCollisionObjectWrapper* colObj0Wrap,
				  int partId0,
				  int index0,
				  const btCollisionObjectWrapper* colObj1Wrap,
				  int partId1,
				  int index1) {

	btAdjustInternalEdgeContacts(cp,colObj1Wrap,colObj0Wrap, partId1,index1);

	return true;
}

btPhysicsEngine::btPhysicsEngine() : PhysicsEngine() {
	init();
}

void btPhysicsEngine::init() {
	btDefaultCollisionConfiguration *configuration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(configuration);
	btBroadphaseInterface *interface = new btDbvtBroadphase();
	btConstraintSolver *solver = new btSequentialImpulseConstraintSolver();

	world = new btDiscreteDynamicsWorld(dispatcher, interface, solver, configuration);
	world->setGravity(btVector3(0, 0, -20.0f));

	gContactAddedCallback = contactAdded;
	running = true;
}

void btPhysicsEngine::simulate(F32 delta) {
	if (running) {
		extraTime += delta;
		for ( ; extraTime > PHYSICS_TICK; extraTime -= PHYSICS_TICK) {
			world->stepSimulation(PHYSICS_TICK);
		}
		world->stepSimulation(extraTime);
		extraTime = 0;
	}
}

void btPhysicsEngine::addBody(PhysicsBody *body) {
	btPhysicsBody *physBody = static_cast<btPhysicsBody *>(body);

	btRigidBody *rigid = physBody->getActor();

	rigid->setCollisionFlags(rigid->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
	world->addRigidBody(rigid);
}

PhysicsBody *btPhysicsEngine::createInterior(Interior *interior) {
	return new btPhysicsInterior(interior);
}

PhysicsBody *btPhysicsEngine::createSphere(F32 radius) {
	return new btPhysicsSphere(radius);
}

#endif
