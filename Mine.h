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


	void SetMine();

public:
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
		BlockMarkedMine = 0x8E,  

		BlockMarked = 0x0E,     //org&0xE0 and | this

		Error = 0xFF,
	};
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
	mutable std::mutex m_MineMapMutex;
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

	//template<class Pr>
	//INLINE pair<bool, pair<MinePos, BlockState>> GetMineMapByFunc(Pr f_cmp) const
	//{
	//	auto it = std::find_if(m_MineMap.begin(), m_MineMap.end(), f_cmp);
	//	if (it != m_MineMap.end()) {
	//		return { true,{it->first,it->second} };
	//	}
	//	else {
	//		return { false,{} };
	//	}
	//}

	INLINE BlockState GetBlockStateByPos(int x, int y)const {
		std::lock_guard lock(m_MineMapMutex);
		auto it = std::find_if(m_MineMap.begin(), m_MineMap.end(), [&](const std::pair<std::pair<int, int>, Mine::BlockState>& v) {
			if (v.first.first == x && v.first.second == y)
			return true;
			else
				return false;
			});
		if (it != m_MineMap.end()) {
			return it->second;
		}
		else {
			return Error;
		}
	}


	INLINE	bool SetBlockStateByPos(const pair<int, int>& pos, BlockState state) 
	{
		std::lock_guard lock(m_MineMapMutex);
		auto it = m_MineMap.find(pos);
		if (it != m_MineMap.end()) {
			(*it).second = state;
			return true;
		}
		else {
			return false;
		}
	}

	template<class _Fn>
	INLINE bool SetBlockStateByPos(const pair<int, int>& pos, _Fn func) 
	{
		std::lock_guard lock(m_MineMapMutex);
		auto it = m_MineMap.find(pos);
		if (it != m_MineMap.end()) {
			func(*it, (*it).second);
			return true;
		}
		else {
			return false;
		}
	}
};

