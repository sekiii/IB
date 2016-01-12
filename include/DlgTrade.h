#pragma once
#include "EClientSocket.h"
#include "EWrapper.h"
#include "resource.h "
#include "Contract.h"
#include "Order.h"
#include "afxcmn.h"
#include "afxwin.h"




// CDlgTrade 对话框
//Declaration of IB's API Eclient class
class EClient;

class CDlgTrade : public CDialogEx, public EWrapper
{
	DECLARE_DYNAMIC(CDlgTrade)

public:
	CDlgTrade(/*EClient *my,*/ CWnd* pParent = NULL);   // 标准构造函数
	BOOL OnInitDialog();
	~CDlgTrade();
	void CDlgTrade::tickPrice(TickerId tickerId, TickType field, double price, int canAutoExecute);
	void CDlgTrade::orderStatus(OrderId orderId, const CString &status, int filled, int remaining,
		double avgFillPrice, int permId, int parentId, double lastFillPrice,
		int clientId, const CString& whyHeld);
	void CDlgTrade::error(const int id, const int errorCode, const IBString errorString);
	//Contract& getContract() { return *m_contract; }
// 对话框数据
	enum { IDD = IDD_TRADE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_tradelist;
	//std::auto_ptr<Contract>  m_contract;

	//合约变量
	//long	m_id;
	long    m_conId;
	CString	m_symbol;
	IBString m_exchange;
	CString	m_currency;
	CString m_type;
	//long *conId;
	//IBString	*symbol;
	//IBString	*exchange;
	//IBString	*currency;
	//IBString	*type;

	long *conId;// = new long[a];
	CString	*symbol;// = new IBString[a];
	CString	*exchange;
	CString *type;// = new IBString[a];;
	CString	*currency;// = new IBString[a];;

	int *multi;// = new int[a];
	int *amount;// = new int[a];
	int *count;// = new int[a];
	EClient *trade;//=NULL;

	
	//操作变量
	int m_multi;
	int m_amount;
	int m_count;
	int thresh_buy;
	int thresh_sell;
	long m_orderid;
	//int *multi;
	//int *amount;
	//int *count;
	//bool buy; 
	//bool sell; 
	//bool stop;

	//记录文件
	CStdioFile file;

private:
	//void updateContract();
public:
	
	//afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CListBox m_info;
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton9();
	afx_msg void OnNMClickListview(NMHDR *pNMHDR, LRESULT *pResult);

};
