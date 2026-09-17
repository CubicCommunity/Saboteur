#include "../Options.hpp"

#include <Util.h>

#include <Geode/Geode.hpp>

using namespace geode::prelude;
using namespace cs::brkd::saboteur;

bool options::isValidForMp(ZStringView id) {
    return (horrible::isSupporter() && mod->getSettingValue<bool>("sync-all")) || str::startsWith(id, ""_spr);
};

options::SelfDirector::SelfDirector() {
    auto list = OptionManager::get()->getAllOptions();
    m_opts.reserve(list.size());

    for (auto const& [id, o] : list) m_opts.push_back(o);

    m_opts.shrink_to_fit();
};

void options::SelfDirector::setOption(horrible::SharedOption option) {
    m_sbtOpts[option->getIDHash()] = std::move(option);
};

std::span<const horrible::SharedOption> options::SelfDirector::getOptions() const noexcept {
    return m_opts;
};

horrible::HashedMapU64<horrible::SharedOption> const& options::SelfDirector::getSaboteurOptions() const noexcept {
    return m_sbtOpts;
};