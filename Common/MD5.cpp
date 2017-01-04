#include "stdafx.h"
#include "MD5.h"

MD5::MD5()
	:ResultAsString(32, '0'), StringResultValid(true)
{
	Result.fill(0);
}

const MD5::Hash & MD5::Compute(const ByteVector & Input)
{
	Reset(Input.size()); 
	
	ByteVector::const_iterator InputIt = Input.begin();

	do {
		FillM(InputIt, Input.end());
		ConsumeChunk();
	} while (!SizeAppended);

	FillResult(A, 0);
	FillResult(B, 4);
	FillResult(C, 8);
	FillResult(D, 12);

	return Result;
}

const MD5::Hash & MD5::GetHash()
{
	return Result;
}

const std::string & MD5::GetHexString()
{
	constexpr const char HexCharacters[] = "0123456789abcdef";

	if (!StringResultValid)
	{
		auto StringIt = ResultAsString.begin();

		for (uint8_t Byte : Result)
		{
			uint8_t HighNibble = Byte >> 4;
			uint8_t LowNibble = 0x0F & Byte;

			(*StringIt++) = HexCharacters[HighNibble];
			(*StringIt++) = HexCharacters[LowNibble];
		}

		StringResultValid = true;
	}

	return ResultAsString;
}

void MD5::Reset(size_t InputByteCount)
{
	A = MagicA;
	B = MagicB;
	C = MagicC;
	D = MagicD;

	BitCount = InputByteCount * 8;
	BitAppended = false;
	SizeAppended = false;

	StringResultValid = false;
}

void MD5::FillM(ByteVector::const_iterator & It, const ByteVector::const_iterator & End)
{
	uint8_t * Write = reinterpret_cast<uint8_t *>(M.data());
	uint8_t * WriteEnd = (Write + M.size() * 4);
	size_t ByteIndex = 0;

	// Fill M with Chunk Data
	while (It != End)
	{
		if (FillMByte(*It++, Write, WriteEnd, ByteIndex))
		{
			return;
		}
	}

	// Fill single "1" Bit
	if (!BitAppended)
	{
		BitAppended = true;
		if (FillMByte(0x80, Write, WriteEnd, ByteIndex))
		{
			return;
		}

		// Fill last 32 Bit
		while ((ByteIndex % 4) != 0)
		{
			if (FillMByte(0x00, Write, WriteEnd, ByteIndex))
			{
				return;
			}
		}
	}

	auto MIt = M.begin() + (ByteIndex / 4);

	// Pad with Zeros
	while (MIt != M.end() && (MIt != M.end() - 2))
	{
		(*MIt++) = 0;
	}

	// Append with Size
	if (MIt != M.end())
	{
		(*MIt++) = static_cast<uint32_t>(BitCount);
		(*MIt++) = static_cast<uint32_t>(BitCount >> 32);
		SizeAppended = true;
	}
}

bool MD5::FillMByte(uint8_t Byte, uint8_t * & Write, uint8_t * End, size_t & ByteIndex)
{
	*(Write++) = Byte; 
	ByteIndex++;

	return (Write == End);
}

void MD5::ConsumeChunk()
{
	uint32_t TempA = A;
	uint32_t TempB = B;
	uint32_t TempC = C;
	uint32_t TempD = D;

	RoundOne<0>(TempA, TempB, TempC, TempD);
	RoundTwo<16>(TempA, TempB, TempC, TempD);
	RoundThree<32>(TempA, TempB, TempC, TempD);
	RoundFour<48>(TempA, TempB, TempC, TempD);

	A += TempA;
	B += TempB;
	C += TempC;
	D += TempD;
}

void MD5::RotateValues(uint32_t & TempA, uint32_t & TempB, uint32_t & TempC, uint32_t & TempD, uint32_t F, uint32_t g, size_t i)
{
	uint32_t NewValue = _rotl(TempA + F + K[i] + M[g], BitShift[i]);

	TempA = TempD;
	TempD = TempC;
	TempC = TempB;
	TempB += NewValue;
}

void MD5::FillResult(uint32_t Value, size_t Offset)
{
	uint8_t * Byte = reinterpret_cast<uint8_t *>(&Value);

	for (size_t i = 0; i < 4; i++)
	{
		Result[Offset + i] = Byte[i];
	}
}
