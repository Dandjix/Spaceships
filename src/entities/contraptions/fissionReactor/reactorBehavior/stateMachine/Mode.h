#pragma once
//
// Created by timon on 2/19/26.
//

namespace Contraptions::FissionReactor {
    class Reactor;
}

struct UpdateContext;

namespace Contraptions::FissionReactor::Behavior {

    class Mode {
    public:
        virtual ~Mode() = default;

        virtual void update(const UpdateContext & context, FissionReactor::Reactor * reactor) = 0;
    };
}
