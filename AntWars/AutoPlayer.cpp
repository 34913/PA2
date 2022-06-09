#include "AutoPlayer.h"

AutoPlayer::AutoPlayer(const std::string& name)
	:Player(name)
{}

void AutoPlayer::SetLimits(int melee, int ranged, int tank)
{
	tankLimit = tank;
	rangedLimit = ranged;
	meleeLimit = melee;
}

void AutoPlayer::Input()
{
	auto now = std::chrono::steady_clock::now();
	std::chrono::milliseconds millis;

	for (auto& x : bases) {

		if (!x.second.train.empty())
			continue;

		selectedBase = x.first;

		if (golds.GetMoney() >= costs[TankAnt::type]) {
			millis = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastTank);

			if (millis.count() > tankLimit) {
				Player::Input(Command::trainTank);
				lastTank = now;

				continue;
			}
		}
		if (golds.GetMoney() >= costs[RangedAnt::type]) {
			millis = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastRanged);

			if (millis.count() > rangedLimit) {
				Player::Input(Command::trainRange);
				lastRanged = now;

				continue;
			}
		}
		if (golds.GetMoney() >= costs[MeleeAnt::type]) {
			millis = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastMelee);

			if (millis.count() > meleeLimit) {
				Player::Input(Command::trainMelee);
				lastMelee = now;

				continue;
			}
		}
	}
}
