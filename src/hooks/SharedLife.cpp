#include <Util.h>

#include <Geode/Geode.hpp>

#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;
using namespace cs::brkd::saboteur;

#define THIS_ID "shared_life"_spr

static auto const o = Option::create(THIS_ID)
                          ->setName("Shared Lifeline")
                          ->setDescription("Someone starts with a hot potato and must keep passing it to other people to avoid dying. Whoever receives the hot potato cannot hand it back to the same person they received it from.\n<co>3+ Players Required</c>\n<cl>created by Cheeseworks</c>")
                          ->setCategory(SBT_CATEGORY)
                          ->setSillyTier(SillyTier::High);
HORRIBLE_REGISTER_OPTION(o);