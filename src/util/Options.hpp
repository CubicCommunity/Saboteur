#pragma once

#include <Geode/utils/ZStringView.hpp>

namespace cs::brkd::saboteur {
    namespace options {
        bool isValidForMp(geode::ZStringView id);
    };
};