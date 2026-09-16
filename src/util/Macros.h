#pragma once

#define HIGHEST_Z cocos2d::CCScene::get()->getHighestChildZ() + 1

#define SBT_SETUP_INTERFACE_FUNC_NAME sbtSetupInterface

#define SBT_SETUP_INTERFACE_FUNC void SBT_SETUP_INTERFACE_FUNC_NAME(bool on = true)

#define SBT_MODIFY_EVENT_HANDLER(Base, Derived)                                                                           \
    horrible::listenForHorribleOptionChanges(                                                                             \
        THIS_ID,                                                                                                          \
        [](HorribleOptionSave data) {                                                                                     \
            if (auto b = Base::get()) geode::cast::modify_cast<Derived*>(b)->SBT_SETUP_INTERFACE_FUNC_NAME(data.enabled); \
        })

#define SBT_TOGGLE_MODIFY(Base, Derived)         \
    $on_mod(Loaded) {                            \
        SBT_MODIFY_EVENT_HANDLER(Base, Derived); \
    }

#define SBT_REGISTER_OPTION(opt)                                                            \
    $on_mod(Loaded) {                                                                       \
        if (auto om = horrible::OptionManager::get()) om->registerOption(opt);              \
        if (auto sd = cs::brkd::saboteur::options::SelfDirector::get()) sd->setOption(opt); \
    }