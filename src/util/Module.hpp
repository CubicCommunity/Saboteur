#pragma once

#include <globed/core/Core.hpp>
#include <globed/core/ModuleCrtp.hpp>

#include <Geode/Geode.hpp>

namespace cs::brkd::saboteur {
    namespace modules {
        class Core final : public globed::ModuleCrtpBase<Core> {
        private:
            std::atomic<bool> m_roomState = false;
            std::atomic<uint32_t> m_roomStateID = 0;

            geode::async::TaskHolder<> m_roomPoll;

            void startRoomPoll();
            arc::Future<> roomPollCheck();

        protected:
            geode::Result<> onEnabled() override;
            geode::Result<> onDisabled() override;

            void onPlayerJoin(globed::GlobedGJBGL* gjbgl, int accountId) override;
            void onPlayerLeave(globed::GlobedGJBGL* gjbgl, int accountId) override;

        public:
            void onModuleInit();

            std::string_view name() const override {
                return "Saboteur Core";
            };

            std::string_view id() const override {
                return "breakeode.saboteur-core-module";
            };

            std::string_view author() const override {
                return "Breakeode";
            };

            std::string_view description() const override {
                return "This module adds cool stuff!";
            };
        };
    };
};