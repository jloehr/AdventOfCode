#pragma once

typedef std::vector<uint8_t> ByteVector;

class MD5
{
public:
	typedef std::array<uint8_t, 16> Hash;

	MD5();

	const Hash & Compute(const ByteVector & Input);
	const Hash & GetHash();
	const std::string & GetHexString();

private:
	typedef std::array<uint32_t, 16> uint32Array16;
	typedef std::array<uint8_t, 4> uint8Array4;

	static constexpr uint32_t MagicA = 0x67452301;
	static constexpr uint32_t MagicB = 0xEFCDAB89;
	static constexpr uint32_t MagicC = 0x98BADCFE;
	static constexpr uint32_t MagicD = 0x10325476;
	static constexpr uint32_t BitShift[] = {
		7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,
		5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,
		4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,
		6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21 };
	static constexpr uint32_t K[] = {
		0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
		0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
		0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
		0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
		0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
		0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
		0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
		0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
		0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
		0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
		0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
		0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
		0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
		0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
		0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
		0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391 };

	uint32_t A;
	uint32_t B;
	uint32_t C;
	uint32_t D;
	uint32Array16 M;

	uint64_t BitCount;
	bool BitAppended;
	bool SizeAppended;

	Hash Result;
	bool StringResultValid;
	std::string ResultAsString;

	void Reset(size_t InputByteCount);

	void FillM(ByteVector::const_iterator & It, const ByteVector::const_iterator & End);
	bool FillMByte(uint8_t Byte, uint8_t * & Write, uint8_t * End, size_t & ByteIndex);

	void ConsumeChunk();
	void RotateValues(uint32_t & TempA, uint32_t & TempB, uint32_t & TempC, uint32_t & TempD, uint32_t F, uint32_t g, size_t i);

	void FillResult(uint32_t Value, size_t Offset);	

	template<size_t i>
	void RoundOne(uint32_t & TempA, uint32_t & TempB, uint32_t & TempC, uint32_t & TempD)
	{
		uint32_t F = (TempB & TempC) | ((~TempB) & TempD);
		TempA = TempB + _rotl(TempA + F + K[i] + M[i], BitShift[i]);

		RoundOne<i + 1>(TempD, TempA, TempB, TempC);
	}

	template<>
	void RoundOne<16>(uint32_t & TempA, uint32_t & TempB, uint32_t & TempC, uint32_t & TempD)
	{
		return;
	}

	template<size_t i>
	void RoundTwo(uint32_t & TempA, uint32_t & TempB, uint32_t & TempC, uint32_t & TempD)
	{
		constexpr size_t g = (5 * i + 1) % 16;
		uint32_t F = (TempD & TempB) | ((~TempD) & TempC);
		TempA = TempB + _rotl(TempA + F + K[i] + M[g], BitShift[i]);

		RoundTwo<i + 1>(TempD, TempA, TempB, TempC);
	}

	template<>
	void RoundTwo<32>(uint32_t & TempA, uint32_t & TempB, uint32_t & TempC, uint32_t & TempD)
	{
		return;
	}

	template<size_t i>
	void RoundThree(uint32_t & TempA, uint32_t & TempB, uint32_t & TempC, uint32_t & TempD)
	{
		constexpr size_t g = (3 * i + 5) % 16;
		uint32_t F = TempB ^ TempC ^ TempD;
		TempA = TempB + _rotl(TempA + F + K[i] + M[g], BitShift[i]);

		RoundThree<i + 1>(TempD, TempA, TempB, TempC);
	}

	template<>
	void RoundThree<48>(uint32_t & TempA, uint32_t & TempB, uint32_t & TempC, uint32_t & TempD)
	{
		return;
	}

	template<size_t i>
	void RoundFour(uint32_t & TempA, uint32_t & TempB, uint32_t & TempC, uint32_t & TempD)
	{
		constexpr size_t g = (7 * i) % 16;
		uint32_t F = TempC ^ (TempB | (~TempD));
		TempA = TempB + _rotl(TempA + F + K[i] + M[g], BitShift[i]);

		RoundFour<i + 1>(TempD, TempA, TempB, TempC);
	}

	template<>
	void RoundFour<64>(uint32_t & TempA, uint32_t & TempB, uint32_t & TempC, uint32_t & TempD)
	{
		return;
	}
};

