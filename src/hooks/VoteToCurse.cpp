#include <Util.h>

#include <Geode/Geode.hpp>

#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;
using namespace cs::brkd::saboteur;

#define THIS_ID "vote_to_curse"_spr

static auto const o = Option::create(THIS_ID)
                          ->setName("Vote To Curse!")
                          ->setDescription("Occassionally be prompted vote on someone to briefly annoy while playing a level.\n<co>3+ Players Required</c>\n<cl>created by Cheeseworks</c>")
                          ->setCategory(SBT_CATEGORY)
                          ->setSillyTier(SillyTier::Medium);
HORRIBLE_REGISTER_OPTION(o);