#include "../Options.hpp"

#include <Util.h>

#include <Geode/Geode.hpp>

using namespace geode::prelude;
using namespace cs::brkd::saboteur;

bool options::isValidForMp(ZStringView id) {
    return mod->getSettingValue<bool>(id) || str::startsWith(id, ""_spr);
};