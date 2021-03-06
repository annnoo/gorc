#pragma once

#include "libold/content/assets/animation.hpp"
#include "content/id.hpp"
#include "utility/uid.hpp"

namespace gorc {
namespace game {
namespace world {
namespace keys {

class key_state {
public:
    uid(96857468);

    thing_id mix_id;
    bool is_pov_mix = false;

    maybe<asset_ref<content::assets::animation>> animation;
    double animation_time;
    double current_frame;
    double speed;
    flag_set<flags::key_flag> flags;
    float expiration_time = 0.0f;
    unsigned int creation_timestamp = 0;

    int high_priority, low_priority, body_priority;

    key_state();
};

}
}
}
}
