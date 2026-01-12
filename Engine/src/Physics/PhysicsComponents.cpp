#include "Engine/Physics/PhysicsComponents.h"
#include <box2d/box2d.h>

namespace Engine {

    void Rigidbody2DComponent::ApplyForce(const glm::vec2& force, const glm::vec2& point, bool wake) {
        if (RuntimeBody) {
            b2Body* body = static_cast<b2Body*>(RuntimeBody);
            body->ApplyForce(b2Vec2(force.x, force.y), b2Vec2(point.x, point.y), wake);
        }
    }

    void Rigidbody2DComponent::ApplyForceToCenter(const glm::vec2& force, bool wake) {
        if (RuntimeBody) {
            b2Body* body = static_cast<b2Body*>(RuntimeBody);
            body->ApplyForceToCenter(b2Vec2(force.x, force.y), wake);
        }
    }

    void Rigidbody2DComponent::ApplyLinearImpulse(const glm::vec2& impulse, const glm::vec2& point, bool wake) {
        if (RuntimeBody) {
            b2Body* body = static_cast<b2Body*>(RuntimeBody);
            body->ApplyLinearImpulse(b2Vec2(impulse.x, impulse.y), b2Vec2(point.x, point.y), wake);
        }
    }

    void Rigidbody2DComponent::ApplyLinearImpulseToCenter(const glm::vec2& impulse, bool wake) {
        if (RuntimeBody) {
            b2Body* body = static_cast<b2Body*>(RuntimeBody);
            body->ApplyLinearImpulseToCenter(b2Vec2(impulse.x, impulse.y), wake);
        }
    }

    void Rigidbody2DComponent::ApplyAngularImpulse(float impulse, bool wake) {
        if (RuntimeBody) {
            b2Body* body = static_cast<b2Body*>(RuntimeBody);
            body->ApplyAngularImpulse(impulse, wake);
        }
    }

}

