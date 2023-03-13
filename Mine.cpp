#include "Mine.h"
#include "Data.h"
void Mine::SetMine()
{
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
		}
	} while (unUsedMine);

	Log("Mine初始化完成");
}

Mine::~Mine()
{
}

Mine::pair<bool, Mine::MinePos> Mine::GetBeClickedMine(const pair<int, int>& clickPoint) const
{
	std::unique_lock lock(m_DisplayMineMapMutex,std::defer_lock);
	using Data::bitPicSize;
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
		return { true,iter->second };
	}
	else{
		return { false,{-1,-1} };
	}
}



