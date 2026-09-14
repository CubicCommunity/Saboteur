#include <Util.h>

#include <globed/prelude.hpp>

#include <Geode/Geode.hpp>

#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;
using namespace cs::brkd::saboteur;

class $modify(SbtHookPlayLayer, PlayLayer) {
    struct Fields final {
        ListenerHandle m_request;
        ListenerHandle m_sync;

        ~Fields() {
            // unregister events from gobed
        };
    };

    SBT_SETUP_INTERFACE_FUNC{
        // set the stuff up
    };

    void setupHasCompleted() {
        PlayLayer::setupHasCompleted();
        SBT_SETUP_INTERFACE_FUNC_NAME(true);
    };
};