
// RFIDDlg.cpp: 구현 파일
//
#include "is_d2xx.h"
#include "pch.h"
#include "framework.h"
#include "RFID.h"
#include "RFIDDlg.h"
#include "afxdialogex.h"
#include <iostream>
#include <cstring>

// sound 출력용
#include <stdio.h>
#include <conio.h>
#include <Windows.h>
#include "mmsystem.h"
#pragma comment(lib,"winmm.lib") // 라이브러리 불러오기

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;
// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.
enum ThreadWorking
{
	STOP = 0,
	RUNNING = 1,
};
bool m_bThreadStart;
CWinThread* m_pThread;
ThreadWorking m_ThreadWorkType;

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CRFIDDlg 대화 상자



CRFIDDlg::CRFIDDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_RFID_DIALOG, pParent)
	, m_strRfid(_T(""))
	, m_strName(_T(""))
	, m_intPrice(0)
	, m_strType(_T(""))

	, m_strCountry(_T(""))
	, m_strProductionYear(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	conn = mysql_init(NULL);
	mysql_options(conn, MYSQL_OPT_CONNECT_TIMEOUT, &timeout_sec);
	conn_result = mysql_real_connect(conn, "127.0.0.1", "root", "hyonwoo12", "mysql", 3306, NULL, 0);
}
CRFIDDlg::~CRFIDDlg()
{
	mysql_free_result(result);
	mysql_close(conn);
	OnDisconnect();
}

void CRFIDDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT2, m_strName);
	DDX_Text(pDX, IDC_EDIT3, m_intPrice);
	DDX_Text(pDX, IDC_EDIT4, m_strType);
	DDX_Control(pDX, IDC_PIC, m_picture_control);
	DDX_Text(pDX, IDC_EDIT1, m_strCountry);
	DDX_Text(pDX, IDC_EDIT5, m_strProductionYear);
}

BEGIN_MESSAGE_MAP(CRFIDDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CRFIDDlg::OnConnect)
	ON_BN_CLICKED(IDC_BUTTON2, &CRFIDDlg::OnDisconnect)
	ON_BN_CLICKED(IDC_BUTTON5, &CRFIDDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CRFIDDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON3, &CRFIDDlg::OnClickedThreadButton)
END_MESSAGE_MAP()


// CRFIDDlg 메시지 처리기

BOOL CRFIDDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

UINT CRFIDDlg::ThreadFuctionFirstTest(LPVOID _mothod)
{
	CRFIDDlg* pDlg = (CRFIDDlg*)AfxGetApp()->m_pMainWnd;
	int num = 0;
	while (m_ThreadWorkType == RUNNING)
	{

		pDlg->Maria_db();

		Sleep(500);
	}

	return 0;
}

void CRFIDDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CRFIDDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CRFIDDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CRFIDDlg::OnConnect()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
		//열린 포트번호 찾기
	if (is_GetDeviceNumber(&usbnumber) == IS_OK)
	{
		printf("FTDI USB To Serial 연결된 개수 : %d\n", usbnumber);
		if (is_GetSerialNumber(0, readSerialNumber) == IS_OK)
		{
			if (memcmp(changeSerialNumber, readSerialNumber, sizeof(changeSerialNumber) != 0))
			{
				if (is_SetSerialNumber(0, changeSerialNumber) == IS_OK)
				{
					printf(" USB To Serial Number 를 변경 하였습니다.\n");
					printf(" FTDI SerialNumber :  %s \n", changeSerialNumber);
				}
			}
			else
				printf(" FTDI SerialNumber :  %s \n", changeSerialNumber);
		}
	}

	//열린 포트번호로 연결
	unsigned long portNumber;
	if (is_GetCOMPort_NoConnect(0, &portNumber) == IS_OK)
	{
		printf("COM Port : %d\n", portNumber);
	}
	if (is_OpenSerialNumber(&ftHandle, readSerialNumber, 115200) != IS_OK)
	{
		printf("USB To Serial과 통신 연결 실패\n");
		//return -1;
	}
	else {
		printf("Serial포트 %d와 통신 연결성공!! \n", portNumber);
	}
	Sleep(100);
	flag_r = 0;

}


void CRFIDDlg::OnDisconnect()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
		// 무선파워를 끊어요.
	is_RfOff(ftHandle);
	//USB 포트를 Close
	if (is_Close(ftHandle) == IS_OK)
	{
		printf("연결을 닫습니다. ");
	}

}


void CRFIDDlg::OnBnClickedButton5()
{
	if (m_pThread == NULL)
	{
		AfxMessageBox(L"Thread Not Start!!");
	}
	else
	{
		m_pThread->SuspendThread();

		DWORD dwResult;
		::GetExitCodeThread(m_pThread->m_hThread, &dwResult);

		delete m_pThread;
		m_pThread = NULL;

		m_ThreadWorkType = STOP;
	}
}


void CRFIDDlg::OnBnClickedButton6()
{
	Maria_db();
}

void CRFIDDlg::OnClickedThreadButton()
{

	if (m_pThread == NULL)
	{
		m_pThread = AfxBeginThread(ThreadFuctionFirstTest, this);
		if (m_pThread == NULL)
		{
			AfxMessageBox(_T("Error!!"));
		}
		m_pThread->m_bAutoDelete = TRUE;
		m_ThreadWorkType = RUNNING;
	}
}


void CRFIDDlg::Read_Card()
{
	CString temp, temp1 = _T("");

	if (is_WriteReadCommand(ftHandle, CM1_COMMON, CMD2_COMMON_ALL_UID_READ + BUZZER_ON,
		writeLength, wirteData, &readLength, readData) == IS_OK)
	{
		int i;
		for (i = 0; i < readLength; i++)
		{
			temp.Format(_T("%02x "), readData[i]);
			temp1 += temp;
		}
		m_strRfid = temp1;
	}
}

void CRFIDDlg::Maria_db()
{
	CRect m_rect;
	CBitmap m_bitmap;
	CString str[8];

	int id_num =0;

	string query = "";
	MYSQL_ROW row;


	Read_Card();
	
	query = string("SELECT * FROM wine WHERE card_uid = '") + string(CT2CA(m_strRfid)) + string("'");

	// Send Query
	if (mysql_query(conn, query.c_str()))
	{
		cout << "SELECT Fail" << endl;
	}

	// Get Response
	result = mysql_store_result(conn);

	row = mysql_fetch_row(result);  // 모든 레코드 탐색
	
	if (row != nullptr)
	{
		for (size_t i = 1; i < 8; i++)
		{
			str[i] = row[i];
		}

		CRect rect;//픽쳐 컨트롤의 크기를 저장할 CRect 객체
		m_picture_control.GetWindowRect(rect);//GetWindowRect를 사용해서 픽쳐 컨트롤의 크기를 받는다.
		CDC* dc; //픽쳐 컨트롤의 DC를 가져올  CDC 포인터
		dc = m_picture_control.GetDC(); //픽쳐 컨트롤의 DC를 얻는다.
		CImage image;//불러오고 싶은 이미지를 로드할 CImage 
		image.Load(str[2]);//이미지 로드

		image.StretchBlt(dc->m_hDC, 0, 0, rect.Width(), rect.Height(), SRCCOPY);//이미지를 픽쳐 컨트롤 크기로 조정
		ReleaseDC(dc);//DC 해제

		SetDlgItemText(IDC_EDIT2, str[3]);
		SetDlgItemText(IDC_EDIT3, str[5]);
		SetDlgItemText(IDC_EDIT4, str[4]);
		SetDlgItemText(IDC_EDIT1, str[6]);
		SetDlgItemText(IDC_EDIT5, str[7]);
	}
		
	
}







