#include "../Module.hpp"

#include <Util.h>

#include <Geode/Geode.hpp>

using namespace geode::prelude;
using namespace cs::brkd::saboteur;

void modules::Core::onModuleInit() {
    log::trace("Core Globed module initialized");
    setAutoEnableMode(globed::AutoEnableMode::Server);
};

void modules::Core::startRoomPoll() {
    log::trace("Starting room state poll...");

    m_roomPoll.spawn(
        roomPollCheck(),
        [this]() {
            log::warn("Globed room state poll ended abruptly!");
            log::trace("Re-starting room state poll...");

            if (isEnabled()) startRoomPoll();
        });
};

arc::Future<> modules::Core::roomPollCheck() {
    if (!isEnabled()) co_return;

    while (true) {
        auto room = globed::api::room::isInRoom();
        if (room == m_roomState) {
            co_await arc::sleepFor(asp::Duration::fromMillis(200));
            continue;
        };

        m_roomState = room;
        m_roomStateID = globed::api::room::getId();

        log::info("Player has {} the room{}!", room ? "joined" : "left", room ? fmt::format(" ({})", m_roomStateID) : "");
        co_await async::waitForMainThread([room]() {
            events::RoomJoin().send(room);
        });
    };

    co_return;
};

Result<> modules::Core::onEnabled() {
    log::debug("Core Globed module enabled");

    startRoomPoll();
    return Ok();
};

Result<> modules::Core::onDisabled() {
    log::debug("Core Globed module disabled");

    m_roomPoll.cancel();
    return Ok();
};

void modules::Core::onPlayerJoin(globed::GlobedGJBGL* gjbgl, int accountId) {
    log::debug("Player {} has joined the level!", accountId);
    events::RoomLevelPlayer(gjbgl).send(true, accountId);
};

void modules::Core::onPlayerLeave(globed::GlobedGJBGL* gjbgl, int accountId) {
    log::debug("Player {} has left the level!", accountId);
    events::RoomLevelPlayer(gjbgl).send(false, accountId);
};