#pragma once

#include <util/base/Singleton.hpp>

#include <horrible/API.h>

#include <Geode/utils/ZStringView.hpp>

namespace cs::brkd::saboteur {
    namespace options {
        bool isValidForMp(geode::ZStringView id);

        using HashedMap = horrible::HashedMapU64<bool>;

        class SelfDirector final : public base::Singleton<SelfDirector> {
        private:
            horrible::HashedMapU64<horrible::SharedOption> m_sbtOpts;
            std::vector<horrible::SharedOption> m_opts;

        public:
            SelfDirector();

            void setOption(horrible::SharedOption option);

            std::span<const horrible::SharedOption> getOptions() const noexcept;
            horrible::HashedMapU64<horrible::SharedOption> const& getSaboteurOptions() const noexcept;
        };
    };
};