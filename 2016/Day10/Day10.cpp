// Day10.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

struct Bot;
typedef uint16_t BotID;
typedef std::shared_ptr<Bot> BotPointer;
typedef std::map<BotID, BotPointer> BotMap;
typedef std::pair<BotID, uint8_t> BotValuePair;
typedef std::vector<BotValuePair> BotValueVector;
typedef std::queue<BotPointer> BotQueue;

struct Bot
{
	BotID Number;
	std::queue<uint8_t> Values;
	BotPointer LowBot;
	BotPointer HighBot;

	bool HasBothValues()
	{
		return (Values.size() >= 2);
	}

	void PushValue(uint8_t Value)
	{
		Values.push(Value);
	}

	bool Run(BotQueue & CommandQueue)
	{
		constexpr uint8_t LowTarget = 17;
		constexpr uint8_t HighTarget = 61;

		if (!HasBothValues())
			return false;

		uint8_t LowValue = Values.front();
		Values.pop();
		uint8_t HighValue = Values.front();
		Values.pop();

		if (LowValue > HighValue)
		{
			std::swap(LowValue, HighValue);
		}

		LowBot->PushValue(LowValue);
		HighBot->PushValue(HighValue);

		CommandQueue.push(LowBot);
		CommandQueue.push(HighBot);

		return ((LowValue == LowTarget) && (HighValue == HighTarget));
	}
};

BotID MakeOutputID(BotID Number);
BotMap CreateFactory(const StringVectorVector & Lines, BotValueVector & Input);
BotID Run(BotMap & Bots, BotValueVector & Input);

int main()
{
	StringVectorVector Lines = GetFileLineParts("Input.txt");

	BotValueVector Input;
	BotMap Bots = CreateFactory(Lines, Input);

	BotID PartOne = Run(Bots, Input);

	BotPointer Output0 = Bots[MakeOutputID(0)];
	BotPointer Output1 = Bots[MakeOutputID(1)];
	BotPointer Output2 = Bots[MakeOutputID(2)];

	std::cout << "17-61 Bot: " << static_cast<uint32_t>(PartOne) << std::endl;
	std::cout << "Output 0 * 1 * 2: " << static_cast<uint32_t>(Output0->Values.front() * Output1->Values.front() * Output2->Values.front()) << std::endl;

	system("pause");

    return 0;
}

BotID MakeOutputID(BotID Number)
{
	return (0x0100 + Number);
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

void ConnectBots(BotMap & Bots, BotID BotNumber, BotID LowBotNumber, BotID HighBotNumber)
{
	BotPointer Bot = GetBot(Bots, BotNumber);

	Bot->LowBot = GetBot(Bots, LowBotNumber);
	Bot->HighBot = GetBot(Bots, HighBotNumber);
}

BotMap CreateFactory(const StringVectorVector & Lines, BotValueVector & Input)
{
	BotMap Bots;

	for (const StringVector & Line : Lines)
	{
		if (Line[0] == "value")
		{
			uint8_t Value = static_cast<uint8_t>(std::stoul(Line[1]));
			BotID BotNumber = static_cast<BotID>(std::stoul(Line[5]));

			Input.emplace_back(BotNumber, Value);
		}
		else
		{
			BotID BotNumber = static_cast<BotID>(std::stoul(Line[1]));
			BotID LowBotNumber = static_cast<BotID>(std::stoul(Line[6]));
			BotID HighBotNumber = static_cast<BotID>(std::stoul(Line[11]));

			if (Line[5] == "output")
				LowBotNumber = MakeOutputID(LowBotNumber);

			if (Line[10] == "output")
				HighBotNumber = MakeOutputID(HighBotNumber);

			ConnectBots(Bots, BotNumber, LowBotNumber, HighBotNumber);
		}
	}

	return Bots;
}

BotID Run(BotMap & Bots, BotValueVector & Input)
{
	BotID PartOne = 0;
	BotQueue RunQueue;

	for (const BotValuePair & ValueInput : Input)
	{
		BotPointer Bot = GetBot(Bots, ValueInput.first);
		Bot->PushValue(ValueInput.second);

		if (Bot->Run(RunQueue) && (PartOne == 0))
		{
			PartOne = Bot->Number;
		}
	}

	while (!RunQueue.empty())
	{
		BotPointer Bot = RunQueue.front();
		RunQueue.pop();

		if (Bot->Run(RunQueue) && (PartOne == 0))
		{
			PartOne = Bot->Number;
		}
	}

	return PartOne;
}
