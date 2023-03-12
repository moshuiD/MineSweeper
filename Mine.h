#pragma once
#include<utility>
#include<map>
#include "Log.hpp"
#ifdef _DEBUG
#define INLINE inline
#else
#define INLINE __forceinline
#endif 

class Mine
{
private:

	struct RangeCmp
	{
		/*using KeyValue = std::pair<int, int>;
		bool operator ()(const KeyValue& p1, const KeyValue& p2) const
		{
			if(p1.first)
		}*/
	};

	enum BlockState
	{
		HaveOne = 1,
		HaveTwo = 2,
		HaveThree = 3,
		HaveFour = 4,
		HaveFive = 5,
		HaveSix = 6,
		HaveSeven = 7,
		HaveEight = 8,
		HaveNine = 9,

		BlockBeInited = 0x0F,
		BlockNoMine = 0x40,
		BlockHaveMine = 0x8F,
		BlockMarkMine=0x0E,     //org&0xE0 and | this

	};
	void SetMine();
public:

	using MinePos = std::pair<int, int>;
	using MineMap = std::map<MinePos, BlockState>;
	using MineDisplayMap = std::map<std::pair<int, int>, MinePos, RangeCmp>;
	explicit Mine(int maxX, int maxY, int mineCount) :
		m_MaxX(maxX), m_MaxY(maxY), m_MineCount(mineCount)
	{
		Log("Mine被实例化一次");
		SetMine();
	};
	~Mine();


private:
	const int m_MaxX;
	const int m_MaxY;
	const int m_MineCount;
	MineMap m_MineMap;
public:
	INLINE int GetMaxX() const { return m_MaxX; };
	INLINE int GetMaxY() const { return m_MaxY; };
	INLINE int GetMaxCount() const {return m_MineCount;};
};

