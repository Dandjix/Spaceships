//
// Created by timon on 1/18/26.
//

#include "Button.h"

void Button::interact(Humanoid *activator) {


}

bool Button::is_interactable(Humanoid *activator) {
    return IInteractable::distance_check(this,activator);
}
