#include "main.h"
#include <dllentry.h>

DEFAULT_SETTINGS(settings);

void dllenter() {}
void dllexit() {}

THook(void, "?_hurt@Player@@MEAA_NAEBVActorDamageSource@@H_N1@Z",
	Player* player, ActorDamageSource *source, int damage, bool knock, bool ignite) {
	original(player, source, damage, knock, ignite);

	if (!player || !source) return;
	if ((source->getDamagingEntityType() == ActorType::Arrow) && (source->getEntityType() == ActorType::Player_0)) {

		auto shooter = (Player*)LocateService<Level>()->fetchEntity(source->getEntityUniqueID(), false);
		if (!shooter || (shooter == player)) return; // if player shoots themself

		float currHealth = getAttribute(player, 7)->currentVal;
		float currAbsorption = getAttribute(player, 14)->currentVal;

		std::string msg("§e" + player->mPlayerName + " is on§c " + std::to_string((int)currHealth));
		if (settings.useResourcePackGlyphs) {
			msg += "" + (currAbsorption > 0.0f ? " " + std::to_string((int)currAbsorption) + "" : "") + "§e!"; // glyph 0xE1FE, 0xE1FF
		}
		else {
			msg += "§c❤§r" + (currAbsorption > 0.0f ? " " + std::to_string((int)currAbsorption) + "§e❤§r" : "") + "§e!";
		}

		auto textPkt = TextPacket::createTextPacket<TextPacketType::SystemMessage>(msg);
		PlaySoundPacket soundPkt("random.orb", shooter->getPos(), 0.375f, 0.5f);

		shooter->sendNetworkPacket(textPkt);
		shooter->sendNetworkPacket(soundPkt);
	}
}