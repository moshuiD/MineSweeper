#include "framework.h"
#include "MineSweeper.h"
#include "MineSweeperDlg.h"
#include "afxdialogex.h"
#include <thread>
#include "Log.hpp"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMineSweeperDlg 对话框
CMineSweeperDlg::CMineSweeperDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MINESWEEPER_DIALOG, pParent)
{
#ifdef DEBUG
	AllocConsole();
	FILE* f;
	freopen_s(&f, "CONOUT$", "w", stdout);
#endif
	m_Mine = unique_ptr<Mine>(new Mine(9, 9, 10));
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_UnusedBlock.LoadBitmapW(IDB_BLOCK1);
	std::thread t([this] {UpdateThread(); });
	t.detach();
}

CMineSweeperDlg::~CMineSweeperDlg()
{
	m_UnusedBlock.DeleteObject();
}

void CMineSweeperDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMineSweeperDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(IDM_SETTING_CUSTOM, &CMineSweeperDlg::OnSettingCustom)
	ON_COMMAND(IDM_SETTING_EASY, &CMineSweeperDlg::OnSettingEasy)
	ON_COMMAND(IDM_SETTING_HARD, &CMineSweeperDlg::OnSettingHard)
	ON_COMMAND(IDM_SETTING_MID, &CMineSweeperDlg::OnSettingMid)
END_MESSAGE_MAP()


// CMineSweeperDlg 消息处理程序

BOOL CMineSweeperDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//CMenu* pSysMenu = GetSystemMenu(FALSE);
	//if (pSysMenu != nullptr)
	//{
	//	BOOL bNameValid;
	//	CString strAboutMenu;
	//	bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
	//	ASSERT(bNameValid);
	//	if (!strAboutMenu.IsEmpty())
	//	{
	//		pSysMenu->AppendMenu(MF_SEPARATOR);
	//		//pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
	//	}
	//}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMineSweeperDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	CDialogEx::OnSysCommand(nID, lParam);
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMineSweeperDlg::OnPaint()
{
	Log("绘制一次");
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		AutoChangeControlSize();

		RECT BombAreaRect;
		::GetWindowRect(::GetDlgItem(m_hWnd, IDC_BOMBAREA), &BombAreaRect);
		ScreenToClient(&BombAreaRect);

		CPaintDC dc(this);
		CDC memDC;
		memDC.CreateCompatibleDC(&dc);
		CBitmap* pOldBmp = memDC.SelectObject(&m_UnusedBlock);

		CRect rect(0, 0, bitPicSize, bitPicSize);
		for (int i = 0; i < m_Mine->GetMaxX(); i++)
		{
			for (int j = 0; j < m_Mine->GetMaxY(); j++)
			{
				dc.BitBlt(BombAreaRect.left + 2 + i * bitPicSize, BombAreaRect.top + 2 + j * bitPicSize, bitPicSize, bitPicSize, &memDC, rect.left, rect.top, SRCCOPY);
			}
		}

		memDC.SelectObject(pOldBmp); // 恢复先前选择的对象
		memDC.DeleteDC(); // 删除内存设备上下文

		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMineSweeperDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMineSweeperDlg::UpdateThread()
{
	Log("thread start");
	while (true)
	{
		if (GetAsyncKeyState(VK_F2) & 1) {
			SendMessage(WM_PAINT);
		}
		Sleep(1);
	}
}

void CMineSweeperDlg::AutoChangeControlSize()
{
	auto h = ::GetDlgItem(m_hWnd, IDC_BOMBAREA);
	RECT BombAreaRect;
	::GetWindowRect(h, &BombAreaRect);
	ScreenToClient(&BombAreaRect);
	::SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, (BombAreaRect.left + 2) * 5 + m_Mine->GetMaxX() * bitPicSize, (m_Mine->GetMaxY() * bitPicSize) + BombAreaRect.top + 75, SWP_NOZORDER | SWP_NOMOVE);
	auto bombArea = GetDlgItem(IDC_BOMBAREA);
	bombArea->MoveWindow(BombAreaRect.left, BombAreaRect.top, (BombAreaRect.left + 2) + m_Mine->GetMaxX() * bitPicSize, m_Mine->GetMaxY() * bitPicSize);

	auto headerArea = GetDlgItem(IDC_HEADER);
	RECT headerAreaRect;
	headerArea->GetWindowRect(&headerAreaRect);
	ScreenToClient(&headerAreaRect);
	headerArea->MoveWindow(headerAreaRect.left, headerAreaRect.top, headerAreaRect.left + m_Mine->GetMaxX() * bitPicSize + 4, headerAreaRect.bottom - headerAreaRect.top);

}



void CMineSweeperDlg::OnSettingCustom()
{
	
}


void CMineSweeperDlg::OnSettingEasy()
{
	m_Mine = unique_ptr<Mine>(new Mine(9, 9, 10));
	SendMessage(WM_PAINT);
}

void CMineSweeperDlg::OnSettingMid()
{
	m_Mine = unique_ptr<Mine>(new Mine(16, 16, 40));
	SendMessage(WM_PAINT);
}

void CMineSweeperDlg::OnSettingHard()
{
	m_Mine = unique_ptr<Mine>(new Mine(32, 16, 99));
	SendMessage(WM_PAINT);
}


