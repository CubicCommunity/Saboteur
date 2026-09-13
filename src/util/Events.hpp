#pragma once

#include <dbuf/ByteWriter.hpp>
#include <dbuf/ByteReader.hpp>

#include <globed/prelude.hpp>

#include <globed/core/Event.hpp>

#include <Geode/Geode.hpp>

namespace cs::brkd::saboteur {
    struct RoomOptionToggleEvent final : globed::ServerEvent<RoomOptionToggleEvent, globed::EventServer::Game> {
        static constexpr auto Id = "room-option-toggle-event"_spr;

        int playerId = 0;
        std::string option;
        bool on = false;

        RoomOptionToggleEvent(int id = 0) : playerId(id) {};

        std::vector<uint8_t> encode() const {
            dbuf::ByteWriter wr;
            wr.writeI32(playerId);

            return std::move(wr).intoInner();
        };

        static geode::Result<RoomOptionToggleEvent> decode(std::span<const uint8_t> data) {
            dbuf::ByteReader reader{data};
            RoomOptionToggleEvent out{};

            GEODE_UNWRAP_INTO(out.playerId, reader.readI32());

            return Ok(std::move(out));
        };
    };
};