//Í·ÎÄ¼þ

#if !defined(AFX_DLGORDER_H__115EE565_0BA0_11D6_B0ED_00B0D074179C__INCLUDED_)
#define AFX_DLGORDER_H__115EE565_0BA0_11D6_B0ED_00B0D074179C__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <memory>

#include "Contract.h"
#include "Order.h"
#include   "resource.h "

class CIBMFCDlg;

class CDlgReqdata : public CDialog
{
	// Construction
public:
	/*enum DialogType {
		ORDER, CANCEL_ORDER, REQ_MKT_DATA, CANCEL_MKT_DATA, REQ_CONTRACT_DETAILS,
		REQ_MKT_DEPTH, CANCEL_MKT_DEPTH, REQ_HISTORICAL_DATA, EXERCISE_OPTIONS,
		CANCEL_HISTORICAL_DATA, REQ_REAL_TIME_BARS, CANCEL_REAL_TIME_BARS, CALC_IMPL_VOL,
		CALC_OPTION_PRICE, CANCEL_CALC_IMPL_VOL, CANCEL_CALC_OPTION_PRICE, REQ_MARKET_DATA_TYPE,
		REQ_FUNDAMENTAL_DATA, CANCEL_FUNDAMENTAL_DATA
	};*/

	CDlgReqdata(CWnd* pParent = NULL);   // standard constructor
	~CDlgReqdata();

	void init(CIBMFCDlg* dlg, CString dlgTitle, TagValueListSPtr* options);

	Contract& getContract() { return *m_contract; }

// Dialog Data
	//{{AFX_DATA(CDlgOrder)
	enum { IDD = IDD_REQDATA };
	long	m_id;
	long    m_conId;
	CString	m_symbol;
	CString	m_expiry;
	CString	m_right;
	CString	m_multiplier;
	CString	m_exchange;
	CString	m_primaryExchange;
	CString	m_currency;
	BOOL    m_includeExpired;
	CString	m_secIdType;
	CString	m_secId;
	double	m_strike;
	CString	m_secType;
	CString	m_localSymbol;
	CString	m_tradingClass;
	CString m_genericTicks;
	bool    m_snapshotMktData;
	CComboBox m_marketDataTypeCombo;
	//int m_marketDataType;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	std::auto_ptr<Contract>  m_contract;
	//Contract m_contract;
	CIBMFCDlg*              m_pDlg;
	CString                 m_dlgTitle;
	TagValueListSPtr*		m_options;

private:

	void updateContract();

public:
	DECLARE_MESSAGE_MAP()
	//afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
#endif