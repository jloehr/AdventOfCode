// Day21.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

struct Item {
	size_t Price;
	size_t Damage;
	size_t Armor;

	Item(size_t Price, size_t Damage, size_t Armor)
		:Price(Price), Damage(Damage), Armor(Armor)
	{}
};

typedef std::shared_ptr<Item> PItem;
typedef std::vector<PItem> ItemVector;

struct Character;
typedef std::shared_ptr<Character> PCharacter;

struct Character {
	size_t Hitpoints;
	size_t Damage;
	size_t Armor;
	size_t SpendGold;

	Character(PCharacter Other)
		:Hitpoints(Other->Hitpoints), Damage(Other->Damage), Armor(Other->Armor), SpendGold(Other->SpendGold)
	{}

	Character(PCharacter Other, PItem Item)
		:Hitpoints(Other->Hitpoints), Damage(Other->Damage + Item->Damage), Armor(Other->Armor + Item->Armor), SpendGold(Other->SpendGold + Item->Price)
	{}

	Character(size_t Hitpoints)
		:Hitpoints(Hitpoints), Damage(0), Armor(0), SpendGold(0)
	{}

	Character(size_t Hitpoints, size_t Damage, size_t Armor)
		:Hitpoints(Hitpoints), Damage(Damage), Armor(Armor), SpendGold(0)
	{}

	size_t GetDamagePerRound(size_t OthersArmor)
	{
		if (OthersArmor >= Damage)
		{
			return 1;
		}
		else
		{
			return (Damage - OthersArmor);
		}
	}
};

typedef std::function <size_t(PCharacter, bool, bool)> SimpleItemFunction;
typedef std::function <size_t(PCharacter, bool, bool, PItem)> ItemFunction;

static PCharacter LoadBoss(const std::string & FileName);
static bool WouldPlayerWin(PCharacter Player);
static size_t GetLeastAmountToSpendToWin();
static size_t GetMostAmountToSpendToStillLoose();

static size_t ChooseWeapon(PCharacter Player, bool PlayerShallWin, bool MinimizeGold);
static size_t ChooseArmor(PCharacter Player, bool PlayerShallWin, bool MinimizeGold);
static size_t ChooseFirstRing(PCharacter Player, bool PlayerShallWin, bool MinimizeGold);
static size_t ChooseSecondRing(PCharacter Player, bool PlayerShallWin, bool MinimizeGold, const PItem & FirstRing);
static size_t CheckIfPlayerWouldWin(PCharacter Player, bool PlayerShallWin, bool MinimizeGold, const PItem & Item = PItem());

static size_t IterateOverItems(PCharacter Player, bool PlayerShallWin, bool MinimizeGold, const ItemVector & Items, ItemFunction NextFunction, const PItem & InvalidItem = PItem());

static ItemFunction GetSimpleNextFunctionCall(SimpleItemFunction Funtion);

static const ItemVector Weapons({
	std::make_shared<Item>(8,4,0),
	std::make_shared<Item>(10,5,0),
	std::make_shared<Item>(25,6,0),
	std::make_shared<Item>(40,7,0),
	std::make_shared<Item>(74,8,0)
});

static const ItemVector Armor({
	std::make_shared<Item>(13,0,1),
	std::make_shared<Item>(31,0,2),
	std::make_shared<Item>(53,0,3),
	std::make_shared<Item>(75,0,4),
	std::make_shared<Item>(102,0,5),
});

static const ItemVector Rings({
	std::make_shared<Item>(25,1,0),
	std::make_shared<Item>(20,0,1),
	std::make_shared<Item>(40,0,2),
	std::make_shared<Item>(50,2,0),
	std::make_shared<Item>(80,0,3),
	std::make_shared<Item>(100,3,0),
});

static const PCharacter Boss = LoadBoss("Input.txt");

int main()
{
	size_t MinSpendGold = GetLeastAmountToSpendToWin();
	size_t MaxSpendGold = GetMostAmountToSpendToStillLoose();

	std::cout << "Spend Gold (Part One): " << MinSpendGold << std::endl;
	std::cout << "Spend Gold (Part Two): " << MaxSpendGold << std::endl;

	system("pause");

	return 0;
}

static PCharacter LoadBoss(const std::string & FileName)
{
	StringVectorVector Input = GetFileLineParts(FileName);

	return std::make_shared<Character>(std::atoi(Input[0][2].c_str()), std::atoi(Input[1][1].c_str()), std::atoi(Input[2][1].c_str()));
}

static bool WouldPlayerWin(PCharacter Player)
{
	size_t PlayerRoundsToKillBoss = ((Boss->Hitpoints - 1) / Player->GetDamagePerRound(Boss->Armor)) + 1;
	size_t BossRoundsToKillPlayer = ((Player->Hitpoints - 1) / Boss->GetDamagePerRound(Player->Armor)) + 1;

	return (PlayerRoundsToKillBoss <= BossRoundsToKillPlayer);
}

static size_t GetLeastAmountToSpendToWin()
{
	return ChooseWeapon(std::make_shared<Character>(100), true, true);
}

static size_t GetMostAmountToSpendToStillLoose()
{
	return ChooseWeapon(std::make_shared<Character>(100), false, false);
}

static size_t ChooseWeapon(PCharacter Player, bool PlayerShallWin, bool MinimizeGold)
{
	return IterateOverItems(Player, PlayerShallWin, MinimizeGold, Weapons, GetSimpleNextFunctionCall(ChooseArmor));
}

static size_t ChooseArmor(PCharacter Player, bool PlayerShallWin, bool MinimizeGold)
{
	size_t ResultWithoutArmor = ChooseFirstRing(Player, PlayerShallWin, MinimizeGold);
	size_t ResultWitArmor = IterateOverItems(Player, PlayerShallWin, MinimizeGold, Armor, GetSimpleNextFunctionCall(ChooseFirstRing));

	return (MinimizeGold) ? std::min(ResultWitArmor, ResultWithoutArmor) : std::max(ResultWitArmor, ResultWithoutArmor);
}

static size_t ChooseFirstRing(PCharacter Player, bool PlayerShallWin, bool MinimizeGold)
{
	size_t ResultWithoutAnyRings = CheckIfPlayerWouldWin(Player, PlayerShallWin, MinimizeGold);
	size_t ResultWithRings = IterateOverItems(Player, PlayerShallWin, MinimizeGold, Rings, ChooseSecondRing);

	return (MinimizeGold) ? std::min(ResultWithoutAnyRings, ResultWithRings) : std::max(ResultWithoutAnyRings, ResultWithRings);
}

static size_t ChooseSecondRing(PCharacter Player, bool PlayerShallWin, bool MinimizeGold, const PItem & FirstRing)
{
	size_t ResultWithoutAnyRings = CheckIfPlayerWouldWin(Player, PlayerShallWin, MinimizeGold);
	size_t ResultWithRings = IterateOverItems(Player, PlayerShallWin, MinimizeGold, Rings, CheckIfPlayerWouldWin, FirstRing);

	return (MinimizeGold) ? std::min(ResultWithoutAnyRings, ResultWithRings) : std::max(ResultWithoutAnyRings, ResultWithRings);
}

static size_t CheckIfPlayerWouldWin(PCharacter Player, bool PlayerShallWin, bool MinimizeGold, const PItem & Item)
{
	return (WouldPlayerWin(Player) == PlayerShallWin) ? Player->SpendGold : (MinimizeGold) ? SIZE_MAX : 0;
}

static size_t IterateOverItems(PCharacter Player, bool PlayerShallWin, bool MinimizeGold, const ItemVector & Items, ItemFunction NextFunction, const PItem & InvalidItem)
{
	size_t BestGoldSpend = (MinimizeGold) ? SIZE_MAX : 0;

	for (const PItem & Item : Items)
	{
		if (Item == InvalidItem)
		{
			continue;
		}

		size_t GoldSpend = NextFunction(std::make_shared<Character>(Player, Item), PlayerShallWin, MinimizeGold, Item);

		if ((MinimizeGold && (GoldSpend < BestGoldSpend)) || (!MinimizeGold && (GoldSpend > BestGoldSpend)))
		{
			BestGoldSpend = GoldSpend;
		}
	}

	return BestGoldSpend;
}

static ItemFunction GetSimpleNextFunctionCall(SimpleItemFunction Funtion)
{
	return [Funtion](PCharacter Player, bool PlayerShallWin, bool MinimizeGold, PItem Item)->size_t { return Funtion(Player, PlayerShallWin, MinimizeGold); };
}