#pragma once

#include <horrible/API.h>

#include <cue/Util.hpp>

#include <util/Include.h>
#include <util/Macros.h>

#include <Geode/Geode.hpp>

// Category for all options in this mod
#define SBT_CATEGORY "Saboteur"

// Namespace for utility methods for Saboteur
namespace cs::brkd::saboteur {
    using namespace ::horrible;

    // Pointer reference to the current mod instance
    inline geode::Mod* mod = geode::Mod::get();

    /**
     * Play a sound effect using FMOD
     *
     * @param name Name of the audio file
     */
    inline void playSfx(geode::ZStringView name) {
        if (auto fmod = FMODAudioEngine::sharedEngine()) (void)fmod->playEffectAsync(name);
    };

    namespace str = geode::utils::string;  // Shortcut for geode::utils::string
};
