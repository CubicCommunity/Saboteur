#include "../Events.hpp"

#include <Util.h>

#include <Geode/Geode.hpp>

using namespace geode::prelude;
using namespace cs::brkd::saboteur;

std::vector<uint8_t> RoomOptionSyncEvent::encode() const {
    dbuf::ByteWriter wr;
    wr.writeI32(levelId);
    wr.writeU64(options.size());

    for (auto const& [k, v] : options) {
        wr.writeU64(k);
        wr.writeBool(v);
    };

    return std::move(wr).intoInner();
};

Result<RoomOptionSyncEvent> RoomOptionSyncEvent::decode(std::span<const uint8_t> data) {
    dbuf::ByteReader reader{data};
    RoomOptionSyncEvent out{};

    GEODE_UNWRAP_INTO(out.levelId, reader.readI32());
    GEODE_UNWRAP_INTO(size_t size, reader.readU64());

    options::OptionMap opts;

    if (size > 0) {
        opts.reserve(size);

        for (size_t i = 0; i < size; ++i) {
            GEODE_UNWRAP_INTO(size_t id, reader.readU64());
            GEODE_UNWRAP_INTO(bool on, reader.readBool());

            opts[id] = on;
        };
    };

    out.options = std::move(opts);

    return Ok(std::move(out));
};