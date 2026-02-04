//
// Created by timon on 2/4/26.
//

#include <format>

#include "entities/scripts/ShortLivedEntity.h"
#include "gtest/gtest.h"
#include "physics/PhysicsEntity.h"
#include "physics/shapes/ConvexPhysicsShape.h"
TEST(ConvexShapeTestSuite, ConvexShapeTest) {
    class DummyEntity : public PhysicsEntity {
    public:
        DummyEntity(Vector2Int position, float angle, const std::vector<Vector2Int> &points)
            : PhysicsEntity(
                position, angle,
                new ConvexPhysicsShape(
                    this,
                    points
                )) {
        }

        Entity * initializeRendering(const EntityRendering::Context &context) override { return this; }

        Entity * finalizeRendering(const EntityRendering::Context &context) override { return this; }

        void render(SDL_Renderer *renderer, const RenderingContext &context) override {
        }

        //same thing as in shortLivedEntity, unclean but oh well
        nlohmann::json toJson() override { return {}; }
        constexpr std::string getJsonType() override { return ""; }
    };

    auto entity = DummyEntity(
        {0, 0},
        0,
        {
            {0, 0},
            {0, 100},
            {100, 100},
            {100, 0}
        });


    std::string vertices_string= "[";
    for (auto v: dynamic_cast<ConvexPhysicsShape*>(entity.shape)->getVertices()) {
        vertices_string += std::format("({},{}),",v.x,v.y);
    }

    std::cout << "points : " << vertices_string << std::endl;

    ASSERT_TRUE(entity.shape->is_inside({1,1}));
    ASSERT_TRUE(entity.shape->is_inside({1,99}));
    ASSERT_TRUE(entity.shape->is_inside({99,1}));
    ASSERT_TRUE(entity.shape->is_inside({99,99}));
    ASSERT_TRUE(entity.shape->is_inside({50,50}));



    ASSERT_FALSE(entity.shape->is_inside({-1,-1}));
    ASSERT_FALSE(entity.shape->is_inside({1,-1}));
    ASSERT_FALSE(entity.shape->is_inside({-1,1}));

    ASSERT_FALSE(entity.shape->is_inside({-1,99}));
    ASSERT_FALSE(entity.shape->is_inside({1,101}));
    ASSERT_FALSE(entity.shape->is_inside({-1,101}));

    ASSERT_FALSE(entity.shape->is_inside({101,1}));
    ASSERT_FALSE(entity.shape->is_inside({99,-1}));
    ASSERT_FALSE(entity.shape->is_inside({101,-1}));

    ASSERT_FALSE(entity.shape->is_inside({101,99}));
    ASSERT_FALSE(entity.shape->is_inside({99,101}));
    ASSERT_FALSE(entity.shape->is_inside({101,101}));
}
