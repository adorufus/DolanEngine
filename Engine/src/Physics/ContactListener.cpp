#include "Engine/Physics/ContactListener.h"
#include "Engine/Core/Logger.h"

namespace Engine {

    void ContactListener::BeginContact(b2Contact* contact) {
        // Get the two bodies involved in the collision
        b2Body* bodyA = contact->GetFixtureA()->GetBody();
        b2Body* bodyB = contact->GetFixtureB()->GetBody();
        
        // User data can be used to store Entity handle
        // For now, just log
        // GE_CORE_TRACE("Collision Begin");
    }

    void ContactListener::EndContact(b2Contact* contact) {
        // GE_CORE_TRACE("Collision End");
    }

    void ContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold) {
        // Called before collision resolution
    }

    void ContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) {
        // Called after collision resolution
        // Can be used to apply damage, play sounds, etc.
    }

}

