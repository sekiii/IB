// DlgTrade.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "IBMFC.h"
#include "DlgTrade.h"
#include "afxdialogex.h"
#include "resource.h"


static const int a = 3;

static long s_orderid;

// CDlgTrade �Ի���

IMPLEMENT_DYNAMIC(CDlgTrade, CDialogEx)

CDlgTrade::CDlgTrade(/*EClient *my,*/ CWnd* pParent /*=NULL*/)
: CDialogEx(CDlgTrade::IDD, pParent){
	
	//A50 һ��
	m_conId =  209677133 ;
	m_symbol = _T("XINA50");
	m_exchange = _T("SGX");
	m_currency = _T("USD");
	m_type = _T("FUT");
	
	//A50 ����
	/*
	m_conId =  212549858 ;
	m_symbol = _T("XINA50");
	m_exchange = _T("SGX");
	m_currency = _T("USD");
	m_type = _T("FUT");
	*/
	/*
	m_conId =  137937575 ;
	m_symbol = _T("AU");
	m_exchange = _T("SGX");
	m_currency = _T("USD");
	m_type = _T("IND");  
	*/
	m_multi = 1;
	m_amount = 1;
	m_count = 1;//����
	thresh_buy=-9999;
	thresh_sell=9999;
	//buy = false;
	//sell = false;
	//stop = false;
	conId = new long[a];
	symbol = new IBString[a];
	exchange = new IBString[a];
	currency = new IBString[a];
	type = new IBString[a];
	multi = new int[a];
	amount = new int[a];
	count = new int[a];

	trade = new EClientSocket(this);
	//trade = my;
	trade->eConnect("", 7497, 0, /* extraAuth */ false);

	//read order id
	LPCTSTR filename="info.txt"; //record of order id
	if(file.Open(filename,CFile::modeRead)){
		CString str;
		file.ReadString(str);
		m_orderid=atol(str);
		file.Close();
	}

	//read contract record
	LPCTSTR filename2="contract.txt"; //record of order id
	if(file.Open(filename2,CFile::modeRead)){
		CString str;
		int i=0;

		while(i<a&&file.ReadString(str)){
			conId[i]=atol(str);
			file.ReadString(str);
			symbol[i]=str;
			file.ReadString(str);
			exchange[i]=str;
			file.ReadString(str);
			currency[i]=str;
			file.ReadString(str);
			type[i]=str;
			file.ReadString(str);
			multi[i]=atol(str);
			file.ReadString(str);
			amount[i]=atol(str);
			//file.ReadString(str);
			//count[i]=atol(str);

			i++;
		}
		file.Close();
	}

}

CDlgTrade::~CDlgTrade()
{

	//д��order id��¼
	LPCTSTR filename="info.txt";
	if(file.Open(filename, CFile::typeText|CFile::modeReadWrite)){
		CString str;
		str.Format("%d",m_orderid);
		file.WriteString(str);
		file.Close();
	}
	
	//д���Լ��¼�������
	LPCTSTR filename2="contract.txt";
	if(file.Open(filename2, CFile::typeText|CFile::modeCreate|CFile::modeReadWrite)){
		CString str;
		int i=0;

		//UpdateData();

		while(i<a&&conId[i]>0){
			str.Format("%d\n",conId[i]);
			file.WriteString(str);
			str=symbol[i]+"\n";
			file.WriteString(str);
			str=exchange[i]+"\n";
			file.WriteString(str);
			str=currency[i]+"\n";
			file.WriteString(str);
			str=type[i]+"\n";
			file.WriteString(str);
			str.Format("%d\n",multi[i]);
			file.WriteString(str);
			str.Format("%d\n",amount[i]);
			file.WriteString(str);
			//file.WriteString(str);
			//str.Format("%d\n",count[i]);
			
			i++;
		}
			file.Close();
	}

	trade->eDisconnect();
	delete []trade;
	
	delete [] conId;
	delete [] symbol;
	delete [] exchange;
	delete [] currency;
	delete [] type;
	delete [] multi;
	delete [] amount;
	delete [] count;
	
	
}


BOOL CDlgTrade::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	//SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	//SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	//����ʼ��
	m_tradelist.SetExtendedStyle(m_tradelist.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	CRect rect;  
	m_tradelist.InsertColumn(0, "��Լ", LVCFMT_CENTER);
	m_tradelist.InsertColumn(1, "ϵ��", LVCFMT_LEFT);
	m_tradelist.InsertColumn(2, "����", LVCFMT_LEFT);
	m_tradelist.InsertColumn(3, "��һ��", LVCFMT_LEFT);
	m_tradelist.InsertColumn(4, "��һ��", LVCFMT_LEFT);
	m_tradelist.GetClientRect(rect); //��õ�ǰ�ͻ�����Ϣ  
	m_tradelist.SetColumnWidth(0, rect.Width() /4); //�����еĿ��  
	m_tradelist.SetColumnWidth(1, rect.Width() /8); 
	m_tradelist.SetColumnWidth(2, rect.Width() /8);
	m_tradelist.SetColumnWidth(3, rect.Width() /4);
	m_tradelist.SetColumnWidth(4, rect.Width() /4);

	//��Ӽ�¼��Լ
	
	Contract temp;
	CString str;
	int i=0, id=0;
	while(conId[i]>0&&i<a){
		str.Format("%i", conId[i]);	
		id = conId[i];

		m_tradelist.InsertItem(conId[i], str);
		str.Format("%i", amount[i]);
		m_tradelist.SetItemText(i, 1, str);
		str.Format("%i", multi[i]);
		m_tradelist.SetItemText(i, 2, str);

		temp.conId = conId[i];
		temp.symbol =symbol[i];
		temp.exchange = exchange[i];
		temp.currency = currency[i];
		temp.secType = type[i];
		TagValueListSPtr m_mktDataOptions;
		CString m_genericTicks = _T("100,101,104,105,106,107,165,221,225,233,236,258,293,294,295,318");
		trade->reqMktData(id, temp, m_genericTicks, false, m_mktDataOptions);//��Լ��ֱ����Ϊ���ĺ�
	
		i++;
	}
	
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}


void CDlgTrade::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_conId);
	DDX_Text(pDX, IDC_EDIT2, m_amount);
	DDX_Text(pDX, IDC_EDIT3, m_multi);
	DDX_Text(pDX, IDC_EDIT4, m_symbol);
	DDX_Text(pDX, IDC_EDIT5, m_exchange);
	DDX_Text(pDX, IDC_EDIT6, m_count);
	//DDX_Text(pDX, IDC_EDIT7, m_id);
	DDX_Text(pDX, IDC_EDIT10, m_type);
	DDX_Text(pDX, IDC_EDIT11, thresh_sell);
	DDX_Text(pDX, IDC_EDIT12, thresh_buy);
	DDX_Control(pDX, IDC_LIST1, m_tradelist);
	DDX_Control(pDX, IDC_LIST2, m_info);
}


BEGIN_MESSAGE_MAP(CDlgTrade, CDialogEx)
	//ON_BN_CLICKED(IDCANCEL, &CDlgTrade::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON2, &CDlgTrade::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CDlgTrade::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CDlgTrade::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CDlgTrade::OnBnClickedButton5)
	//ON_BN_CLICKED(IDOK, &CDlgTrade::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgTrade::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON6, &CDlgTrade::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON9, &CDlgTrade::OnBnClickedButton9)
	ON_NOTIFY(NM_CLICK, IDC_LISTVIEW, &CDlgTrade::OnNMClickListview)
END_MESSAGE_MAP()



//��Ӱ�ť
void CDlgTrade::OnBnClickedButton2()
{
	//��ʾ�����Ϣ
	UpdateData(TRUE);
	CString str;
	str.Format("%i", m_conId);
	//��ȡ����
	int nRows = 0;
	nRows = m_tradelist.GetItemCount();
	//�ҳ���Լ������
	int nIndex = -1;
	for (int i = 0; i < nRows; i++)
	{
		if (m_tradelist.GetItemText(i, 0).Find(str) >= 0)
		{
			nIndex = i;
		}
	}
	//�����Լ������
	if (nIndex == -1){
		m_tradelist.InsertItem(m_conId, str);
		LVFINDINFO info;
		info.flags = LVFI_PARTIAL | LVFI_STRING;
		info.psz = str;
		nIndex = m_tradelist.FindItem(&info);
		str.Format("%i", m_amount);
		m_tradelist.SetItemText(nIndex, 1, str);
		str.Format("%i", m_multi);
		m_tradelist.SetItemText(nIndex, 2, str);

		//���¶�����Ϣ
		int i = 0;
		while (i < 3){
			if (conId[i] <0){

				conId[i] = m_conId;
				multi[i] = m_multi;
				count[i] = m_count;
				amount[i] = m_amount;
				symbol[i] = m_symbol;
				exchange[i] = m_exchange;
				currency[i] = m_currency;
				type[i] = m_type;
				//Ҫ���г�����
				Contract temp;
				int id = m_conId;//��Լ��ֱ����Ϊ���ĺŷ�ֹ����ظ�
			
				temp.conId = conId[i];
				temp.symbol = symbol[i];
				temp.exchange = exchange[i];
				temp.currency = currency[i];
				temp.secType = type[i];
				TagValueListSPtr m_mktDataOptions;
				CString m_genericTicks = _T("100,101,104,105,106,107,165,221,225,233,236,258,293,294,295,318");
				trade->reqMktData(id, temp, m_genericTicks, false, m_mktDataOptions);//��Լ��ֱ����Ϊ���ĺ�
				//m_id++;
				break;
			}
			else
				i++;
		}
		if(i==3)
			m_info.InsertString(-1,"The list is full!");
		
	}

	//�����Լ�Ѵ���
	else
	{
		//���±����Ϣ
		str.Format("%i", m_amount);
		m_tradelist.SetItemText(nIndex, 1, str);
		str.Format("%i", m_multi);
		m_tradelist.SetItemText(nIndex, 2, str);

		//���¶�����Ϣ
		int i = 0;
		str.Format("%i", m_conId);
		while (i < 3)
		if (conId[i] != str)
			i++;
		//conId[i] = m_conId;
		multi[i] = m_multi;
		count[i] = m_count;
		amount[i] = m_amount;
		//symbol[i] = m_symbol;
		//exchange[i] = m_exchange;
		//currency[i] = m_currency;
		//type[i] = m_type;
	}

	
}

//��۲�
void CDlgTrade::OnBnClickedButton3()
{
	UpdateData(TRUE);
	//m_info.InsertString(-1, "buy");
	int i = 0;
	Order order;
	Contract temp;
	CString str;
	
	
	for (i = 0; i < 3; i++){
		if (conId[i]>0){
			//�ҵ�������
			str.Format("%i", conId[i]);
			LVFINDINFO info;
			int nIndex;
			info.flags = LVFI_PARTIAL | LVFI_STRING;
			info.psz = str;
			nIndex = m_tradelist.FindItem(&info);

			//contract information
			temp.conId = conId[i];
			temp.symbol = symbol[i];
			temp.exchange = exchange[i];
			temp.currency = currency[i];
			temp.secType = type[i];

			//order information
			if (amount[i] > 0){
				order.action = _T("BUY");
				str = m_tradelist.GetItemText(nIndex, 3);
				double e = atof((LPSTR)(LPCTSTR)str.GetBuffer(str.GetLength()));
				order.lmtPrice = e;
				order.totalQuantity = amount[i] * count[i];
			}
			else{
				order.action = _T("SELL");
				str = m_tradelist.GetItemText(nIndex, 4);
				double e = atof((LPSTR)(LPCTSTR)str.GetBuffer(str.GetLength()));
				order.lmtPrice = e;
				order.totalQuantity = amount[i] * count[i] * (-1);
			}
	
			order.orderType = _T("MKT");
			order.clientId = conId[i];
			order.orderId = m_orderid;
			
			//�µ�
			m_orderid++;
			trade->placeOrder(m_orderid, temp, order);
			str.Format("palce order:%i", conId[i]);
			m_info.InsertString(-1, str);
			
		}
	}
	//m_info.InsertString(-1, "buy finished");
	
}

//���۲�
void CDlgTrade::OnBnClickedButton4()
{
	UpdateData(TRUE);
	//m_info.InsertString(-1, "buy");
	int i = 0;
	Order order;
	Contract temp;
	CString str;


	for (i = 0; i < 3; i++){
		if (conId[i]>0){
			//�ҵ�������
			str.Format("%i", conId[i]);
			LVFINDINFO info;
			int nIndex;
			info.flags = LVFI_PARTIAL | LVFI_STRING;
			info.psz = str;
			nIndex = m_tradelist.FindItem(&info);

			//contract information
			temp.conId = conId[i];
			temp.symbol = symbol[i];
			temp.exchange = exchange[i];
			temp.currency = currency[i];
			temp.secType = type[i];

			//order information
			if (amount[i] < 0){
				order.action = _T("BUY");
				str = m_tradelist.GetItemText(nIndex, 3);
				double e = atof((LPSTR)(LPCTSTR)str.GetBuffer(str.GetLength()));
				order.lmtPrice = e;
				order.totalQuantity = amount[i] * count[i] * (-1);
			}
			else{
				order.action = _T("SELL");
				str = m_tradelist.GetItemText(nIndex, 4);
				double e = atof((LPSTR)(LPCTSTR)str.GetBuffer(str.GetLength()));
				order.lmtPrice = e;
				order.totalQuantity = amount[i] * count[i];
			}
			
			order.orderType = _T("MKT");
			order.clientId = m_conId;
			order.orderId = m_orderid;


			//�µ�
			m_orderid++;
			trade->placeOrder(m_orderid, temp, order);
			
		}
	}

}

//ɾ�������Ϣ
void CDlgTrade::OnBnClickedButton5()
{
	CString str;
	CString a;
	CString m;
	
	int select;
	int nItem = m_tradelist.GetSelectionMark();
	if(nItem!=-1){
			str=m_tradelist.GetItemText(nItem, 0);
			select = int(atof((LPSTR)(LPCTSTR)str.GetBuffer(str.GetLength())));
			m_conId=select;
			m_tradelist.DeleteItem(nItem);
				//ֹͣ�г���Ϣ
				trade->cancelMktData(int(m_conId));
				//���¶���������Ϣ
				int i = 0;
				str.Format("%i", m_conId);
				while (i < 3)
				{
					if (conId[i] != m_conId)
						i++;
					else
					if (multi[i] == m_multi&&count[i] == m_count)
					{
						conId[i] = NULL;
						multi[i] = NULL;
						count[i] = NULL;
						amount[i] = NULL;
						//symbol[i] = NULL;
						//exchange[i] = NULL;
						//currency[i] = NULL;
					}
				}
	}
	else{
	
	
	UpdateData(TRUE);
	//��ȡ����
	int nRows = 0;
	nRows = m_tradelist.GetItemCount();
	//�ҳ���Լ������
	//int nIndex = -1;

	str.Format("%i", m_conId);
	bool flag = false;

	for (int i = 0; i < nRows; i++)
	{
		if (flag == true)
			break;

		if (m_tradelist.GetItemText(i, 0).Find(str) >= 0)
		{
			a.Format("%i", m_amount);
			m.Format("%i", m_multi);
			if (m_tradelist.GetItemText(i, 1) == a&&m_tradelist.GetItemText(i, 2) == m)
			{
				m_tradelist.DeleteItem(i);
				//ֹͣ�г���Ϣ
				trade->cancelMktData(int(m_conId));
				//���¶�����Ϣ
				int i = 0;
				str.Format("%i", m_conId);
				while (i < 3)
				{
					if (conId[i] != str)
						i++;
					else
					if (multi[i] == m_multi&&count[i] == m_count)
					{
						conId[i] = NULL;
						multi[i] = NULL;
						count[i] = NULL;
						amount[i] = NULL;
						//symbol[i] = NULL;
						//exchange[i] = NULL;
						//currency[i] = NULL;
					}
				}
				flag = true;
			}
		}
	}
		}	
}

void CDlgTrade::OnBnClickedCancel()
{
	UpdateData(TRUE);
	//stop = true;
	CDialogEx::OnCancel();
}

//������⣺�����г���Ϣ������۲�
void CDlgTrade::tickPrice(TickerId tickerId, TickType field, double price, int canAutoExecute)
{
	CString str, temp;
	str.Format("%f", price);
	temp.Format("%i", tickerId);

	/*
	//����к�Լ��ѡ��
	int select;
	int nItem = m_tradelist.GetSelectionMark();
	if(nItem!=-1){
			str=m_tradelist.GetItemText(nItem, 0);
			select = int(atof((LPSTR)(LPCTSTR)str.GetBuffer(str.GetLength())));
			m_conId=select;
			int i=0;
			while (i < a){
					if (conId[i] != m_conId)
						i++;
					else{
						m_multi=multi[i];
						m_count=count[i];
						m_amount=amount[i];
						m_symbol=symbol[i];
						m_exchange=exchange[i];
						m_currency=currency[i];
					}
			}
	}
	*/

	LVFINDINFO info;
	int nIndex;
	info.flags = LVFI_PARTIAL | LVFI_STRING;
	info.psz = temp;
	nIndex = m_tradelist.FindItem(&info);
	//str.Format("%i", nIndex);
	//insert
	if (field == 1)
		m_tradelist.SetItemText(nIndex, 3, str);
	if (field == 2)
		m_tradelist.SetItemText(nIndex, 4, str);

	// ������۲����۲����ʾ
	CEdit* show;
	double diff_buy, diff_sell;
	int  a2, a3, b2, b3;
	double a1,b1;
	int nRows = 0;
	nRows = m_tradelist.GetItemCount();
	diff_buy=0;
	diff_sell=0;
	


	//������������Լ��ϵ��һ��һ��������¼���۲�
	if (nRows==2){
		//�˴�Aϵ��Ϊ��Bϵ��Ϊ��
		//A����һ�ۣ�*ϵ��*����-B����һ�ۣ�*ϵ��*����
		//str = m_tradelist.GetItemText(0, 4);
		//a1 = atof((LPSTR)(LPCTSTR)str.GetBuffer(str.GetLength()));//��һ��
		str = m_tradelist.GetItemText(0, 1);
		a2 = atof((LPSTR)(LPCTSTR)str.GetBuffer(str.GetLength()));//ϵ��
		str = m_tradelist.GetItemText(0, 2);
		a3 = atof((LPSTR)(LPCTSTR)str.GetBuffer(str.GetLength()));//����
		//str = m_tradelist.GetItemText(1, 3);
		//b1 = atof((LPSTR)(LPCTSTR)str.GetBuffer(str.GetLength()));//��һ��
		str = m_tradelist.GetItemText(1, 1);
		b2 = atof((LPSTR)(LPCTSTR)str.GetBuffer(str.GetLength()));//ϵ��
		str = m_tradelist.GetItemText(1, 2);
		b3 = atof((LPSTR)(LPCTSTR)str.GetBuffer(str.GetLength()));//����

		if(a2>0&&b2<0){
			str = m_tradelist.GetItemText(0, 4);
			a1 = atof((LPSTR)(LPCTSTR)str.GetBuffer(str.GetLength()));//��һ��
			str = m_tradelist.GetItemText(1, 3);
			b1 = atof((LPSTR)(LPCTSTR)str.GetBuffer(str.GetLength()));//��һ��
			//calculate
			diff_buy = a1*abs(a2)*a3 - b1*abs(b2)*b3;
		}

		if(a2<0&&b2>0){
			str = m_tradelist.GetItemText(0, 3);
			a1 = atof((LPSTR)(LPCTSTR)str.GetBuffer(str.GetLength()));//��һ��
			str = m_tradelist.GetItemText(1, 4);
			b1 = atof((LPSTR)(LPCTSTR)str.GetBuffer(str.GetLength()));//��һ��
			//calculate
			diff_buy = a1*abs(a2)*a3 - b1*abs(b2)*b3;
		}
		

		//debug
		//str = m_tradelist.GetItemText(1, 3);
		//diff_buy = atof((LPSTR)(LPCTSTR)str.GetBuffer(str.GetLength()));//��һ��
		//str = m_tradelist.GetItemText(1, 4);
		str.Format("%f", diff_buy);
		show = (CEdit*)GetDlgItem(IDC_EDIT8);
		show->SetWindowText(_T(str));

		//�Զ���
		if(diff_buy<thresh_buy){
			OnBnClickedButton3();
		}


		//A����һ�ۣ�*ϵ��*����-B����һ�ۣ�*ϵ��*����
		//str = m_tradelist.GetItemText(0, 3);
		//a1 = atof((LPSTR)(LPCTSTR)str.GetBuffer(str.GetLength()));//��һ��
		str = m_tradelist.GetItemText(0, 1);
		a2 = atof((LPSTR)(LPCTSTR)str.GetBuffer(str.GetLength()));//ϵ��
		str = m_tradelist.GetItemText(0, 2);
		a3 = atof((LPSTR)(LPCTSTR)str.GetBuffer(str.GetLength()));//����
		//str = m_tradelist.GetItemText(1, 4);
		//b1 = atof((LPSTR)(LPCTSTR)str.GetBuffer(str.GetLength()));//��һ��
		str = m_tradelist.GetItemText(1, 1);
		b2 = atof((LPSTR)(LPCTSTR)str.GetBuffer(str.GetLength()));//ϵ��
		str = m_tradelist.GetItemText(1, 2);
		b3 = atof((LPSTR)(LPCTSTR)str.GetBuffer(str.GetLength()));//����

		if(a2<0&&b2>0){
			str = m_tradelist.GetItemText(0, 4);
			a1 = atof((LPSTR)(LPCTSTR)str.GetBuffer(str.GetLength()));//��һ��
			str = m_tradelist.GetItemText(1, 3);
			b1 = atof((LPSTR)(LPCTSTR)str.GetBuffer(str.GetLength()));//��һ��
			//calculate
			diff_sell = a1*abs(a2)*a3 - b1*abs(b2)*b3;
		}

		if(a2>0&&b2<0){
			str = m_tradelist.GetItemText(0, 3);
			a1 = atof((LPSTR)(LPCTSTR)str.GetBuffer(str.GetLength()));//��һ��
			str = m_tradelist.GetItemText(1, 4);
			b1 = atof((LPSTR)(LPCTSTR)str.GetBuffer(str.GetLength()));//��һ��
			//calculate
			diff_sell = a1*abs(a2)*a3 - b1*abs(b2)*b3;
		}

		
		//diff_sell = 5;
		str.Format("%f", diff_sell);
		show = (CEdit*)GetDlgItem(IDC_EDIT9);
		show->SetWindowText(_T(str));
		
		//�Զ���
		if(diff_buy>thresh_sell){
			OnBnClickedButton4();
		}
	}
}

//������Ϣ
void CDlgTrade::orderStatus(OrderId orderId, const CString &status, int filled, int remaining,
	double avgFillPrice, int permId, int parentId, double lastFillPrice,
	int clientId, const CString& whyHeld)
{
	// create string
	CString str;
	str.Format("orderId=%i clientId=%i permId=%i status=%s \r\n   filled=%i  remaining=%i  avgFillPrice=%f lastFillPrice=%f parentId=%i whyHeld=%s",
		orderId, clientId, permId, (const char *)status, filled, remaining, avgFillPrice, lastFillPrice, parentId, (const char*)whyHeld);

	// add to listbox
	int i = m_info.InsertString(-1,str);
}

//����ϵͳ��Ϣ
void CDlgTrade::error(const int id, const int errorCode, const IBString errorString)
{
	CString displayString;
	//displayString.Format("Error: tickerID: %d, Errorcode: %d2", id, errorCode);
	//m_server.InsertString(-1,displayString);
	displayString.Format(errorString);
	m_info.InsertString(-1, displayString);
}

//����ֹͣ
void CDlgTrade::OnBnClickedButton6()
{
	//�������е���

	//�Ͽ�����
	trade->eDisconnect();
	m_info.InsertString(-1, "Disconnected.");
}


void CDlgTrade::OnBnClickedButton9()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDlgTrade::~CDlgTrade();
}


//ѡ�б����¼�


void CDlgTrade::OnNMClickListview(NMHDR *pNMHDR, LRESULT *pResult)
{
	//UpdateData();
	//m_info.InsertString(-1, "Debug0.");
	LPNMITEMACTIVATE lpNMItemActivate = (LPNMITEMACTIVATE)pNMHDR;

	/*
		m_info.InsertString(-1, "Debug1.");
		int select;
		CString str;
		int nItem = m_tradelist.GetSelectionMark();
		if(nItem!=-1){
			str=m_tradelist.GetItemText(nItem, 0);
			select = int(atof((LPSTR)(LPCTSTR)str.GetBuffer(str.GetLength())));
			m_conId=select;
			int i=0;
			while (i < a){
					if (conId[i] != m_conId)
						i++;
					else{
						m_multi=multi[i];
						m_count=count[i];
						m_amount=amount[i];
						m_symbol=symbol[i];
						m_exchange=exchange[i];
						m_currency=currency[i];
						UpdateData();
					}
			}
		}
	
	*/
	// TODO: Add your control notification handler code here
	*pResult = 0;
}


