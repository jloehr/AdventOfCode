#pragma once

#include "stdafx.h"

class Wire;

typedef Wire * RawPWire;
typedef std::shared_ptr<Wire> PWire;
typedef std::map<std::string, PWire> WireNameMap;
typedef std::set<RawPWire> WireSet;
typedef std::function<void ()> WireMethod;

class Wire
{
public:
	Wire(std::string & Identifier);
	Wire(std::string & Identifier, uint32_t StaticValue);
	~Wire();

	void Setup(std::vector<std::string> & Input, WireNameMap & Wires);

	bool HasSignal();
	uint32_t GetValue();
	void AddNewOutput(RawPWire NewOutput);
	void TryToSignal();

	void CascadeReset(std::set<std::string> & ResetWires);
	void ChangeValue(uint32_t StaticValue);

	void PrintInfo(uint32_t Intend);

private:
	typedef std::function<uint32_t(uint32_t)> SingleValueMethod;
	typedef WireMethod(Wire::*PGetWireMethod)();
	typedef std::map<std::string, PGetWireMethod> WireMethodMap;

	static WireMethodMap GateMethodsMap;
	static bool TryParseString(std::string & Text, uint32_t & Number);

	std::string Identifier;
	std::string OpCode;

	PWire InputA;
	PWire InputB;
	WireMethod InputMethod;
	WireSet OutputWires;

	bool HasValue;
	uint32_t Value;

	void SetupValueWire(std::string & InputString, WireNameMap & Wires, SingleValueMethod ValueModifier);
	void SetupGate(std::string & InputAString, std::string & OpString, std::string & InputBString, WireNameMap & Wires);
	void SetupInput(std::string & InputString, PWire & InputField, WireNameMap & Wires);

	WireMethod GetANDMethod();
	WireMethod GetORMethod();
	WireMethod GetRSHIFTMethod();
	WireMethod GetLSHIFTMethod();

	WireMethod CreateInputMethod(std::function<uint32_t(uint32_t, uint32_t)> GateMethod);

	bool BothInputsReady();
};

