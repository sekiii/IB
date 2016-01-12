// IBMFCDlg.h : 头文件
//

#pragma once
#include "EWrapper.h"
#include "HScrollListBox.h"
#include "Contract.h"
#include "Order.h"
#include "DlgReqdata.h"
#include "afxcmn.h"

//Declaration of IB's API Eclient class
class EClient;

// CIBMFCDlg dialog
class CIBMFCDlg : public CDialogEx, public EWrapper
{
// Construction
public:
	CIBMFCDlg(CWnd* pParent = NULL);  // standard constructor

// Dialog data
	enum { IDD = IDD_IBMFC_DIALOG };
	

protected:
	virtual void DoDataExchange(CDataExchange* pDX);   // DDX/DDV support

	// This member variable will be our entry point to IB's API
	EClient *m_pClient;

	// Through this variable we will print the incoming data to the UI
	CHScrollListBox m_server;
	CHScrollListBox m_msg;
	CHScrollListBox m_data;

	// Through this we will request some market data
	Contract m_contract;

protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	void CIBMFCDlg::nextValidId(OrderId orderId);
	void CIBMFCDlg::tickPrice(TickerId tickerId, TickType field, double price, int canAutoExecute); 
	//void CIBMFCDlg::tickSize(TickerId tickerId, TickType tickType, int size);
	void CIBMFCDlg::error(const int id, const int errorCode, const IBString errorString);
    afx_msg void OnLbnSelchangeList2();
    afx_msg void OnReqMrkData();

private:
	//std::auto_ptr<CDlgReqdata> m_dlgReqdata;
	TagValueListSPtr m_mktDataOptions;
	

public:
	afx_msg void OnBnClickedClear();
	afx_msg void OnBnClickedStopreqdata();
	CListCtrl m_list;
	afx_msg void OnBnClickedTrade();
	afx_msg void OnBnClickedOk2();
	
};

