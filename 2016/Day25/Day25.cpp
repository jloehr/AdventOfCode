// Day25.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

/*
A = Input

cpy a d			A, D = Input
cpy 7 c			C = 7
cpy 365 b		B = 365
inc d
dec b
jnz b -2
dec c
jnz c -5		D = Input + 7 * 365
cpy d a			A = Input + 7 * 365
jnz 0 0
cpy a b			B = Input + 7 * 365
cpy 0 a			
cpy 2 c			
jnz b 2			
jnz 1 6
dec b			
dec c			
jnz c -4		
inc a			
jnz 1 -7		A = (Input + 7 * 365) / 2; B = 0; C = ((Input + 7 * 365) % 2) + 1
cpy 2 b			
jnz c 2			
jnz 1 4			
dec b			
dec c			
jnz 1 -4		
jnz 0 0			
out b			((Input + 7 * 365) % 2) ? 0 : 1;	
jnz a -19		
jnz 1 -21		

(Input + 7 * 365) % 2 == 0 -> Output: 1
(Input + 7 * 365) % 2 == 1 -> Output: 0
*/

int main()
{

	StringVectorVector Lines = GetFileLineParts("Input.txt");

	const uint64_t C = static_cast<uint64_t>(std::stoull(Lines[1][1]));
	const uint64_t B = static_cast<uint64_t>(std::stoull(Lines[2][1]));

	const uint64_t StaticPart = C * B;

	// Get next valid pattern
	uint64_t ValidPattern = 0;
	while (ValidPattern < StaticPart)
	{
		ValidPattern <<= 1;
		++ValidPattern;
		ValidPattern <<= 1;
	}

	uint64_t ValidInput = ValidPattern - StaticPart;

	std::cout << "Start Input: " << ValidInput << std::endl;

	system("pause");

	return 0;
}
