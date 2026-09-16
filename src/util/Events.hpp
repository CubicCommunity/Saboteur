#pragma once

#include <dbuf/ByteWriter.hpp>
#include <dbuf/ByteReader.hpp>

#include <globed/prelude.hpp>

#include <globed/core/Event.hpp>

#include <util/Options.hpp>

#include <Geode/Geode.hpp>

namespace cs::brkd::saboteur {
    struct RoomOptionSyncEvent final : globed::ServerEvent<RoomOptionSyncEvent, globed::EventServer::Game> {
        static constexpr auto Id = "option-sync-ev"_spr;

        int32_t levelId = 0;
        options::OptionMap options;

        RoomOptionSyncEvent(int32_t lvlId = 0, std::optional<options::OptionMap> opts = std::nullopt) :
            levelId(lvlId), options(opts.has_value() ? std::move(opts).value() : options::OptionMap{}) {};

        std::vector<uint8_t> encode() const;
        static geode::Result<RoomOptionSyncEvent> decode(std::span<const uint8_t> data);
    };
};