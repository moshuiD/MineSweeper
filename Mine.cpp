#include "Mine.h"

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


