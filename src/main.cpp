#include "main.h"
#include <dllentry.h>

DEFAULT_SETTINGS(settings);

void dllenter() {}
void dllexit() {}

TInstanceHook(bool, "?_hurt@Player@@MEAA_NAEBVActorDamageSource@@H_N1@Z",
	Player, ActorDamageSource &source, int damage, bool knock, bool ignite) {
	bool result = original(this, source, damage, knock, ignite);

	if ((source.getDamagingEntityType() == ActorType::Arrow) && (source.getEntityType() == ActorType::Player_0)) {

		auto shooter = (Player*)(LocateService<Level>()->fetchEntity(source.getEntityUniqueID(), false));
		if (!shooter || (shooter == this)) return result; // if player shoots themself

		int32_t currHealth = this->getHealthAsInt();
		int32_t currAbsorption = this->getAbsorptionAsInt();

		std::string msg("§e" + this->mPlayerName + " is on§c " + std::to_string(currHealth));
		if (settings.useResourcePackGlyphs) {
			msg += "" + ((currAbsorption > 0) ? " " + std::to_string(currAbsorption) + "" : "") + "§e!"; // glyph 0xE1FE, 0xE1FF
		}
		else {
			msg += "§c❤§r" + ((currAbsorption > 0) ? " " + std::to_string(currAbsorption) + "§e❤§r" : "") + "§e!";
		}

		auto textPkt = TextPacket::createTextPacket<TextPacketType::SystemMessage>(msg);
		PlaySoundPacket soundPkt("random.orb", shooter->getPos(), 0.375f, 0.5f);

		shooter->sendNetworkPacket(textPkt);
		shooter->sendNetworkPacket(soundPkt);
	}
	return result;
}