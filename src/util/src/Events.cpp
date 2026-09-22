#include "../Events.hpp"

#include <Util.h>

#include <Geode/Geode.hpp>

using namespace geode::prelude;
using namespace cs::brkd::saboteur;

geode::ByteVector events::RoomOptionSync::encode() const {
    dbuf::ByteWriter wr;
    wr.writeI32(levelId);
    wr.writeU64(options.size());

    for (auto const& [k, v] : options) {
        wr.writeU64(k);
        wr.writeBool(v);
    };

    return std::move(wr).intoInner();
};

Result<events::RoomOptionSync> events::RoomOptionSync::decode(geode::ByteSpan data) {
    dbuf::ByteReader reader{data};
    events::RoomOptionSync out{};

    GEODE_UNWRAP_INTO(out.levelId, reader.readI32());
    GEODE_UNWRAP_INTO(size_t size, reader.readU64());

    options::HashedMap opts;

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