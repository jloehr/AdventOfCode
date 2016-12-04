#pragma once


const static std::set<char> BadLetters = { 'i', 'j', 'l' };

template<size_t Size>
class Password
{
public:
	Password(std::string & Start) 
	{
		Value.fill('a'); 
		auto Character = Start.rbegin();
		auto Field = Value.rbegin();

		for (; (Character != Start.rend()) && (Field != Value.rend()); ++Character, ++Field )
		{
			*Field = *Character;
		}

		StraightLettersPosition = Value.rend();
		DoubleLetterPositions = { Value.rend(), Value.rend() };

		RemoveBadLetters();
		FindStraightLetters();
		FindDoubleLetterPositions();

		SkipIncrement = IsValid();
	}

	~Password() {}

	Password & operator++()
	{
		if (SkipIncrement)
		{
			SkipIncrement = false;
		}
		else
		{
			Increment(Value.rbegin());
		}

		while (!IsValid())
		{
			DoStep();

			//std::cout << "- " << Value.data() << std::endl;
		}

		return *this;
	}

	const char * GetData() const
	{
		return Value.data();
	}


private:
	typedef std::array<char, Size> Array;
	Array Value;

	bool SkipIncrement;

	typename Array::reverse_iterator StraightLettersPosition;
	std::array<typename Array::reverse_iterator, 2> DoubleLetterPositions;

	bool HasStraightLetters()
	{
		return (StraightLettersPosition != Value.rend());
	}

	bool HasDoubleLetters()
	{
		return (DoubleLetterPositions[1] != Value.rend());
	}

	bool IsValid()
	{
		return (HasStraightLetters() && HasDoubleLetters());
	}

	void Increment(typename Array::reverse_iterator Position)
	{
		if (Position == Value.rend())
		{
			return;
		}

		bool FoundBadLetter = false;

		do {
			++(*Position);
			FoundBadLetter = (BadLetters.find(*Position) != BadLetters.end());
		} while (FoundBadLetter);

		if ((*Position) > 'z')
		{
			(*Position) = 'a';
			CheckLetter(Position);
			Increment(Position + 1);
		}

		CheckLetter(Position);
	}

	void DoStep()
	{
		if (Value[Size - 1] == Value[Size - 2])
		{
			Increment(Value.rbegin());
		}
		else
		{
			Increment(Value.rbegin() + 1);
			CheckLetter(Value.rbegin());
		}
	}

	void RemoveBadLetters()
	{
		for (auto Character = Value.begin(); Character != Value.end(); ++Character)
		{
			if (BadLetters.find(*Character) != BadLetters.end())
			{
				Increment(typename Array::reverse_iterator(Character + 1));
				break;
			}
		}
	}

	void FindStraightLetters()
	{
		for (auto Iter = Value.begin(); (Iter + 3) < Value.end(); ++Iter)
		{
			if (((*Iter) == ((*(Iter + 1)) - 1)) && (((*(Iter + 1))) == ((*(Iter + 2)) - 1)))
			{
				StraightLettersPosition = typename Array::reverse_iterator(Iter + 3);
				return;
			}
		}
	}

	void FindDoubleLetterPositions()
	{
		for (auto Iter = Value.begin(); (Iter + 1) < Value.end(); ++Iter)
		{
			if ((*Iter) == (*(Iter + 1)))
			{
				typename Array::reverse_iterator RIter = typename Array::reverse_iterator(Iter + 2);

				if (DoubleLetterPositions[0] == Value.rend())
				{
					DoubleLetterPositions[0] = RIter;
				}
				else if (DoubleLetterPositions[0] != RIter)
				{
					DoubleLetterPositions[1] = RIter;
					return;
				}
			}
		}
	}

	void CheckLetter(typename Array::reverse_iterator Position)
	{
		CheckStraighLetter(Position);
		CheckDoubleLetter(Position);
	}

	void CheckStraighLetter(typename Array::reverse_iterator Position)
	{
		if ((StraightLettersPosition == Position) || (StraightLettersPosition == Value.rend()))
		{
			if (((*Position) == ((*(Position + 1)) + 1)) && ((*(Position + 1)) == ((*(Position + 2)) + 1)))
			{
				StraightLettersPosition = Position;
			}
			else
			{
				StraightLettersPosition = Value.rend();
			}
		}
	}

	void CheckDoubleLetter(typename Array::reverse_iterator Position)
	{
		if ((DoubleLetterPositions[0] == Position) || (DoubleLetterPositions[0] == Value.rend()))
		{
			if ((*Position) == (*(Position + 1)))
			{
				DoubleLetterPositions[0] = Position;
			}
			else
			{
				if (DoubleLetterPositions[1] != Value.rend())
				{
					DoubleLetterPositions[0] = DoubleLetterPositions[1];
					DoubleLetterPositions[1] = Value.rend();
				}
				else
				{
					DoubleLetterPositions[0] = Value.rend();
				}

			}
		}
		else if ((DoubleLetterPositions[1] == Position) || ((DoubleLetterPositions[1] == Value.rend()) && (DoubleLetterPositions[0] != (Position + 1))))
		{
			if ((*Position) == (*(Position + 1)))
			{
				DoubleLetterPositions[1] = Position;
			}
			else
			{
				DoubleLetterPositions[1] = Value.rend();
			}
		}	
	}
};

template<size_t Size>
std::ostream& operator<<(std::ostream& out, const Password<Size>& p) {
	std::string Buffer(p.GetData(), Size);
	return out << Buffer;
}

