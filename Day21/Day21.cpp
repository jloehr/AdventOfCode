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

typedef std::function <size_t(PCharacter, PCharacter)> SimpleItemFunction;
typedef std::function <size_t(PCharacter, PCharacter, PItem)> ItemFunction;

static PCharacter LoadBoss(const std::string & FileName);
static bool WouldPlayerWin(PCharacter Player, PCharacter Boss);
static size_t GetLeastAmountToSpendToWin(PCharacter Boss);

static size_t ChooseWeapon(PCharacter Boss, PCharacter Player);
static size_t ChooseArmor(PCharacter Boss, PCharacter Player);
static size_t ChooseFirstRing(PCharacter Boss, PCharacter Player);
static size_t ChooseSecondRing(PCharacter Boss, PCharacter Player, const PItem & FirstRing);
static size_t CheckIfPlayerWouldWin(PCharacter Boss, PCharacter Player, const PItem & SecondRing);

static size_t IterateOverItems(PCharacter Boss, PCharacter Player, const ItemVector & Items, ItemFunction NextFunction, const PItem & InvalidItem = PItem());

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

int main()
{
	PCharacter Boss = LoadBoss("Input.txt");

	size_t MinSpendGold = GetLeastAmountToSpendToWin(Boss);

	std::cout << "Spend Gold: " << MinSpendGold << std::endl;

	system("pause");

	return 0;
}

static PCharacter LoadBoss(const std::string & FileName)
{
	StringVectorVector Input = GetFileLineParts(FileName);

	return std::make_shared<Character>(std::atoi(Input[0][1].c_str()), std::atoi(Input[1][1].c_str()), std::atoi(Input[2][1].c_str()));
}

static bool WouldPlayerWin(PCharacter Player, PCharacter Boss)
{
	return (Player->GetDamagePerRound(Boss->Armor) >= Boss->GetDamagePerRound(Player->Armor));
}

static size_t GetLeastAmountToSpendToWin(PCharacter Boss)
{
	return ChooseWeapon(Boss, std::make_shared<Character>(100));
}

static size_t ChooseWeapon(PCharacter Boss, PCharacter Player)
{
	return IterateOverItems(Boss, Player, Weapons, GetSimpleNextFunctionCall(ChooseArmor));
}

static size_t ChooseArmor(PCharacter Boss, PCharacter Player)
{
	size_t ResultWithoutArmor = ChooseFirstRing(Boss, Player);
	size_t ResultWitArmor = IterateOverItems(Boss, Player, Armor, GetSimpleNextFunctionCall(ChooseFirstRing));

	return std::min(ResultWitArmor, ResultWithoutArmor);
}

static size_t ChooseFirstRing(PCharacter Boss, PCharacter Player)
{
	if (WouldPlayerWin(Player, Boss))
	{
		return Player->SpendGold;
	}

	return IterateOverItems(Boss, Player, Rings, ChooseSecondRing);
}

static size_t ChooseSecondRing(PCharacter Boss, PCharacter Player, const PItem & FirstRing)
{
	if (WouldPlayerWin(Player, Boss))
	{
		return Player->SpendGold;
	}

	return IterateOverItems(Boss, Player, Rings, CheckIfPlayerWouldWin);
}

static size_t CheckIfPlayerWouldWin(PCharacter Boss, PCharacter Player, const PItem & SecondRing)
{
	return (WouldPlayerWin(Player, Boss)) ? Player->SpendGold : SIZE_MAX;
}

static size_t IterateOverItems(PCharacter Boss, PCharacter Player, const ItemVector & Items, ItemFunction NextFunction, const PItem & InvalidItem)
{
	size_t MinGoldSpend = SIZE_MAX;

	for (const PItem & Item : Items)
	{
		if (Item == InvalidItem)
		{
			continue;
		}

		size_t GoldSpend = NextFunction(Boss, std::make_shared<Character>(Player, Item), Item);

		if (GoldSpend < MinGoldSpend)
		{
			MinGoldSpend = GoldSpend;
		}
	}

	return MinGoldSpend;
}

static ItemFunction GetSimpleNextFunctionCall(SimpleItemFunction Funtion)
{
	return [Funtion](PCharacter Boss, PCharacter Player, PItem Item)->size_t { return Funtion(Boss, Player); };
}