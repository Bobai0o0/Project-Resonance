// Transition / Easing types for Sweep primitive
#pragma once
#include <cstring>

enum TransitionType
{
	Linear = 0,

	EaseInSine,
	EaseOutSine,
	EaseInOutSine,

	EaseInQuad,
	EaseOutQuad,
	EaseInOutQuad,

	EaseInCubic,
	EaseOutCubic,
	EaseInOutCubic,

	EaseInQuart,
	EaseOutQuart,
	EaseInOutQuart,

	EaseInQuint,
	EaseOutQuint,
	EaseInOutQuint,

	EaseInExpo,
	EaseOutExpo,
	EaseInOutExpo,

	EaseInCirc,
	EaseOutCirc,
	EaseInOutCirc,

	EaseInBack,
	EaseOutBack,
	EaseInOutBack,

	EaseInElastic,
	EaseOutElastic,
	EaseInOutElastic,

	EaseInBounce,
	EaseOutBounce,
	EaseInOutBounce
};

struct TransitionMap
{
	const char *name;
	TransitionType type;
};

static const TransitionMap transitionMap[] =
	{
		{"Linear", Linear},

		{"EaseInSine", EaseInSine},
		{"EaseOutSine", EaseOutSine},
		{"EaseInOutSine", EaseInOutSine},

		{"EaseInQuad", EaseInQuad},
		{"EaseOutQuad", EaseOutQuad},
		{"EaseInOutQuad", EaseInOutQuad},

		{"EaseInCubic", EaseInCubic},
		{"EaseOutCubic", EaseOutCubic},
		{"EaseInOutCubic", EaseInOutCubic},

		{"EaseInQuart", EaseInQuart},
		{"EaseOutQuart", EaseOutQuart},
		{"EaseInOutQuart", EaseInOutQuart},

		{"EaseInQuint", EaseInQuint},
		{"EaseOutQuint", EaseOutQuint},
		{"EaseInOutQuint", EaseInOutQuint},

		{"EaseInExpo", EaseInExpo},
		{"EaseOutExpo", EaseOutExpo},
		{"EaseInOutExpo", EaseInOutExpo},

		{"EaseInCirc", EaseInCirc},
		{"EaseOutCirc", EaseOutCirc},
		{"EaseInOutCirc", EaseInOutCirc},

		{"EaseInBack", EaseInBack},
		{"EaseOutBack", EaseOutBack},
		{"EaseInOutBack", EaseInOutBack},

		{"EaseInElastic", EaseInElastic},
		{"EaseOutElastic", EaseOutElastic},
		{"EaseInOutElastic", EaseInOutElastic},

		{"EaseInBounce", EaseInBounce},
		{"EaseOutBounce", EaseOutBounce},
		{"EaseInOutBounce", EaseInOutBounce}};

class StartEndParam
{
private:
	float mStart{0.0f};
	float mEnd{0.0f};
	TransitionType mTransitionType{TransitionType::Linear};

	static int strcasecmp(const char *s1, const char *s2)
	{
		while (*s1 && *s2)
		{
			char c1 = (*s1 >= 'A' && *s1 <= 'Z') ? (*s1 + 32) : *s1;
			char c2 = (*s2 >= 'A' && *s2 <= 'Z') ? (*s2 + 32) : *s2;
			if (c1 != c2)
				return c1 - c2;
			s1++;
			s2++;
		}
		return (*s1 - *s2);
	}

public:
	StartEndParam() = default;

	StartEndParam(float start, float end, TransitionType type)
		: mStart(start), mEnd(end), mTransitionType(type)
	{
	}

	StartEndParam(float start, float end, const char *type)
		: mStart(start), mEnd(end), mTransitionType(TransitionTypeFromString(type))
	{
	}

	StartEndParam(const StartEndParam &other)
		: mStart(other.mStart),
		  mEnd(other.mEnd),
		  mTransitionType(other.mTransitionType)
	{
	}

	StartEndParam &operator=(const StartEndParam &other)
	{
		if (this != &other)
		{
			mStart = other.mStart;
			mEnd = other.mEnd;
			mTransitionType = other.mTransitionType;
		}
		return *this;
	}

	float &getStartParam() { return mStart; }
	float &getEndParam() { return mEnd; }
	TransitionType getTransitionType() const { return mTransitionType; }
	static TransitionType TransitionTypeFromString(const char *value)
	{
		for (size_t i = 0; i < sizeof(transitionMap) / sizeof(transitionMap[0]); ++i)
		{
			if (strcasecmp(value, transitionMap[i].name) == 0)
				return transitionMap[i].type;
		}
		return Linear;
	}
};
