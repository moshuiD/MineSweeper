
// MineSweeperDlg.h: 头文件
//

#pragma once


// CMineSweeperDlg 对话框
class CMineSweeperDlg : public CDialogEx
{
// 构造
public:
	CMineSweeperDlg(CWnd* pParent = nullptr);	// 标准构造函数
	~CMineSweeperDlg();
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MINESWEEPER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	CBitmap m_UnusedBlock;
	CBitmap m_UsedBlock;
	CBitmap m_Bomber;
	CBitmap m_Flag;
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	void UpdateThread();
	const int bitPicSize = 28;
public:
	afx_msg void OnSettingCustom();
	afx_msg void OnSettingEasy();
	afx_msg void OnSettingHard();
	afx_msg void OnSettingMid();
};
