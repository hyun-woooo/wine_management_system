
// RFIDDlg.h: 헤더 파일
//

#pragma once
#include "is_d2xx.h"
#include <vector>
#include <mysql.h>
#include <thread>

// 규원이가 바꾼 내용 
	
// CRFIDDlg 대화 상자
class CRFIDDlg : public CDialogEx
{
// 생성입니다.
public:
	CRFIDDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.
	~CRFIDDlg();
	IS_HANDLE ftHandle = 0;
	char readSerialNumber[100] = "COM07";
	char changeSerialNumber[100] = "RFID01";
	short usbnumber;
	unsigned char wirteData[1024];
	unsigned short writeLength = 0;
	unsigned char readData[1024];
	unsigned short readLength = 0;
	BOOL flag_r=0;

	//Maria_db
	MYSQL* conn;
	MYSQL_RES* result;
	MYSQL* conn_result;
	unsigned int timeout_sec = 1;
	
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RFID_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnConnect();
	afx_msg void OnDisconnect();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnClickedThreadButton();

	CString m_strRfid;
	CString m_strName;
	CString m_strType;
	CStatic m_picture_control;
	CString m_strCountry;
	CString m_strProductionYear;
	int m_intPrice;

	void Read_Card();
	void Maria_db();
	
	static UINT ThreadFuctionFirstTest(LPVOID _mothod);
	
	
};
