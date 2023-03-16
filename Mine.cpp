#include "Mine.h"
#include "Data.h"
#include <queue>
#include <set>
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
		std::lock_guard lock(m_MineMapMutex);
		if (m_MineMap[{t_X, t_Y}] != BlockHaveMine) {
			m_MineMap[{t_X, t_Y}] = BlockHaveMine;
			unUsedMine--;
			Log("���� x:%d y:%d", t_X, t_Y);
		}
	} while (unUsedMine);

	Log("Mine��ʼ�����");
}

void Mine::ChangeMine()
{
	bool isSet = false;
	do
	{
		int t_X = rand() % m_MaxX;
		int t_Y = rand() % m_MaxY;
		std::lock_guard lock(m_MineMapMutex);
		if (m_MineMap[{t_X, t_Y}] != BlockHaveMine) {
			m_MineMap[{t_X, t_Y}] = BlockHaveMine;
			isSet = true;
			Log("�������� x:%d y:%d", t_X, t_Y);
		}
	} while (!isSet);
}

void Mine::HandleClickBlock(const std::pair<int, int>& pos)
{
	auto blockState = GetBlockStateByPos(pos.first, pos.second);
	if (blockState - (blockState & 0xE0) == BlockMarked) {
		return;
	}
	SetBlockStateByPos(pos, BlockNoMine);
	auto nearNum = GetNearMineNum(pos);
	if (nearNum == 0) {
		std::queue<MinePos> needChangeBlockList;
		std::set<MinePos> processedBlockSet;
		needChangeBlockList.push(pos);

		do {
			auto& curPos = needChangeBlockList.front();

			needChangeBlockList.pop();
			processedBlockSet.insert(curPos);

			SetBlockStateByPos(curPos, BlockNoMine);
			for (int i = -1; i <= 1; i++) 
			{
				for (int j = -1; j <= 1; j++) 
				{
					if (i == 0 && j == 0) continue;
					MinePos neighborPos = { curPos.first + i, curPos.second + j };
					if (neighborPos.first < 0 || neighborPos.first > m_MaxX) continue;
					if (neighborPos.second < 0 || neighborPos.second > m_MaxY) continue;
					if (processedBlockSet.count(neighborPos) > 0) continue;

					auto nums = GetNearMineNum(neighborPos);
					if (nums == 0) {
						needChangeBlockList.push(neighborPos);
					}
					else {
						SetBlockStateByPos(neighborPos, [&](std::pair<const Mine::MinePos, Mine::BlockState>& v, const Mine::BlockState state) {
							v.second = (Mine::BlockState)(BlockNoMine | nums);
						});
					}
				}
			}
		} while (!needChangeBlockList.empty());
	}
	else {
		SetBlockStateByPos(pos, [&](std::pair<const Mine::MinePos, Mine::BlockState>& v, const Mine::BlockState state) {
			v.second = (Mine::BlockState)(state | nearNum);
		});
	}
	Log("λ��: %d %d,��Χ�� %d ������", pos.first, pos.second, nearNum);
}

int Mine::GetNearMineNum(const std::pair<int, int>& pos)
{
	int count = 0;
	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			if (i == 0 && j == 0) continue;
			auto state = GetBlockStateByPos(pos.first + i, pos.second + j);
			if (state == BlockHaveMine || state == BlockMarkedMine)
			{
				count++;
			}
		}
	}
	return count;

}

Mine::~Mine()
{
	Timer::Stop();
	SetWindowTextA(m_TimeShower, "000");
	Log("Mine���ͷ�һ��");
}

Mine::pair<bool, Mine::MinePos> Mine::GetBeClickedMine(const pair<int, int>& clickPoint)
{
	using Data::bitPicSize;
	std::unique_lock lock(m_DisplayMineMapMutex, std::defer_lock);
	lock.lock();
	auto iter = std::find_if(m_DisplayMineMap.begin(), m_DisplayMineMap.end(), [&](const pair<pair<int, int>, MinePos>& v1)->bool {
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
	else {
		return { false,{-1,-1} };
	}
}
