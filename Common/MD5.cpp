#include "stdafx.h"
#include "MD5.h"

MD5::MD5()
{
}

const MD5::Hash & MD5::Compute(const ByteVector & Input)
{
	Reset(Input.size()); 
	
	ByteVector::const_iterator InputIt = Input.begin();

	do {
		FillM(InputIt, Input.end());
		ConsumeChunk();
	} while (InputIt != Input.end());

	FillResult(A, 0);
	FillResult(B, 4);
	FillResult(C, 8);
	FillResult(D, 12);

	return Result;
}

std::string MD5::AsString()
{
	std::stringstream StringStream;

	for (uint8_t Value : Result)
	{
		StringStream << std::hex << std::setfill('0') << std::setw(2) << static_cast<uint16_t>(Value);
	}

	return StringStream.str();
}

void MD5::Reset(size_t InputByteCount)
{
	A = MagicA;
	B = MagicB;
	C = MagicC;
	D = MagicD;

	BitAppended = false;
	BitCount = InputByteCount * 8;

	Result.fill(0);
}

void MD5::FillM(ByteVector::const_iterator & It, const ByteVector::const_iterator & End)
{
	auto BufferIt = Buffer.begin();
	auto MIt = M.begin();

	// Fill M with Chunk Data
	while (It != End)
	{
		if (FillBuffer(*It++, BufferIt, MIt))
		{
			return;
		}
	}

	// Fill single "1" Bit
	if (!BitAppended)
	{
		BitAppended = true;
		if (FillBuffer(0x80, BufferIt, MIt))
		{
			return;
		}

		// Fill last 32 Bit
		while (BufferIt != Buffer.begin())
		{
			if (FillBuffer(0x00, BufferIt, MIt))
			{
				return;
			}
		}
	}

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
	}
}

bool MD5::FillBuffer(uint8_t Byte, uint8Array4::iterator & BufferIt, uint32Array16::iterator & MIt)
{
	(*BufferIt++) = Byte;

	if (BufferIt == Buffer.end())
	{
		BufferIt = Buffer.begin();
		(*MIt++) = *reinterpret_cast<uint32_t *>(Buffer.data());
		if (MIt == M.end())
		{
			return true;
		}
	}

	return false;
}

void MD5::ConsumeChunk()
{
	uint32_t TempA = A;
	uint32_t TempB = B;
	uint32_t TempC = C;
	uint32_t TempD = D;

	for (size_t i = 0; i < 64; i++)
	{
		uint32_t F;
		size_t g;

		switch (i / 16)
		{
		case 0:
			F = (TempB & TempC) | ((~TempB) & TempD);
			g = i;
			break;
		case 1:
			F = (TempD & TempB) | ((~TempD) & TempC);
			g = (5 * i + 1) % 16;
			break;
		case 2:
			F = TempB ^ TempC ^TempD;
			g = (3 * i + 5) % 16;
			break;
		case 3:
			F = TempC ^ (TempB | (~TempD));
			g = (7 * i) % 16;
			break;
		}

		uint32_t NewValue = LeftRotate(TempA + F + K[i] + M[g], BitShift[i]);

		TempA = TempD;
		TempD = TempC;
		TempC = TempB;
		TempB += NewValue;
	}

	A += TempA;
	B += TempB;
	C += TempC;
	D += TempD;
}

void MD5::FillResult(uint32_t Value, size_t Offset)
{
	uint8_t * Byte = reinterpret_cast<uint8_t *>(&Value);

	for (size_t i = 0; i < 4; i++)
	{
		Result[Offset + i] = *(Byte + i);
	}

}

uint32_t MD5::LeftRotate(uint32_t Value, size_t Shift)
{
	return (Value << Shift) | (Value >> (32 - Shift));
}
