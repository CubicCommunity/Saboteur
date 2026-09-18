#include "../Options.hpp"

#include <Util.h>

#include <ranges>

#include <Geode/Geode.hpp>

using namespace geode::prelude;
using namespace cs::brkd::saboteur;

bool options::isSyncing() noexcept {
    return SelfDirector::get()->isSyncEnabled();
};

bool options::shouldAlertSync() {
    return mod->getSettingValue<bool>("sync-popup");
};

bool options::isValidForMp(ZStringView id) {
    return (horrible::isSupporter() && mod->getSettingValue<bool>("sync-all")) || str::startsWith(id, ""_spr);
};

options::SelfDirector::SelfDirector() {
    auto list = OptionManager::get()->getAllOptions();
    m_opts.reserve(list.size());

    for (auto const& o : list | std::views::values) m_opts.push_back(o);

    m_opts.shrink_to_fit();
};

void options::SelfDirector::setOption(horrible::SharedOption option) {
    m_sbtOpts[option->getIDHash()] = std::move(option);
};

void options::SelfDirector::enableSync(bool on) {
    m_syncEnabled = on;
};

std::span<const horrible::SharedOption> options::SelfDirector::getOptions() const noexcept {
    return m_opts;
};

horrible::HashedMapU64<horrible::SharedOption> const& options::SelfDirector::getSaboteurOptions() const noexcept {
    return m_sbtOpts;
};

bool options::SelfDirector::isSyncEnabled() const noexcept {
    return m_syncEnabled;
};