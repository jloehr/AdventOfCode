#pragma once
class LivingCreature
{
public:
	LivingCreature(size_t Hitpoints);
	LivingCreature(const LivingCreature & Other);
	virtual ~LivingCreature();

	virtual void ApplyDamage(size_t Damage);
	virtual void Heal(size_t Hitpoints);
	virtual bool IsAlive() const;

	size_t GetHitpoints() const;

private:
	size_t Hitpoints;
};

