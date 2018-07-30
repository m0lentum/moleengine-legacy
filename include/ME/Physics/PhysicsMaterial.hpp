#ifndef PHYSICS_MATERIAL_HPP
#define PHYSICS_MATERIAL_HPP

namespace me
{
    struct PhysicsMaterial
    {
   		float elasticity;
		float friction;
		float drag;
		float angularDrag;
        float bounceThreshold;




        PhysicsMaterial(float elasticity, float friction, float drag, float angularDrag, float bounceThreshold) :
            elasticity(elasticity),
            friction(friction),
            drag(drag),
            angularDrag(angularDrag),
            bounceThreshold(bounceThreshold)
        {}

        PhysicsMaterial(const PhysicsMaterial &copy) :
            elasticity(copy.elasticity),
            friction(copy.friction),
            drag(copy.drag),
            angularDrag(copy.angularDrag),
            bounceThreshold(copy.bounceThreshold)
        {}

        PhysicsMaterial(PhysicsMaterial&& move) :
            elasticity(std::move(move.elasticity)),
            friction(std::move(move.friction)),
            drag(std::move(move.drag)),
            angularDrag(std::move(move.angularDrag)),
            bounceThreshold(std::move(move.bounceThreshold))
        {}

        PhysicsMaterial& operator=(PhysicsMaterial&& other)
        {
            if (&other != this)
            {
                elasticity = std::move(other.elasticity);
                friction = std::move(other.friction);
                drag = std::move(other.drag);
                angularDrag = std::move(other.drag);
                bounceThreshold = std::move(other.bounceThreshold);
            }

            return *this;
        }
    };
}

#endif // PHYSICS_MATERIAL_HPP