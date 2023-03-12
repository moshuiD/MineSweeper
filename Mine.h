#pragma once
#include<utility>
#include<map>
class Mine
{
private:
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

		BlockBeInited= 0x0F,
		BlockNoMine= 0x40,
		BlockHaveMine= 0x8F,
	};

public:

	using MinePos = std::pair<int, int>;
	using MineMap = std::map<MinePos, BlockState>;
	explicit Mine(int maxX, int maxY, int mineCount) :
		m_MaxX(maxX), m_MaxY(maxY), m_MineCount(mineCount)
	{

	};
	~Mine();
private:
	const int m_MaxX;
	const int m_MaxY;
	const int m_MineCount;
};

