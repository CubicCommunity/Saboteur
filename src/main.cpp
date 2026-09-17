#include <Util.h>

#include <globed/prelude.hpp>

#include <globed/soft-link/Table.hpp>

#include <Geode/Geode.hpp>

#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;
using namespace cs::brkd::saboteur;

$execute {
    auto& core = modules::Core::get();
};

class $modify(SbtHookPlayLayer, PlayLayer) {
    struct Fields final {
        ListenerHandle toggles;

        ListenerHandle sync;
        ListenerHandle menu;

        options::HashedMap previousStates;

        ~Fields() {
            if (globed::api::room::isOwner()) {
                log::warn("Player leaving level is room owner, keeping all states");
                return;
            };

            log::debug("Restoring {} previous Horrible Menu option states", previousStates.size());
            for (auto const& [id, on] : previousStates) {
                if (auto om = OptionManager::get()) om->toggleOption(id, on);
            };
        };
    };

    SBT_SETUP_INTERFACE_FUNC {
        auto f = m_fields.self();

        if (!on) {
            f->sync.destroy();
            f->menu.destroy();

            f->previousStates.clear();

            return;
        };

        if (!globed::api::room::isInRoom()) {
            log::warn("{}", (void*)globed::api::table());
            log::warn("{}", (void*)globed::api::roomTable());

            log::trace("Skipping multiplayer option synchronization for level {}: not in a room", m_level->m_levelID);
            return;
        };

        log::debug("Enabling multiplayer option synchronization for level {} as {}", m_level->m_levelID, globed::api::room::isOwner() ? "owner" : "client");

        if (globed::api::room::isOwner()) {
            if (globed::api::game::getPlayerCount() >= 2) {
                syncOptionsOwner();
            } else {
                log::debug("Not communicating multiplayer option states for level {}: only {} player(s)", m_level->m_levelID, globed::api::game::getPlayerCount());
            };

            f->sync = events::RoomOptionSync::listen([this](events::RoomOptionSync const& ev, globed::EventOptions const& opts) {
                if (ev.levelId != m_level->m_levelID) {
                    log::trace("Ignoring option sync request for level {} while playing level {}", ev.levelId, m_level->m_levelID);
                    return;
                };

                if (!globed::api::game::getPlayer(opts.sender)) {
                    log::warn("Ignoring option sync request from an unknown player for level {}", ev.levelId);
                    return;
                };

                if (auto sd = options::SelfDirector::get()) {
                    options::HashedMap list;
                    for (auto const& [id, opt] : sd->getSaboteurOptions()) list[id] = opt->isEnabled();

                    auto evOpts = globed::EventOptions{};
                    evOpts.targetPlayers = {opts.sender};

                    log::info("Responding with {} Saboteur option states for level {}", list.size(), ev.levelId);
                    events::RoomOptionSync(m_level->m_levelID, std::move(list)).send(std::move(evOpts));
                } else {
                    log::error("Could not respond to option sync request for level {}: SelfDirector unavailable", ev.levelId);
                };
            });

            log::debug("Listening for multiplayer option sync requests as owner for level {}", m_level->m_levelID);

            f->toggles = OptionEvent().listen([this](std::string_view id, OptionSave data) {
                if (!options::isValidForMp(std::string{id})) return;

                if (globed::api::game::getPlayerCount() >= 2) {
                    syncOptionsOwner();
                } else {
                    log::debug("Not communicating multiplayer option states for level {}: only {} player(s)", m_level->m_levelID, globed::api::game::getPlayerCount());
                };
            });
        } else {
            f->sync = events::RoomOptionSync::listen([this, f](events::RoomOptionSync const& ev, globed::EventOptions const& opts) {
                if (opts.sender != globed::api::room::getOwner()) {
                    log::trace("Ignoring option sync event for level {} from a non-owner sender", ev.levelId);
                    return;
                };

                if (m_level->m_levelID != ev.levelId) {
                    log::trace("Ignoring owner option sync event for level {} while playing level {}", ev.levelId, m_level->m_levelID);
                    return;
                };

                if (f->previousStates.empty()) savePreviousStates(m_level->m_levelID);

                if (auto om = OptionManager::get()) {
                    log::info("Applying {} synchronized option states for level {}", ev.options.size(), ev.levelId);
                    for (auto const& [id, on] : ev.options) om->toggleOption(id, on);
                } else {
                    log::error("Could not apply synchronized option states for level {}: OptionManager unavailable", ev.levelId);
                };
            });

            log::debug("Listening for multiplayer option sync events as client for level {}", m_level->m_levelID);

            f->menu = MenuEvent().listen([](Ref<Popup> popup, bool open) {
                log::trace("Menu event received: {}", open ? "opened" : "closed");
                if (open) cue::resetNode(popup);
            });

            if (!globed::api::game::getPlayer(globed::api::room::getOwner())) {
                log::warn("Cannot request owner option states for level {}: owner is not present", m_level->m_levelID);
                return;
            };

            auto evOpts = globed::EventOptions{};
            evOpts.targetPlayers = {globed::api::room::getOwner()};

            log::info("Requesting synchronized option states for level {} from the room owner", m_level->m_levelID);
            events::RoomOptionSync(m_level->m_levelID).send(std::move(evOpts));
        };
    };

    void setupHasCompleted() {
        PlayLayer::setupHasCompleted();

        auto res = globed::getRootTable();
        if (res.isErr()) return log::error("Root table error: {}", res.unwrapErr());

        log::info("{}", (void*)res.unwrap());

        SBT_SETUP_INTERFACE_FUNC_NAME(true);
    };

    void savePreviousStates(int levelId) {
        if (auto sd = options::SelfDirector::get()) {
            log::debug("Saving and disabling {} local Saboteur option states for level {}", sd->getOptions().size(), levelId);
            for (auto const& opt : sd->getOptions()) {
                m_fields->previousStates[opt->getIDHash()] = opt->isEnabled();
                opt->disable();
            };
        } else {
            log::error("Could not disable local Saboteur options for level {}: SelfDirector unavailable", levelId);
        };
    };

    void syncOptionsOwner() {
        auto plrs = globed::api::game::getPlayerIds();

        auto evOpts = globed::EventOptions{};
        evOpts.targetPlayers.reserve(evOpts.targetPlayers.size() - 1);

        for (auto const& p : plrs) {
            if (p != globed::api::room::getOwner()) evOpts.targetPlayers.push_back(p);
        };

        options::HashedMap list;

        for (auto const& [id, opt] : options::SelfDirector::get()->getSaboteurOptions()) list[id] = opt->isEnabled();

        log::info("Syncing {} Saboteur option states for all players in level {}", list.size(), m_level->m_levelID);
        events::RoomOptionSync(m_level->m_levelID, std::move(list)).send(std::move(evOpts));
    };
};