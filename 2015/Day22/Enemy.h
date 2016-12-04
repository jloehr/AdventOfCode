#pragma once
#include "stdafx.h"
#include "LivingCreature.h"

#include "Effect.h"

class Enemy;
typedef std::shared_ptr<Enemy> PEnemy;

class Enemy :
	public LivingCreature
{
public:
	Enemy(size_t Hitpoints, size_t Damage);
	Enemy(const Enemy & Other);
	virtual ~Enemy();
	
	void ApplyEffects();

	size_t GetDamage() const;
	Effect<Enemy> & GetPoisonEffect();

private:
	size_t Damage;

	Effect<Enemy> PoisonEffect;
};

