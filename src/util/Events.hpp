#pragma once

#include <dbuf/ByteWriter.hpp>
#include <dbuf/ByteReader.hpp>

#include <globed/prelude.hpp>

#include <globed/core/Event.hpp>

#include <util/Options.hpp>

#include <Geode/Geode.hpp>

namespace cs::brkd::saboteur {
    namespace events {
        struct RoomOptionSync final : globed::ServerEvent<RoomOptionSync, globed::EventServer::Game> {
            static constexpr auto Id = "option-sync-ev"_spr;

            int32_t levelId = 0;
            options::HashedMap options;

            RoomOptionSync(int32_t lvlId = 0, std::optional<options::HashedMap> opts = std::nullopt) :
                levelId(lvlId), options(opts.has_value() ? std::move(opts).value() : options::HashedMap{}) {};

            geode::ByteVector encode() const;
            static geode::Result<RoomOptionSync> decode(geode::ByteSpan data);
        };

        struct RoomLevelPlayer final : geode::Event<RoomLevelPlayer, bool(bool, int), globed::GlobedGJBGL*> {
            using Event::Event;
        };

        struct RoomJoin final : geode::ThreadSafeEvent<RoomJoin, bool(bool)> {
            using ThreadSafeEvent::ThreadSafeEvent;
        };
    };
};