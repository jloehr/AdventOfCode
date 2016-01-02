// Day22.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "Enemy.h"
#include "Wizard.h"
#include "Settings.h"

typedef std::pair<PEnemy, PWizard> BattleState;
typedef std::function<bool(const BattleState &, const BattleState &)> BattleStateComparator;
typedef std::set<BattleState, BattleStateComparator> BattleStateSet;
typedef std::vector<BattleState> BattleStateVector;

static PEnemy LoadBoss(const std::string & InputFile);
static size_t GetLEastAmountOfManaToSpendToKillBoss();

static bool BossTurn(BattleState State);
static BattleStateVector PlayerTurn(BattleState State);
static void PreTurn(BattleState State);

static BattleStateComparator SortAscBySpendMana = [](const BattleState & A, const BattleState & B)->bool 
{	
	// Spend Mana
	if (A.second->GetSpendMana() != B.second->GetSpendMana())
	{
		return (A.second->GetSpendMana() < B.second->GetSpendMana());
	}

	// Boss Hitpoints
	if (A.first->GetHitpoints() != B.first->GetHitpoints())
	{
		return (A.first->GetHitpoints() < B.first->GetHitpoints());
	}

	// Player Hitpoints
	if (A.second->GetHitpoints() != B.second->GetHitpoints())
	{
		return (A.second->GetHitpoints() > B.second->GetHitpoints());
	}

	// Player Mana
	if (A.second->GetMana() != B.second->GetMana())
	{
		return (A.second->GetMana() > B.second->GetMana());
	}

	// Enemy Poison Effect
	if (A.first->GetPoisonEffect().GetRemainingTurns() != B.first->GetPoisonEffect().GetRemainingTurns())
	{
		return (A.first->GetPoisonEffect().GetRemainingTurns() > B.first->GetPoisonEffect().GetRemainingTurns());
	}

	// Player Shield
	if (A.second->GetShieldEffect().GetRemainingTurns() != B.second->GetShieldEffect().GetRemainingTurns())
	{
		return (A.second->GetShieldEffect().GetRemainingTurns() > B.second->GetShieldEffect().GetRemainingTurns());
	}

	// Player Recharge
	//if (A.second->GetRechargeEffect().GetRemainingTurns() != B.second->GetRechargeEffect().GetRemainingTurns())
	{
		return (A.second->GetRechargeEffect().GetRemainingTurns() > B.second->GetRechargeEffect().GetRemainingTurns());
	}

};

int main()
{
	size_t SpendMana = GetLEastAmountOfManaToSpendToKillBoss();

	std::cout << "Spend Mana: " << SpendMana << std::endl;

	system("pause");

    return 0;
}

static PEnemy LoadBoss(const std::string & InputFile)
{
	StringVectorVector InputParts = GetFileLineParts(InputFile);

	return std::make_shared<Enemy>(std::atoi(InputParts[0][2].c_str()), std::atoi(InputParts[1][1].c_str()));
}

static size_t GetLEastAmountOfManaToSpendToKillBoss()
{
	PEnemy Boss = LoadBoss("Input.txt");
	PWizard Player = std::make_shared<Wizard>(PlayerStartingHitpoints, PlayerStartingMana);
	BattleStateSet BattleStates(SortAscBySpendMana);

	BattleStates.insert(std::make_pair(Boss, Player));

	do
	{
		auto BestStateIter = BattleStates.begin(); 

		if (!BestStateIter->first->IsAlive())
		{
			return BestStateIter->second->GetSpendMana();
		}

		BattleState CurrentState = (*BestStateIter);
		BattleStates.erase(BestStateIter);

		BattleStateVector NewStates = PlayerTurn(CurrentState);
		for (BattleState & NewState : NewStates)
		{
			BattleStates.insert(NewState);
		}

	} while (!BattleStates.empty());

	return SIZE_MAX;
}

static bool BossTurn(BattleState State)
{
	PEnemy Boss = State.first;
	PWizard Player = State.second;

	PreTurn(State);
	if (!Boss->IsAlive())
	{
		return true;
	}

	Player->ApplyDamage(Boss->GetDamage());
	
	return Player->IsAlive();
}

static BattleStateVector PlayerTurn(BattleState State)
{
	PEnemy Boss = State.first;
	PWizard Player = State.second;

	PreTurn(State);
	if (!Boss->IsAlive())
	{
		return BattleStateVector({ State });
	}

	BattleStateVector NewStates;

	for (auto & Spell : Wizard::SpellCastFunctionVector)
	{
		PEnemy BossCopy = std::make_shared<Enemy>(*Boss);
		PWizard PlayerCopy = std::make_shared<Wizard>(*Player);

		if (Spell(PlayerCopy, BossCopy))
		{
			if (Boss->IsAlive())
			{
				bool PlayerStillAlive = BossTurn(std::make_pair(BossCopy, PlayerCopy));
				if (!PlayerStillAlive)
				{
					continue;
				}
			}

			NewStates.push_back(std::make_pair(BossCopy, PlayerCopy));
		}
	}

	return NewStates;
}


static void PreTurn(BattleState State)
{
	State.first->ApplyEffects();
	State.second->ApplyEffects();
}