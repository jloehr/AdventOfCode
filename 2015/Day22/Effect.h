#pragma once

template<class HolderType>
class Effect
{
public:
	typedef std::function<void(HolderType &)> EffectActionFunction;

	Effect(size_t Duration, HolderType & Holder, EffectActionFunction EffectAction)
		:Duration(Duration), RemainingTurns(0), Holder(Holder), EffectAction(EffectAction)
	{}

	Effect(const Effect & Other, HolderType & NewHolder)
		: Duration(Other.Duration), RemainingTurns(Other.RemainingTurns), Holder(NewHolder), EffectAction(Other.EffectAction)
	{}

	virtual ~Effect()
	{}

	void Activate()
	{
		RemainingTurns = Duration;
	}

	void Update()
	{
		if (RemainingTurns > 0)
		{
			DoEffect();
			--RemainingTurns;
		}
	}

	bool IsActive() const
	{
		return (RemainingTurns != 0);
	}

	size_t GetRemainingTurns() const
	{
		return RemainingTurns;
	}

private:
	const size_t Duration;
	size_t RemainingTurns;

	HolderType & Holder;
	EffectActionFunction EffectAction;

	void DoEffect()
	{
		if (EffectAction)
		{
			EffectAction(Holder);
		}
	}
};

