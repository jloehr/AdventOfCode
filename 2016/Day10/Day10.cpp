// Day10.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

struct Bot;
typedef uint16_t BotID;
typedef std::shared_ptr<Bot> BotPointer;
typedef std::map<BotID, BotPointer> BotMap;

struct Bot
{
	BotID Number;
	uint8_t LowValue = 0;
	uint8_t HighValue = 0;
	BotPointer LowBot;
	BotPointer HighBot;

	bool HasBothValues()
	{
		return (HighValue != 0);
	}
};

BotMap CreateFactory(const StringVectorVector & Lines);
BotID GetBotNumber(const BotMap & Bots, uint8_t Low, uint8_t Hight);

int main()
{
	StringVectorVector Lines = GetFileLineParts("Input.txt");

	BotMap Bots = CreateFactory(Lines);

	std::cout << "17-61 Bot: " << static_cast<uint32_t>(GetBotNumber(Bots, 17, 61)) << std::endl;

	system("pause");

    return 0;
}

BotID MakeOutputID(BotID Number)
{
	return (0x0100 & Number);
}

BotPointer GetBot(BotMap & Bots, BotID BotNumber)
{
	BotPointer & Bot = Bots[BotNumber];

	if (!Bot)
	{
		Bot = std::make_shared<::Bot>();
		Bot->Number = BotNumber;
	}

	return Bot;
}

void GiveBotValue(BotPointer Bot, uint8_t Value)
{
	if (Bot->HasBothValues())
	{
		return;
	}

	if (Bot->LowValue == 0)
	{
		Bot->LowValue = Value;
		return;
	}

	if (Bot->LowValue > Value)
	{
		std::swap(Bot->LowValue, Value);
	}

	Bot->HighValue = Value;


	if(Bot->LowBot)
		GiveBotValue(Bot->LowBot, Bot->LowValue);
	if(Bot->HighBot)
		GiveBotValue(Bot->HighBot, Bot->HighValue);
}

void GiveBotValue(BotMap & Bots, BotID BotNumber, uint8_t Value)
{
	GiveBotValue(GetBot(Bots, BotNumber), Value);
}


void FillBot(BotMap & Bots, BotID BotNumber, BotID LowBotNumber, BotID HighBotNumber)
{
	BotPointer Bot = GetBot(Bots, BotNumber);

	Bot->LowBot = GetBot(Bots, LowBotNumber);
	Bot->HighBot = GetBot(Bots, HighBotNumber);

	if (Bot->HasBothValues())
	{
		GiveBotValue(Bot->LowBot, Bot->LowValue);
		GiveBotValue(Bot->HighBot, Bot->HighValue);
	}
}

BotMap CreateFactory(const StringVectorVector & Lines)
{
	BotMap Bots;

	for (const StringVector & Line : Lines)
	{
		if (Line[0] == "value")
		{
			uint8_t Value = static_cast<uint8_t>(std::stoul(Line[1]));
			BotID BotNumber = static_cast<BotID>(std::stoul(Line[5]));

			GiveBotValue(Bots, BotNumber, Value);
		}
		else
		{
			BotID BotNumber = static_cast<BotID>(std::stoul(Line[1]));
			BotID LowBotNumber = static_cast<BotID>(std::stoul(Line[6]));
			BotID HighBotNumber = static_cast<BotID>(std::stoul(Line[11]));

			if (Line[5] == "output")
				LowBotNumber = MakeOutputID(LowBotNumber);

			if (Line[10] == "output")
				LowBotNumber = MakeOutputID(LowBotNumber);

			FillBot(Bots, BotNumber, LowBotNumber, HighBotNumber);
		}
	}

	return Bots;
}

BotID GetBotNumber(const BotMap & Bots, uint8_t Low, uint8_t Hight)
{
	for (const auto & Bot : Bots)
	{
		if ((Bot.second->LowValue == Low) && (Bot.second->HighValue == Hight))
		{
			return Bot.first;
		}
	}

	return 0;
}