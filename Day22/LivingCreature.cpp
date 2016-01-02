#include "stdafx.h"
#include "LivingCreature.h"


LivingCreature::LivingCreature(size_t Hitpoints)
	:Hitpoints(Hitpoints)
{
}

LivingCreature::LivingCreature(const LivingCreature & Other)
	:Hitpoints(Other.Hitpoints)
{
}

LivingCreature::~LivingCreature()
{
}

void LivingCreature::ApplyDamage(size_t Damage)
{
	if (Hitpoints > Damage)
	{
		Hitpoints -= Damage;
	}
	else
	{
		Hitpoints = 0;
	}
}

void LivingCreature::Heal(size_t Hitpoints)
{
	this->Hitpoints += Hitpoints;
}

bool LivingCreature::IsAlive() const
{
	return (Hitpoints != 0);
}

size_t LivingCreature::GetHitpoints() const
{
	return Hitpoints;
}