#include "Engine/Physics/PhysicsComponents.h"
#include <box2d/box2d.h>

namespace Engine {

    void Rigidbody2DComponent::SetVelocity(const glm::vec2& velocity) {
        if (RuntimeBody) {
            b2Body* body = static_cast<b2Body*>(RuntimeBody);
            body->SetLinearVelocity(b2Vec2(velocity.x, velocity.y));
        }
        Velocity = velocity;
    }

    glm::vec2 Rigidbody2DComponent::GetVelocity() const {
        if (RuntimeBody) {
            b2Body* body = static_cast<b2Body*>(RuntimeBody);
            const auto& vel = body->GetLinearVelocity();
            return { vel.x, vel.y };
        }
        return Velocity;
    }

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

