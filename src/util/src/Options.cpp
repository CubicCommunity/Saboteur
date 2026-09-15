#include "../Options.hpp"

#include <Util.h>

#include <Geode/Geode.hpp>

using namespace geode::prelude;
using namespace cs::brkd::saboteur;

bool options::isValidForMp(ZStringView id) {
    return mod->getSettingValue<bool>("sync-all") || str::startsWith(id, ""_spr);
};

options::SelfDirector::SelfDirector() {
    if (auto om = OptionManager::get()) {
        auto list = om->getOptions();
        m_opts.reserve(list.size());

        for (auto const& opt : list) {
            if (auto o = opt.lock()) m_opts.push_back(o);
        };

        m_opts.shrink_to_fit();
    };
};

void options::SelfDirector::setOption(std::shared_ptr<horrible::Option> option) {
    m_sbtOpts[option->getIDHash()] = std::move(option);
};

std::span<const std::shared_ptr<horrible::Option>> options::SelfDirector::getOptions() const noexcept {
    return m_opts;
};

std::unordered_map<uint64_t, std::shared_ptr<horrible::Option>> const& options::SelfDirector::getSaboteurOptions() const noexcept {
    return m_sbtOpts;
};