//
// Created by timon on 2/12/26.
//

#pragma once

namespace EntityPlacement {

    class Interface;

    struct Context {
        Interface * interface;

        Context(Interface * interface) : interface(interface) {

        }

    };
};
