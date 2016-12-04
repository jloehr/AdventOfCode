#include "stdafx.h"
#include "Wire.h"

Wire::WireMethodMap Wire::GateMethodsMap = { 
	{ std::string("AND"),		&Wire::GetANDMethod		},
	{ std::string("OR"),		&Wire::GetORMethod		},
	{ std::string("LSHIFT"),	&Wire::GetLSHIFTMethod	},
	{ std::string("RSHIFT"),	&Wire::GetRSHIFTMethod	}
};

bool Wire::TryParseString(std::string & Text, uint32_t & Number)
{
	try {
		Number = std::stoi(Text.c_str());
	}
	catch (std::invalid_argument exception)
	{
		return false;
	}

	return true;
}

Wire::Wire(std::string & Identifier)
	:Identifier(Identifier), HasValue(false), Value(0), InputMethod(nullptr)
{
}

Wire::Wire(std::string & Identifier, uint32_t StaticValue)
	:Identifier(Identifier), HasValue(true), Value(StaticValue), InputMethod(nullptr)
{

}

Wire::~Wire()
{
}

void Wire::Setup(std::vector<std::string> & Input, WireNameMap & Wires)
{
	// Setup Identifiers and Method
	switch (Input.size())
	{
	case 1: // Value or Wire
		SetupValueWire(Input[0], Wires, [](uint32_t Value) -> uint32_t { return Value; });
		break;
	case 2: // NOT Value or Wire
		SetupValueWire(Input[1], Wires, [](uint32_t Value) -> uint32_t { return ~Value; });
		break;
	case 3: // Op
		SetupGate(Input[0], Input[1], Input[2], Wires);
		break;
	}

	TryToSignal();
}

void Wire::SetupValueWire(std::string & InputString, WireNameMap & Wires, SingleValueMethod ValueModifier)
{
	if (TryParseString(InputString, Value))
	{
		Value = ValueModifier(Value);
		InputMethod = [this]() { HasValue = true; };
	}
	else
	{
		if (!Wires[InputString])
		{
			Wires[InputString].reset(new Wire(InputString));
		}

		InputA = Wires[InputString];
		InputA->AddNewOutput(this);

		InputMethod = [this, ValueModifier]() { if (InputA->HasSignal()) { Value = ValueModifier(InputA->GetValue()); HasValue = true; }};
	}
}

void Wire::SetupGate(std::string & InputAString, std::string & OpString, std::string & InputBString, WireNameMap & Wires)
{
	SetupInput(InputAString, InputA, Wires);
	SetupInput(InputBString, InputB, Wires);

	OpCode = OpString;

	auto GetGateMethod = GateMethodsMap.find(OpString);

	if (GetGateMethod != GateMethodsMap.end())
	{
		InputMethod = (this->*GetGateMethod->second)();
	}
}

void Wire::SetupInput(std::string & InputString, PWire & InputField, WireNameMap & Wires)
{
	uint32_t StaticValue;
	if (TryParseString(InputString, StaticValue))
	{
		InputField = std::make_shared<Wire>(InputString, StaticValue);
	}
	else
	{
		if (!Wires[InputString])
		{
			Wires[InputString].reset(new Wire(InputString));
		}

		InputField = Wires[InputString];
		InputField->AddNewOutput(this);
	}
}

WireMethod Wire::GetANDMethod()
{
	return CreateInputMethod([](uint32_t A, uint32_t B) -> uint32_t { return (A & B); });
}

WireMethod Wire::GetORMethod()
{
	return CreateInputMethod([](uint32_t A, uint32_t B) -> uint32_t { return (A | B); });
}
WireMethod Wire::GetRSHIFTMethod()
{
	return CreateInputMethod([](uint32_t A, uint32_t B) -> uint32_t { return (A >> B); });
}

WireMethod Wire::GetLSHIFTMethod()
{
	return CreateInputMethod([](uint32_t A, uint32_t B) -> uint32_t { return (A << B); });
}

WireMethod Wire::CreateInputMethod(std::function<uint32_t(uint32_t, uint32_t)> GateMethod)
{
	return [this, GateMethod]() { if (BothInputsReady()) { Value = GateMethod(InputA->GetValue(),InputB->GetValue()); HasValue = true; } };
}

bool Wire::BothInputsReady()
{
	return (InputA->HasSignal() && InputB->HasSignal());
}

bool Wire::HasSignal()
{
	return HasValue;
}

uint32_t Wire::GetValue()
{
	if (HasValue)
	{
		return Value;
	}

	// throw exception
	return 0;
}

void Wire::AddNewOutput(RawPWire NewOutput)
{
	OutputWires.insert(NewOutput);
}

void Wire::TryToSignal()
{
	if (HasValue)
	{
		return;
	}

	if (InputMethod != nullptr)
	{
		InputMethod();
	}
	
	if (HasValue && !OutputWires.empty())
	{
		for (RawPWire OutputWire : OutputWires)
		{
			OutputWire->TryToSignal();
		}
	}
}

void Wire::CascadeReset(std::set<std::string> & ResetWires)
{
	if (ResetWires.find(Identifier) != ResetWires.end())
	{
		return;
	}
	else
	{
		ResetWires.insert(Identifier);
	}

	HasValue = false;
	Value = 0;

	for (RawPWire Output : OutputWires)
	{
		Output->CascadeReset(ResetWires);
	}
}

void Wire::ChangeValue(uint32_t StaticValue)
{
	CascadeReset(std::set<std::string>());
	Value = StaticValue;
	TryToSignal();
}

void Wire::PrintInfo(uint32_t Intend)
{
	std::string IntedString(Intend, '\t');

	std::cout << IntedString << " - " << Identifier << ": ";

	if (HasValue)
	{
		std::cout << " Has Value " << Value << std::endl;
	}
	else
	{
		std::cout << " No Value!" << OpCode << std::endl;

		if (InputA)
		{
			InputA->PrintInfo(Intend + 1);
		}

		if (InputB)
		{
			InputB->PrintInfo(Intend + 1);
		}
	}
}