#pragma once

enum class UpgradeType {
	ATTACK_SPEED,
	ATTACK_DAMAGE
};

struct UpgradableComponent {
	int NumAttackSpeedUpgrades = 0;
	int NumDamageUpgrades = 0;

	int GetAttackSpeedUpgradeCost() const {
		return 100 * (NumAttackSpeedUpgrades + 1);
	}

	float GetAttackSpeedMultiplier() const {
		return 1.0f + 0.1f * NumAttackSpeedUpgrades;
	}

	int GetDamageUpgradeCost() const {
		return 250 * (NumDamageUpgrades + 1);
	}

	int GetDamage(int base) const {
		return base + NumDamageUpgrades;
	}

	void Upgrade(UpgradeType type) {
		switch (type) {
		case UpgradeType::ATTACK_SPEED:
			NumAttackSpeedUpgrades++;
			break;
		case UpgradeType::ATTACK_DAMAGE:
			NumDamageUpgrades++;
			break;
		default:
			assert(false);
			break;
		}
	}

	int GetUpgradeCost(UpgradeType type) const {
		switch (type) {
		case UpgradeType::ATTACK_SPEED:
			return GetAttackSpeedUpgradeCost();
		case UpgradeType::ATTACK_DAMAGE:
			return GetDamageUpgradeCost();
		default:
			assert(false);
			return 0;
		}
	}
};