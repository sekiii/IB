
// IBMFCDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "IBMFC.h"
#include "IBMFCDlg.h"
#include "afxdialogex.h"
#include "EClientSocket.h"
#include "Dlgconnect.h"
#include "DlgReqdata.h"
#include "DlgTrade.h"
#include "resource.h"
#include "Order.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CIBMFCDlg �Ի���
CString getField(TickType tickType) {
	switch (tickType)
	{
	case BID_SIZE:	                    return "bidSize";
	case BID:		                    return "bidPrice";
	case ASK:		                    return "askPrice";
	case ASK_SIZE:	                    return "askSize";
	case LAST:		                    return "lastPrice";
	case LAST_SIZE:	                    return "lastSize";
	case HIGH:		                    return "high";
	case LOW:		                    return "low";
	case VOLUME:	                    return "volume";
	case CLOSE:		                    return "close";
	case BID_OPTION_COMPUTATION:		return "bidOptComp";
	case ASK_OPTION_COMPUTATION:		return "askOptComp";
	case LAST_OPTION_COMPUTATION:		return "lastOptComp";
	case MODEL_OPTION:					return "optionModel";
	case OPEN:                          return "open";
	case LOW_13_WEEK:                   return "13WeekLow";
	case HIGH_13_WEEK:                  return "13WeekHigh";
	case LOW_26_WEEK:                   return "26WeekLow";
	case HIGH_26_WEEK:                  return "26WeekHigh";
	case LOW_52_WEEK:                   return "52WeekLow";
	case HIGH_52_WEEK:                  return "52WeekHigh";
	case AVG_VOLUME:                    return "AvgVolume";
	case OPEN_INTEREST:                 return "OpenInterest";
	case OPTION_HISTORICAL_VOL:         return "OptionHistoricalVolatility";
	case OPTION_IMPLIED_VOL:            return "OptionImpliedVolatility";
	case OPTION_BID_EXCH:               return "OptionBidExchStr";
	case OPTION_ASK_EXCH:               return "OptionAskExchStr";
	case OPTION_CALL_OPEN_INTEREST:     return "OptionCallOpenInterest";
	case OPTION_PUT_OPEN_INTEREST:      return "OptionPutOpenInterest";
	case OPTION_CALL_VOLUME:            return "OptionCallVolume";
	case OPTION_PUT_VOLUME:             return "OptionPutVolume";
	case INDEX_FUTURE_PREMIUM:          return "IndexFuturePremium";
	case BID_EXCH:                      return "bidExch";
	case ASK_EXCH:                      return "askExch";
	case AUCTION_VOLUME:                return "auctionVolume";
	case AUCTION_PRICE:                 return "auctionPrice";
	case AUCTION_IMBALANCE:             return "auctionImbalance";
	case MARK_PRICE:                    return "markPrice";
	case BID_EFP_COMPUTATION:           return "bidEFP";
	case ASK_EFP_COMPUTATION:           return "askEFP";
	case LAST_EFP_COMPUTATION:          return "lastEFP";
	case OPEN_EFP_COMPUTATION:          return "openEFP";
	case HIGH_EFP_COMPUTATION:          return "highEFP";
	case LOW_EFP_COMPUTATION:           return "lowEFP";
	case CLOSE_EFP_COMPUTATION:         return "closeEFP";
	case LAST_TIMESTAMP:                return "lastTimestamp";
	case SHORTABLE:                     return "shortable";
	case FUNDAMENTAL_RATIOS:            return "fundamentals";
	case RT_VOLUME:                     return "RTVolume";
	case HALTED:                        return "halted";
	case BID_YIELD:                     return "bidYield";
	case ASK_YIELD:                     return "askYield";
	case LAST_YIELD:                    return "lastYield";
	case CUST_OPTION_COMPUTATION:       return "custOptComp";
	case TRADE_COUNT:                   return "trades";
	case TRADE_RATE:                    return "trades/min";
	case VOLUME_RATE:                   return "volume/min";
	case LAST_RTH_TRADE:                return "lastRTHTrade";
	default:                            return "unknown";
	}
}



CIBMFCDlg::CIBMFCDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CIBMFCDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	// Initialize IB's API client
	m_pClient = new EClientSocket(this);

	// Initialize our contract
	m_contract.symbol = "EUR";
	m_contract.secType = "CASH";
	m_contract.currency = "GBP";
	m_contract.exchange = "IDEALPRO";

}

void CIBMFCDlg::error(const int id, const int errorCode, const IBString errorString)
{
	CString displayString;
	//displayString.Format("Error: tickerID: %d, Errorcode: %d2", id, errorCode);
	//m_server.InsertString(-1,displayString);
	displayString.Format(errorString);
	m_server.InsertString(-1, displayString);
}

void CIBMFCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LIST2, m_server);
	DDX_Control(pDX, IDC_LIST1, m_msg);
	//DDX_Control(pDX, IDC_LIST3, m_data);
	CString temp;
	//m_data.InsertString(-1, temp.GetBuffer(5));
	DDX_Control(pDX, IDC_LISTVIEW, m_list);
}

BEGIN_MESSAGE_MAP(CIBMFCDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CIBMFCDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &CIBMFCDlg::OnReqMrkData)
	ON_BN_CLICKED(IDC_CLEAR, &CIBMFCDlg::OnBnClickedClear)
	ON_BN_CLICKED(IDC_STOPREQDATA, &CIBMFCDlg::OnBnClickedStopreqdata)
	ON_BN_CLICKED(IDC_TRADE, &CIBMFCDlg::OnBnClickedTrade)
	ON_BN_CLICKED(IDOK2, &CIBMFCDlg::OnBnClickedOk2)
END_MESSAGE_MAP()


// CIBMFCDlg ��Ϣ�������

BOOL CIBMFCDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	//����ʼ��
	DWORD dwStyle = m_list.GetExtendedStyle();
	dwStyle |= LVS_EX_GRIDLINES;
	m_list.SetExtendedStyle(dwStyle);

	m_list.InsertColumn(1, "��Լ��", LVCFMT_LEFT, 150);
	m_list.InsertColumn(0, "���", LVCFMT_LEFT, 60);
	m_list.InsertColumn(2, "��һ��", LVCFMT_LEFT, 150);
	m_list.InsertColumn(3, "��һ��", LVCFMT_LEFT, 150);
	//m_list.InsertColumn(4, "size", LVCFMT_LEFT, 100);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CIBMFCDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CIBMFCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);

}

//����
void CIBMFCDlg::OnBnClickedOk()
{
	//CString str;
	m_msg.InsertString(-1,"connecting");

	// IMPORTANT: TWS must be running, and the "Enable Excel Integration"
	// checkbox on the "Settings" menu must be checked!

	// get connection parameters
	CDlgConnect dlg;
	if( dlg.DoModal() == IDCANCEL) {
		return;
	}

	// connect to TWS
	{
		CString displayString;
		displayString.Format( "Connecting to Tws using clientId %d ...", dlg.m_clientId);
		m_msg.InsertString(-1,displayString);
		
	}

	m_pClient->eConnect( dlg.m_ipAddress, dlg.m_port, dlg.m_clientId, /* extraAuth */ false);

	if( m_pClient->serverVersion() > 0)	{
		CString displayString;
		displayString.Format( "Connected to Tws server version %d at %s.",
			m_pClient->serverVersion(), m_pClient->TwsConnectionTime());
		m_msg.InsertString(-1,"Connection successful!");
		m_msg.InsertString(-1,displayString);
		
	}

	
	
}

//�Ͽ�����
void CIBMFCDlg::OnBnClickedOk2()
{
	m_pClient->eDisconnect();
	m_msg.InsertString(-1, "Disconnected.");
}

void CIBMFCDlg::nextValidId(OrderId orderId)
{
	CString str;
	str.Format("connection successful!", orderId);
	//int i = m_server.AddString(str);
	//int top = i-5<0?0:i-5;
	//m_server.SetTopIndex(top);

	// request data
	//m_pClient->reqMktData(1, m_contract, "", false, m_mktDataOptions);
}


void CIBMFCDlg::tickPrice(TickerId tickerId, TickType field, double price, int canAutoExecute)
{
	CString str, temp;
	str.Format("%f", price);
	temp.Format("%i", tickerId);
	//search
	LVFINDINFO info;
	int nIndex;
	info.flags = LVFI_PARTIAL | LVFI_STRING;
	info.psz = temp;
	nIndex = m_list.FindItem(&info);
	//insert
	if (field==1)
		m_list.SetItemText(nIndex, 2, str);
	if (field==2)
		m_list.SetItemText(nIndex, 3, str);
}

/*
void CIBMFCDlg::tickSize(TickerId tickerId, TickType tickType, int size)
{
	CString str, temp;
	str.Format("%i", size);
	temp.Format("%i", tickerId);
	//search
	LVFINDINFO info;
	int nIndex;
	info.flags = LVFI_PARTIAL | LVFI_STRING;
	info.psz = temp;
	nIndex = m_list.FindItem(&info);
	//insert
	m_list.SetItemText(nIndex, 4, str);
}
*/	

//��ȡ�г���Ϣ���򿪺�Լ���
void CIBMFCDlg::OnReqMrkData()
{
	CDlgReqdata dlg;

	if (dlg.DoModal() == IDCANCEL){ 
		return;
	}
	//if (dlg.DoModal() == IDOK)
	//{
		m_msg.InsertString(-1,"request...");
		//��ȡ����
		m_pClient->reqMktData(dlg.m_id, dlg.getContract(), dlg.m_genericTicks, dlg.m_snapshotMktData, m_mktDataOptions);
		CString str;
		str.Format("%i", dlg.m_id);
		m_list.InsertItem(dlg.m_id, str);
		//m_list.InsertItem(dlg.m_id,str);
		//search
		LVFINDINFO info;
		int nIndex;
		info.flags = LVFI_PARTIAL | LVFI_STRING;
		info.psz = str;
		nIndex = m_list.FindItem(&info);
		str.Format("%i", dlg.m_conId);
		m_list.SetItemText(nIndex, 1, str);

	//}
	

}

//�����Ϣ��
void CIBMFCDlg::OnBnClickedClear()
{
	//m_data.ResetContent();
	m_server.ResetContent();
	m_msg.ResetContent();
}

//ֹͣ��ȡ�г���Ϣ
void CIBMFCDlg::OnBnClickedStopreqdata()
{
	CDlgReqdata dlg;

	//CWnd *pWnd = GetDlgItem(IDC_EDIT_CONID);
	//pWnd->EnableWindow(FALSE);

	if (dlg.DoModal() == IDCANCEL) {
		return;
	}

	m_pClient->cancelMktData(dlg.m_id);
	m_msg.InsertString(-1, "request cancelled");
	//search
	LVFINDINFO info;
	int nIndex;
	info.flags = LVFI_PARTIAL | LVFI_STRING;
	CString str;
	str.Format("%i", dlg.m_id);
	info.psz = str;
	nIndex = m_list.FindItem(&info);
	//delete
	m_list.DeleteItem(nIndex);
}

//�򿪼۲�����
void CIBMFCDlg::OnBnClickedTrade()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CDlgTrade dlg;
	//dlg.trade = m_pClient;
	//dlg.OnInitDialog();

	/*
	Contract temp;
	Order order;

	CDlgTrade dlg(this);
	int m_id = 0;
	int i = 0;
	int j = 0;


	
	while (dlg.stop == false){
		if (dlg.buy == true){
			m_msg.InsertString(-1, "buy");
			for (i = 0; i < 3; i++){
				if (dlg.conId[i] != NULL){

					temp.conId = dlg.conId[i];
					temp.symbol = dlg.symbol[i];
					temp.exchange = dlg.exchange[i];
					temp.currency = dlg.currency[i];
					temp.secType = dlg.type[i];
					if (dlg.amount[i]>0)
						order.action = _T("BUY");
					else
						order.action = _T("SELL");
					//m_pClient->placeOrder(m_id, temp, order);
					m_msg.InsertString(-1, "buy");
				}
			}
			dlg.buy = false;

		}


		if (dlg.sell == true){
			//m_msg.InsertString(-1, "sell");
			for (i = 0; i < 3; i++){
				if (dlg.conId[i] != NULL){

					temp.conId = dlg.conId[i];
					temp.symbol = dlg.symbol[i];
					temp.exchange = dlg.exchange[i];
					temp.currency = dlg.currency[i];
					temp.secType = dlg.type[i];

					if (dlg.amount[i] < 0)
						order.action = _T("BUY");
					else
						order.action = _T("SELL");
					//m_pClient->placeOrder(m_id, temp, order);
					m_msg.InsertString(-1, "sell");
				}
			}
			dlg.sell = false;

		}
	}
	*/
	if (dlg.DoModal() == IDCANCEL){
		return;
	}
	
	
}


