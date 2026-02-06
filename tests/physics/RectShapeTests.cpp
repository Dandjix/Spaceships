//
// Created by timon on 2/7/26.
//

#include "gtest/gtest.h"
#include "physics/shapes/RectPhysicsShape.h"

class DummyEntity : public PhysicsEntity {
public:
    DummyEntity(Vector2Int position, float angle, Vector2Int dimensions)
        : PhysicsEntity(
            position, angle,
            new RectPhysicsShape(this, dimensions)) {
    }

    Entity *initializeRendering(const EntityRendering::Context &context) override { return this; }

    Entity *finalizeRendering(const EntityRendering::Context &context) override { return this; }

    void render(SDL_Renderer *renderer, const RenderingContext &context) override {
    }

    //same thing as in shortLivedEntity, unclean but oh well
    nlohmann::json toJson() override { return {}; }
    constexpr std::string getJsonType() override { return ""; }
};

TEST(RectPhysicsShapeTestSuite, RectPhysicsShapeTest) {
    auto s1 = DummyEntity({0,0},0,{200,200});

    auto vertices = dynamic_cast<RectPhysicsShape*>(s1.shape)->getVertices();

    std::vector<Vector2Int> expected = {
        {-100,100},
        {100,100},
        {100,-100},
        {-100,-100},
    };

    for (int i = 0; i < vertices.size(); ++i) {
        ASSERT_EQ(vertices[i],expected[i]);
    }
}
