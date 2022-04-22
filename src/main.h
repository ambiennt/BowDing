#pragma once

#include <base/log.h>
#include <hook.h>
#include <base/base.h>
#include <yaml.h>
#include <Actor/Player.h>
#include <Actor/ActorDamageSource.h>
#include <Actor/Attribute.h>
#include <Level/Level.h>
#include <Packet/PlaySoundPacket.h>
#include <Packet/TextPacket.h>

inline struct Settings {

	bool useResourcePackGlyphs = false;

	template <typename IO> static inline bool io(IO f, Settings &settings, YAML::Node &node) {
		return f(settings.useResourcePackGlyphs, node["useResourcePackGlyphs"]);
	}
} settings;

DEF_LOGGER("BowDing");