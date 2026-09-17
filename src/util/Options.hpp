#pragma once

#include <util/base/Singleton.hpp>

#include <horrible/Option.hpp>

#include <Geode/utils/ZStringView.hpp>

namespace cs::brkd::saboteur {
    namespace options {
        bool isValidForMp(geode::ZStringView id);

        using HashedMap = std::unordered_map<uint64_t, bool>;

        class SelfDirector final : public base::Singleton<SelfDirector> {
        private:
            std::unordered_map<uint64_t, std::shared_ptr<horrible::Option>> m_sbtOpts;
            std::vector<std::shared_ptr<horrible::Option>> m_opts;

        public:
            SelfDirector();

            void setOption(std::shared_ptr<horrible::Option> option);

            std::span<const std::shared_ptr<horrible::Option>> getOptions() const noexcept;
            std::unordered_map<uint64_t, std::shared_ptr<horrible::Option>> const& getSaboteurOptions() const noexcept;
        };
    };
};