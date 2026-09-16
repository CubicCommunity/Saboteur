#include <Util.h>

#include <Geode/Geode.hpp>

#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;
using namespace cs::brkd::saboteur;

#define THIS_ID "algebra_quiz"_spr

static auto const o = Option::create(THIS_ID)
                          ->setName("Steven's Algebra Quiz!")
                          ->setDescription("Not actual algebra. Steven will sometimes pop in and ask everyone a math question. Everyone must answer before the time runs out, and at least half the players must have answered correctly to pass. Otherwise, everyone will be reset in the level.\n<co>2+ Players Required</c>\n<cl>created by Cheeseworks</c>")
                          ->setCategory(SBT_CATEGORY)
                          ->setSillyTier(SillyTier::Medium);
SBT_REGISTER_OPTION(o);