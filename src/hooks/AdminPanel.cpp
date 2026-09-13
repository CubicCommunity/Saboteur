#include <Util.h>

#include <Geode/Geode.hpp>

#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;
using namespace cs::brkd::saboteur;

#define THIS_ID "admin_panel"_spr

static auto const o = Option::create(THIS_ID)
                          ->setName("Admin Panel")
                          ->setDescription("If you're the room owner, you'll have access to an admin panel to troll other people in the room who are playing the same level as you with. Use wisely...\n<co>2+ Players Required</c>\n<cl>created by Cheeseworks</c>")
                          ->setCategory(SBT_CATEGORY)
                          ->setSillyTier(SillyTier::High);
HORRIBLE_REGISTER_OPTION(o);