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

namespace BowDing {

	// why is there no std::string_view + operator overload??? could be using that instead...
	inline constexpr const char *HEALTH_GLYPH     = "\ue1fe";                         // , glyph 0xE1FE
	inline constexpr const char *ABSORPTION_GLYPH = "\ue1ff";                         // , glyph 0xE1FF
	inline constexpr const char *HEALTH_ASCII     = "\u00a7\u0063\u2764\u00a7\u0072"; // §c❤§r
	inline constexpr const char *ABSORPTION_ASCII = "\u00a7\u0065\u2764\u00a7\u0072"; // §e❤§r

	bool isBowDingDamageSource(const ActorDamageSource &src);
	void sendBowDingNotifications(const Player& shooter, const Player &hurt);
}

DEF_LOGGER("BowDing");