#pragma once
#include<utility>
#include <map>
#include "Log.hpp"
#include <future>
#include "Timer.hpp"
#ifdef _DEBUG
#define INLINE inline
#else
#define INLINE __forceinline
#endif 

class Mine: private Timer
{
private:
	void SetMine();
	void ChangeMine();
	void HandleClickBlock(const std::pair<int, int>& pos);
	int GetNearMineNum(const std::pair<int, int>& pos);
public:
	enum GameState
	{
		BeInit,
		InGame,
		Win,
		Lose,
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
		//org&0xE0
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

	
	explicit Mine(int maxX, int maxY, int mineCount, HWND timeShower) :
		m_MaxX(maxX), m_MaxY(maxY), m_MineCount(mineCount), m_TimeShower(timeShower), Timer(1000, [&]() {
		m_Time++;
		char szBuff[1024];
		sprintf_s(szBuff, 1024, "%d", m_Time);
		SetWindowTextA(m_TimeShower, szBuff);
		})
	{
		Log("Mine被实例化一次");
		SetMine();
	};
	~Mine() noexcept;


private:
	const int m_MaxX;
	const int m_MaxY;
	const int m_MineCount;
	const HWND m_TimeShower;
	int m_Time = 0;
	MineMap m_MineMap;
	MineDisplayMap m_DisplayMineMap;
	mutable std::mutex m_DisplayMineMapMutex;
	mutable std::mutex m_MineMapMutex;
	int m_Marked = m_MineCount;
	GameState m_GameState = BeInit;
	

	INLINE void CheckWin()
	{
		int mineCount = 0;
		std::lock_guard lock(m_MineMapMutex);
		for (const auto& [Pos, State] : m_MineMap)
		{
			if (State == BlockHaveMine)
				return;
			if (State == BlockMarkedMine)
				mineCount++;
			if (State == BlockMarked)
				return;
		}
		if (mineCount == m_MineCount) {
			m_GameState = Win;
			return;
		}
		else
			return;
	};
	INLINE GameState ReturnGameState()
	{
		if (m_GameState == BeInit) {
			m_GameState = InGame;
			Timer::Start();
		}
		return m_GameState;
	};
public:
	INLINE int GetMaxX() const { return m_MaxX; };
	INLINE int GetMaxY() const { return m_MaxY; };
	INLINE int GetMaxCount() const { return m_MineCount; };

	INLINE int GetMarked() const { return m_Marked; };
	INLINE void SubMarked() { m_Marked++; };
	INLINE void AddMarked() { m_Marked--; };

	INLINE GameState GetGameState() 
	{ 
		CheckWin();
		if (m_GameState == Win || m_GameState == Lose)
			Timer::Stop();
		return m_GameState;
	}
	pair<bool, MinePos> GetBeClickedMine(const pair<int, int>& clickPoint);
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

	INLINE GameState ResClickBlock(const pair<int, int>& pos) 
	{
		
		auto blockState = GetBlockStateByPos(pos.first, pos.second);

		if (m_GameState == BeInit) {
			if (blockState == BlockHaveMine) {
				ChangeMine();
				HandleClickBlock(pos);
				return ReturnGameState();
			}
			else {
				HandleClickBlock(pos);
				return ReturnGameState();
			}
		}
		
		if (blockState == BlockHaveMine) {
			m_GameState = Lose;
			return ReturnGameState();
		}
		else {
			HandleClickBlock(pos);
		}

	} 
};

