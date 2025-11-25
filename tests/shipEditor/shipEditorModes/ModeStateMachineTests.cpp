//
// Created by timon on 11/25/25.
//

#include "shipEditor/shipEditorModes/ShipEditorMode.h"

class DummyClass {
public:
    bool value;
    DummyClass() : value(true) {  }
};

class ShipEditorModeTestable : public ShipEditorModes::ShipEditorMode {
public:
    using ShipEditorMode::ShipEditorMode;

    ShipEditorModeTestable() : ShipEditorMode(nullptr) {  }

    // Expose the private templated helpers via public wrapper functions
    template<typename T>
    void testRemoveItems(std::vector<T>* active, const std::vector<T>& to_remove) {
        removeItems(active, to_remove);
    }

    template<typename T>
    void testAddItems(std::vector<T>* active, const std::vector<T>& to_add) {
        addItems(active, to_add);
    }

    void enter() override {}
    void leave() override {}
};

#include "gtest/gtest.h"
TEST(EditorStateMachineSuite,AddTest) {
    auto o1 = new DummyClass();
    auto o2 = new DummyClass();

    std::vector<DummyClass *> active = {o1};

    ShipEditorModeTestable mode = {};

    mode.testAddItems(&active,{o2});

    ASSERT_EQ(active.size(),2);
    ASSERT_EQ(active.at(0),o1);
    ASSERT_EQ(active.at(1),o2);

    delete o1;
    delete o2;
}

TEST(EditorStateMachineSuite,RemoveTest) {
    auto o1 = new DummyClass();
    auto o2 = new DummyClass();
    auto o3 = new DummyClass();

    std::vector<DummyClass *> active = {o1,o2,o3};

    ShipEditorModeTestable mode = {};

    mode.testRemoveItems(&active,{o2});

    ASSERT_EQ(active.size(),2);
    ASSERT_EQ(active.at(0),o1);
    ASSERT_EQ(active.at(1),o3);

    delete o1;
    delete o2;
    delete o3;
}