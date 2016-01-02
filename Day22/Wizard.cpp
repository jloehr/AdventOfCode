#include "stdafx.h"
#include "Wizard.h"

#include "Enemy.h"
#include "Settings.h"

const Wizard::SpellCastFunctionVectorType Wizard::SpellCastFunctionVector({
	[](PWizard Caster, PEnemy Target)->bool { return Caster->CastDrain(Target); },
	[](PWizard Caster, PEnemy Target)->bool { return Caster->CastMagicMissle(Target); },
	[](PWizard Caster, PEnemy Target)->bool { return Caster->CastPoison(Target); },
	[](PWizard Caster, PEnemy Target)->bool { return Caster->CastRecharge(); },
	[](PWizard Caster, PEnemy Target)->bool { return Caster->CastShield(); }
});

Wizard::Wizard(size_t Hitpoints, size_t StartingMana)
	:LivingCreature(Hitpoints), CurrentMana(StartingMana), OverallManaSpend(0),
	ShieldEffect(ShieldTurns, (*this), nullptr),
	RechargeEffect(RechargeTurns, (*this), [](Wizard & Holder) { Holder.ReplenishMana(RechargeManaRegain); })
{
}

Wizard::Wizard(const Wizard & Other)
	:LivingCreature(Other), CurrentMana(Other.CurrentMana), OverallManaSpend(Other.OverallManaSpend),
	ShieldEffect(Other.ShieldEffect, (*this)),
	RechargeEffect(Other.RechargeEffect, (*this))
{
}


Wizard::~Wizard()
{
}

bool Wizard::CastMagicMissle(PEnemy Target)
{
	if (!DrainMana(MagicMissleCost))
	{
		return false;
	}

	Target->ApplyDamage(MagicMissleDamage);

	return true;
}

bool Wizard::CastDrain(PEnemy Target)
{
	if (!DrainMana(DrainCost))
	{
		return false;
	}

	Target->ApplyDamage(DrainDamage);
	Heal(DrainHeal);

	return true;
}

bool Wizard::CastShield()
{
	if (ShieldEffect.IsActive())
	{
		return false;
	}

	if (!DrainMana(ShieldCost))
	{
		return false;
	}

	ShieldEffect.Activate();

	return true;
}

bool Wizard::CastPoison(PEnemy Target)
{
	Effect<Enemy> & PoisonEffect = Target->GetPoisonEffect();

	if (PoisonEffect.IsActive())
	{
		return false;
	}

	if (!DrainMana(PoisonCost))
	{
		return false;
	}

	PoisonEffect.Activate();

	return true;
}

bool Wizard::CastRecharge()
{
	if (RechargeEffect.IsActive())
	{
		return false;
	}

	if (!DrainMana(RechargeCost))
	{
		return false;
	}

	RechargeEffect.Activate();

	return true;
}

void Wizard::ApplyDamage(size_t Damage)
{
	if (ShieldEffect.IsActive())
	{
		if (ShieldBonusArmor >= Damage)
		{
			Damage = 1;
		}
		else
		{
			Damage -= ShieldBonusArmor;
		}
	}

	LivingCreature::ApplyDamage(Damage);
}

void Wizard::ApplyEffects()
{
	ShieldEffect.Update();
	RechargeEffect.Update();
}

void Wizard::ReplenishMana(size_t Mana)
{
	CurrentMana += Mana;
}

size_t Wizard::GetSpendMana() const
{
	return OverallManaSpend;
}

bool Wizard::DrainMana(size_t Mana)
{
	if (CurrentMana <= Mana)
	{
		return false;
	}

	CurrentMana -= Mana;
	OverallManaSpend += Mana;

	return true;
}

size_t Wizard::GetMana() const
{
	return CurrentMana;
}

const Effect<Wizard>& Wizard::GetShieldEffect() const
{
	return ShieldEffect;
}

const Effect<Wizard>& Wizard::GetRechargeEffect() const
{
	return RechargeEffect;
}