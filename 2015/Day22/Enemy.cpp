#include "stdafx.h"
#include "Enemy.h"

#include "Settings.h"

Enemy::Enemy(size_t Hitpoints, size_t Damage)
	: LivingCreature(Hitpoints), Damage(Damage), PoisonEffect(PoisonTurns, (*this), [](Enemy & Holder) { Holder.ApplyDamage(PoisonDamage); })
{
}

Enemy::Enemy(const Enemy & Other)
	: LivingCreature(Other), Damage(Other.Damage), PoisonEffect(Other.PoisonEffect, (*this))
{
}

Enemy::~Enemy()
{
}

void Enemy::ApplyEffects()
{
	PoisonEffect.Update();
}

size_t Enemy::GetDamage() const
{
	return Damage;
}

Effect<Enemy>& Enemy::GetPoisonEffect()
{
	return PoisonEffect;
}