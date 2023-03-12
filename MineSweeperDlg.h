#pragma once
#include <memory>
#include "Mine.h"

class CMineSweeperDlg : public CDialogEx
{
	template<class T>
	using shared_ptr = std::shared_ptr<T>;
	template<class T>
	using unique_ptr = std::unique_ptr<T>;
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
	void AutoChangeControlSize();
	unique_ptr<Mine> m_Mine;
public:
	afx_msg void OnSettingCustom();
	afx_msg void OnSettingEasy();
	afx_msg void OnSettingHard();
	afx_msg void OnSettingMid();
private:
	CButton m_MainButton;
	BOOL PreTranslateMessage(MSG* pMsg);
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};
