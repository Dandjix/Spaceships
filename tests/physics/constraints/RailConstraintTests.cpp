//
// Created by timon on 2/17/26.
//

#include "gtest/gtest.h"
#include "physics/constraints/RailConstraint.h"
#include "physics/scripts/PhysicsEntity.h"
#include "physics/scripts/ShortLivedPhysicsEntity.h"
#include "physics/shapes/RectPhysicsShape.h"

class DummyRailedClass : public ShortLivedPhysicsEntity {
public:
    Physics::Constraints::Rail rail_constraint;

    DummyRailedClass(Vector2Int position, float angle, Vector2Int start, Vector2Int end,
                     PhysicsShape *shape)
        : ShortLivedPhysicsEntity
          (
              position,
              angle,
              shape
          ),
          rail_constraint(start, end) {
    }


    Entity *initializeRendering(const EntityRendering::Context &context) override { return this; }

    Entity *finalizeRendering(const EntityRendering::Context &context) override { return this; }

    void render(SDL_Renderer *renderer, const RenderingContext &context) override {
    }

    std::vector<Physics::Constraint *> getConstraints() override {
        return {&rail_constraint};
    }
};

TEST(RailConstraintTestSuite, RailConstraintProgressSimpletTest) {
    {
        //start
        DummyRailedClass railed = DummyRailedClass({0, 0}, 0, {0, 0}, {1000, 0}, nullptr);
        Physics::Constraints::Rail *rail = dynamic_cast<Physics::Constraints::Rail *>(railed.getConstraints().at(0));
        ASSERT_FALSE(rail == nullptr);
        ASSERT_EQ(rail->progress(&railed),0.0f);
    }
    {
        //end
        DummyRailedClass railed = DummyRailedClass({1000, 0}, 0, {0, 0}, {1000, 0}, nullptr);
        Physics::Constraints::Rail *rail = dynamic_cast<Physics::Constraints::Rail *>(railed.getConstraints().at(0));
        ASSERT_FALSE(rail == nullptr);
        ASSERT_EQ(rail->progress(&railed),1.0f);
    }
    {
        //middle
        DummyRailedClass railed = DummyRailedClass({500, 0}, 0, {0, 0}, {1000, 0}, nullptr);
        Physics::Constraints::Rail *rail = dynamic_cast<Physics::Constraints::Rail *>(railed.getConstraints().at(0));
        ASSERT_FALSE(rail == nullptr);
        ASSERT_EQ(rail->progress(&railed),0.5f);
    }
    {
        // 1/4
        DummyRailedClass railed = DummyRailedClass({250, 0}, 0, {0, 0}, {1000, 0}, nullptr);
        Physics::Constraints::Rail *rail = dynamic_cast<Physics::Constraints::Rail *>(railed.getConstraints().at(0));
        ASSERT_FALSE(rail == nullptr);
        ASSERT_EQ(rail->progress(&railed),0.25f);
    }
    {
        //middle translated + rotated
        DummyRailedClass railed = DummyRailedClass({1000, 1000}, 0, {-1000, -1000}, {3000, 3000}, nullptr);
        Physics::Constraints::Rail *rail = dynamic_cast<Physics::Constraints::Rail *>(railed.getConstraints().at(0));
        ASSERT_FALSE(rail == nullptr);
        ASSERT_EQ(rail->progress(&railed),0.5f);
    }
}