#pragma once
#include "LivingCreature.h"

#include "Effect.h"

class Wizard;
typedef std::shared_ptr<Wizard> PWizard;

class Enemy;
typedef std::shared_ptr<Enemy> PEnemy;

class Wizard :
	public LivingCreature
{
public:
	typedef std::function<bool(PWizard, PEnemy)> SpellCastFunction;
	typedef std::vector<SpellCastFunction> SpellCastFunctionVectorType;
	static const SpellCastFunctionVectorType SpellCastFunctionVector;

	Wizard(size_t Hitpoints, size_t StartingMana);
	Wizard(const Wizard & Other);
	virtual ~Wizard();

	bool CastMagicMissle(PEnemy Target);
	bool CastDrain(PEnemy Target);
	bool CastShield();
	bool CastPoison(PEnemy Target);
	bool CastRecharge();

	virtual void ApplyDamage(size_t Damage) override;
	void ApplyPureDamage(size_t Damage);
	void ApplyEffects();
	void ReplenishMana(size_t Mana);

	size_t GetSpendMana() const;
	size_t GetMana() const;
	const Effect<Wizard> & GetShieldEffect() const;
	const Effect<Wizard> & GetRechargeEffect() const;

private:
	size_t CurrentMana;
	size_t OverallManaSpend;

	Effect<Wizard> ShieldEffect;
	Effect<Wizard> RechargeEffect;

	bool DrainMana(size_t Mana);
};

