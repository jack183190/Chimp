#pragma once

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
};