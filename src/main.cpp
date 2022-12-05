#include "main.h"
#include <dllentry.h>

DEFAULT_SETTINGS(settings);

void dllenter() {}
void dllexit() {}

bool BowDing::isBowDingDamageSource(const ActorDamageSource& src) {
	auto dmgerType = src.getDamagingEntityType();
	bool shooterIsPlayer = (src.getEntityType() == ActorType::Player_0);
	return ((dmgerType == ActorType::Arrow) || (dmgerType == ActorType::Trident)) && shooterIsPlayer;
}

void BowDing::sendBowDingNotifications(const Player& shooter, const Player& hurt) {

	int32_t currHealth = hurt.getHealthAsInt();
	int32_t currAbsorption = hurt.getAbsorptionAsInt();

	std::string msg{"§e" + hurt.mPlayerName + " is on §c" + std::to_string(currHealth)};

	{
		using namespace BowDing;
		msg += (settings.useResourcePackGlyphs ? HEALTH_GLYPH : HEALTH_ASCII);
		if (currAbsorption > 0) {
            msg += " " + std::to_string(currAbsorption) + (settings.useResourcePackGlyphs ? ABSORPTION_GLYPH : ABSORPTION_ASCII);
		}
		msg += "§e!";
	}

	auto textPkt = TextPacket::createTextPacket<TextPacketType::SystemMessage>(msg);
	PlaySoundPacket soundPkt{"random.orb", shooter.getBlockPos(), 0.375f, 0.5f};

	shooter.sendNetworkPacket(textPkt);
	shooter.sendNetworkPacket(soundPkt);
}

TInstanceHook(bool, "?_hurt@Player@@MEAA_NAEBVActorDamageSource@@H_N1@Z",
	Player, ActorDamageSource &source, int32_t damage, bool knock, bool ignite) {

	bool result = original(this, source, damage, knock, ignite);

	if (BowDing::isBowDingDamageSource(source)) {
		auto shooter = reinterpret_cast<Player*>(this->mLevel->fetchEntity(source.getEntityUniqueID(), false));

		// make sure not to do anything if the player shoots themself
		if (shooter && (shooter != this)) {
			BowDing::sendBowDingNotifications(*shooter, *this);
		}
	}

	return result;
}