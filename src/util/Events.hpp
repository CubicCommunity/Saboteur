#pragma once

#include <dbuf/ByteWriter.hpp>
#include <dbuf/ByteReader.hpp>

#include <globed/prelude.hpp>

#include <globed/core/Event.hpp>

#include <Geode/Geode.hpp>

namespace cs::brkd::saboteur {
    struct RoomOptionToggleEvent final : globed::ServerEvent<RoomOptionToggleEvent, globed::EventServer::Game> {
        static constexpr auto Id = "option-toggle-ev"_spr;

        uint32_t room = 0;
        std::string option;
        bool on = false;

        RoomOptionToggleEvent(int id = 0, std::string option = "", bool on = false) : room(id), option(std::move(option)), on(on) {};

        std::vector<uint8_t> encode() const {
            dbuf::ByteWriter wr;
            wr.writeU32(room);
            wr.writeStringVar(option);
            wr.writeBool(on);

            return std::move(wr).intoInner();
        };

        static geode::Result<RoomOptionToggleEvent> decode(std::span<const uint8_t> data) {
            dbuf::ByteReader reader{data};
            RoomOptionToggleEvent out{};

            GEODE_UNWRAP_INTO(out.room, reader.readU32());
            GEODE_UNWRAP_INTO(out.option, reader.readStringVar());
            GEODE_UNWRAP_INTO(out.on, reader.readBool());

            return Ok(std::move(out));
        };
    };
};