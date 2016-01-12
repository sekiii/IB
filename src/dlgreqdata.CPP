#include "stdafx.h"
#include "DlgReqdata.h"

BEGIN_MESSAGE_MAP(CDlgReqdata, CDialog)
END_MESSAGE_MAP()

// Remember the last client Id
static Contract s_contract;

CDlgReqdata::CDlgReqdata(CWnd* pParent /*=NULL*/)
: CDialog(CDlgReqdata::IDD, pParent)
, m_contract(new Contract)
{

	//{{AFX_DATA_INIT(CDlgOrder)
	m_id = 0;
	m_conId =177232556;
	//m_conId = 15016059;
	m_symbol = _T("XINA50");
	m_expiry = _T("");
	m_right = _T("");
	m_multiplier = _T("");
	m_exchange = _T("SGX");
	m_primaryExchange = _T("");
	m_currency = _T("USD");
	m_includeExpired = 0;
	m_strike = 0.0f;
	m_secType = _T("FUT");
	m_localSymbol = _T("");
	m_tradingClass = _T("");
	m_secIdType = _T("");
	m_secId = _T("");
	m_genericTicks = _T("100,101,104,105,106,107,165,221,225,233,236,258,293,294,295,318");
	m_snapshotMktData = false;
	//m_conId = s_contract.conId;
	//m_symbol = s_contract.symbol;
	//m_currency = s_contract.currency;
	//m_exchange = s_contract.exchange;
	//m_secType = s_contract.secType;

	CTime theTime = CTime::GetCurrentTime();
	CString str = theTime.Format("%Y%m%d %H:%M:%S");

	updateContract();
	//m_marketDataType = REALTIME; // This is the default market data type selection
	//}}AFX_DATA_INIT
}

CDlgReqdata::~CDlgReqdata()
{
}

void CDlgReqdata::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgOrder)

	// Primary Id
	DDX_Text(pDX, IDC_EDIT_ID, m_id);

	// Contract description fields
	DDX_Text(pDX, IDC_EDIT_CONID, m_conId);
	DDX_Text(pDX, IDC_EDIT_SYMBOL, m_symbol);
	DDX_Text(pDX, IDC_EDIT_SEC_TYPE, m_secType);
	DDX_Text(pDX, IDC_EDIT_EXPIRY, m_expiry);
	DDX_Text(pDX, IDC_EDIT_STRIKE, m_strike);
	DDX_Text(pDX, IDC_EDIT_RIGHT, m_right);
	DDX_Text(pDX, IDC_EDIT_MULT, m_multiplier);
	DDX_Text(pDX, IDC_EDIT_EXCH, m_exchange);
	DDX_Text(pDX, IDC_EDIT_PRIM_EXCH, m_primaryExchange);
	DDX_Text(pDX, IDC_EDIT_CURRENCY, m_currency);
	DDX_Text(pDX, IDC_EDIT_LOCAL_SYMBOL, m_localSymbol);
	DDX_Text(pDX, IDC_EDIT_TRADING_CLASS, m_tradingClass);
	DDX_Text(pDX, IDC_EDIT_IS_EXPIRED, m_includeExpired);
	DDX_Text(pDX, IDC_EDIT_SEC_ID_TYPE, m_secIdType);
	DDX_Text(pDX, IDC_EDIT_SEC_ID, m_secId);
	// Market Data
	DDX_Text(pDX, IDC_EDIT_GENERETIC_TICKS, m_genericTicks);

	int snapshotMktDataState = (m_snapshotMktData ? BST_CHECKED : BST_UNCHECKED);
	DDX_Check(pDX, IDC_CHECK_SNAPSHOT_MKT_DATA, snapshotMktDataState);
	m_snapshotMktData = (snapshotMktDataState == BST_CHECKED);


	//}}AFX_DATA_MAP
}

void CDlgReqdata::init(CIBMFCDlg* dlg, CString dlgTitle, TagValueListSPtr* options)
{
	m_pDlg = dlg;
	m_dlgTitle = dlgTitle;
	m_options = options;
}


void CDlgReqdata::OnBnClickedOk()
{
	
	// TODO: Add extra validation here
	//UpdateData(TRUE);
	s_contract.conId = m_conId;
	s_contract.symbol = m_symbol;
	s_contract.currency = m_currency;
	s_contract.exchange = m_exchange;
	s_contract.secType = m_secType;
	updateContract();
	CDialog::OnOK();
}


void CDlgReqdata::OnBnClickedCancel()
{
	// TODO: Add extra cleanup here
	UpdateData(TRUE);
	s_contract.conId = m_conId;
	s_contract.symbol = m_symbol;
	s_contract.currency = m_currency;
	s_contract.exchange = m_exchange;
	s_contract.secType = m_secType;

	CDialog::OnCancel();
}

void CDlgReqdata::updateContract()
{
	/*
	ASSERT(m_contract.get());

	Contract* contract = m_contract.get();

#define PUT_PROP(Prop) \
	contract->Prop = m_##Prop;

	PUT_PROP(conId);
	PUT_PROP(symbol);
	PUT_PROP(secType);
	PUT_PROP(expiry);
	PUT_PROP(strike);
	PUT_PROP(right);
	PUT_PROP(multiplier);
	PUT_PROP(exchange);
	PUT_PROP(primaryExchange);
	PUT_PROP(currency);
	PUT_PROP(localSymbol);
	PUT_PROP(tradingClass);
	PUT_PROP(includeExpired);
	PUT_PROP(secIdType);
	PUT_PROP(secId);

#undef PUT_PROP
	*/
	m_contract->conId = m_conId;
	m_contract->symbol = m_symbol;
	m_contract->secType = m_secType;
	m_contract->exchange = m_exchange;
	m_contract->currency = m_currency;
	m_contract->strike = m_strike;
	m_contract->expiry = m_expiry;
	m_contract->multiplier = m_multiplier;
	m_contract->tradingClass = m_tradingClass;
	m_contract->primaryExchange = m_primaryExchange;
	m_contract->localSymbol = m_localSymbol;
	m_contract->includeExpired = m_includeExpired;
	m_contract->secIdType = m_secIdType;
	m_contract->secId = m_secId;


}