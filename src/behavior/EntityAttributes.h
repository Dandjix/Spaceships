//
// Created by timon on 7/9/25.
//

#ifndef ENTITYATTRIBUTES_H
#define ENTITYATTRIBUTES_H
#include <string>
#include <unordered_map>


///status effects are handled in this class eventually
class EntityAttributes {
protected:
    std::unordered_map<std::string,float> attributes = {};

public:
    EntityAttributes() {  }

    float getAttribute(std::string key) const {
        if (!attributes.contains(key)) {
            return 0;
        }
        return attributes.at(key);
    }
};



#endif //ENTITYATTRIBUTES_H
