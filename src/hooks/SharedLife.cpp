#include <Util.h>

#include <Geode/Geode.hpp>

#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;
using namespace cs::brkd::saboteur;

#define THIS_ID "shared_life"_spr

static auto const o = Option::create(THIS_ID)
                          ->setName("Shared Lifeline")
                          ->setDescription("Everyone starts off with a shared health meter. If anyone hits an obstacle, everyone takes damage and the meter decreases for everyone, and if it reaches 0, everyone in the level dies.\n<co>2+ Players Required</c>\n<cl>created by Cheeseworks</c>")
                          ->setCategory(SBT_CATEGORY)
                          ->setSillyTier(SillyTier::High);
SBT_REGISTER_OPTION(o);