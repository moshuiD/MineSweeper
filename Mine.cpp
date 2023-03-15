#include "Mine.h"
#include "Data.h"
void Mine::SetMine()
{
	srand(time(nullptr));
	for (int i = 0; i < m_MaxX; i++)
	{
		for (int j = 0; j < m_MaxY; j++)
		{
			m_MineMap[{i, j}] = BlockBeInited;
		}
	}

	int unUsedMine = m_MineCount;
	do
	{
		int t_X = rand() % m_MaxX;
		int t_Y = rand() % m_MaxY;
		if (m_MineMap[{t_X, t_Y}] != BlockHaveMine) {
			m_MineMap[{t_X, t_Y}] = BlockHaveMine;
			unUsedMine--;
			Log("雷在 x:%d y:%d", t_X, t_Y);
		}
	} while (unUsedMine);

	Log("Mine初始化完成");
}

Mine::~Mine()
{
	Timer::Stop();
	SetWindowTextA(m_TimeShower, "000");
	Log("Mine被释放一次");
}

Mine::pair<bool, Mine::MinePos> Mine::GetBeClickedMine(const pair<int, int>& clickPoint)
{
	using Data::bitPicSize;
	std::unique_lock lock(m_DisplayMineMapMutex,std::defer_lock);
	lock.lock();
	auto iter = std::find_if(m_DisplayMineMap.begin(), m_DisplayMineMap.end(), [&](const pair<pair<int,int>,MinePos>& v1)->bool {
		if (clickPoint.first > v1.first.first && clickPoint.first < (v1.first.first + bitPicSize)) {
			if (clickPoint.second > v1.first.second && clickPoint.second < (v1.first.second + bitPicSize)) {
				return true;
			}
		}
		return  false;
	});
	lock.unlock();
	if (iter != m_DisplayMineMap.end()) {
		if (m_GameState==BeInit) {
			m_GameState = InGame;
			Timer::Start();
		}
		return { true,iter->second };
	}
	else{
		return { false,{-1,-1} };
	}
}
