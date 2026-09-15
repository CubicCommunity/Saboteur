#include <Util.h>

#include <globed/prelude.hpp>

#include <Geode/Geode.hpp>

#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;
using namespace cs::brkd::saboteur;

class $modify(SbtHookPlayLayer, PlayLayer) {
    struct Fields final {
        ListenerHandle m_sync;
        ListenerHandle m_menu;

        ~Fields() {
            // unregister events from gobed
        };
    };

    SBT_SETUP_INTERFACE_FUNC {
        auto f = m_fields.self();

        if (!on) {
            f->m_sync.destroy();

            return;
        };

        if (!globed::api::room::isInRoom()) return;

        if (globed::api::room::isOwner()) {
            if (globed::api::game::getPlayerCount() <= 1) return;

            auto plrs = globed::api::game::getPlayerIds();

            auto opts = globed::EventOptions{};
            opts.targetPlayers.reserve(opts.targetPlayers.size() - 1);

            for (auto const& p : plrs) {
                if (p != globed::api::room::getOwner()) opts.targetPlayers.push_back(p);
            };

            std::unordered_map<uint64_t, bool> list;

            auto optList = options::SelfDirector::get()->getOptions();
            for (auto const& o : optList) {
                if (options::isValidForMp(o->getID())) list[o->getIDHash()] = o->isEnabled();
            };

            RoomOptionSyncEvent(m_level->m_levelID, std::move(list)).send();
        } else {
            f->m_sync = RoomOptionSyncEvent::listen([this](RoomOptionSyncEvent const& ev, globed::EventOptions const& opts) {
                if (opts.sender != globed::api::room::getOwner()) return;
                if (m_level->m_levelID.value() != ev.levelId) return;

                if (auto om = OptionManager::get()) {
                    for (auto const& [id, on] : ev.options) om->toggleOption(id, on);
                };
            });

            f->m_menu = MenuEvent().listen([](Ref<Popup> popup, bool open) {
                if (open) cue::resetNode(popup);
            });
        };
    };

    void setupHasCompleted() {
        PlayLayer::setupHasCompleted();
        SBT_SETUP_INTERFACE_FUNC_NAME(true);
    };
};