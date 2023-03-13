#pragma once
#include<utility>
#include <map>
#include "Log.hpp"
#include <future>
#ifdef _DEBUG
#define INLINE inline
#else
#define INLINE __forceinline
#endif 

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

		BlockBeInited = 0x0F,
		BlockNoMine = 0x40,
		BlockHaveMine = 0x8F,
		BlockMarkMine = 0x0E,     //org&0xE0 and | this

	};
	void SetMine();

public:
	template<class Tx, class Ty>
	using pair = std::pair<Tx, Ty>;
	using MinePos = pair<int, int>;
	using MineMap = std::map<MinePos, BlockState>;
	using MineDisplayMap = std::map<pair<int, int>, MinePos>;
	

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
	MineDisplayMap m_DisplayMineMap;
	mutable std::mutex m_DisplayMineMapMutex;
public:
	INLINE int GetMaxX() const { return m_MaxX; };
	INLINE int GetMaxY() const { return m_MaxY; };
	INLINE int GetMaxCount() const { return m_MineCount; };

	pair<bool, MinePos> GetBeClickedMine(const pair<int, int>& clickPoint) const;
	INLINE void AddDisplayMineMap(pair<int, int>& drawPos, MinePos& minePos) 
	{ 
		std::lock_guard lock(m_DisplayMineMapMutex);
		m_DisplayMineMap.insert(std::make_pair(drawPos, minePos)); 
	}
};

