//
// Created by timon on 11/20/25.
//

#include "gtest/gtest.h"
#include "../../src/spaceships/SpaceShip.h"
#include "physics/PhysicsEntity.h"
#include "../../src/physics/shapes/RoundPhysicsShape.h"

class MockRoundPhysicsEntity : public PhysicsEntity
{
public:
    MockRoundPhysicsEntity(const Vector2Int& position, const std::optional<float>& angle, float radius)
        : PhysicsEntity(position, angle, new RoundPhysicsShape(this,radius))
    {
    }

    ~MockRoundPhysicsEntity()
    {
        delete shape;
    }

    void render(SDL_Renderer* renderer, const RenderingContext& context) override {}

    nlohmann::json toJson() override {
        throw std::logic_error("not implemented");
    }

    constexpr std::string getJsonType() override {
        return "mock_sphere";
    }
};

bool are_vectors_close(Vector2Int a, Vector2Int b, int tolerance = 10)
{
    return std::abs(a.x - b.x) <= tolerance && std::abs(a.y - b.y) <= tolerance;
}

TEST(PhysicsCollisionTestSuite, RoundRoundSimpleTest)
{
    auto space_ship = new SpaceShip();

    auto e1 = new MockRoundPhysicsEntity({100,100},0,50);

    auto e2 = new MockRoundPhysicsEntity({125,125},0,50);

    auto visitor = e1->shape->createVisitor();

    e2->shape->consumeVisitor(visitor,space_ship);

    std::cout << "Mock collision positions : "<< std::endl << e1->getPosition() << std::endl;
    std::cout << e2->getPosition() << std::endl << std::endl;

    // diagonal distance of about 45 (radius 50)

    ASSERT_TRUE(are_vectors_close(e1->getPosition(),{100-45,100-45}));
    ASSERT_TRUE(are_vectors_close(e2->getPosition(),{125+45,125+45}));

    delete e1;
    delete e2;
    delete visitor;
    delete space_ship;

}