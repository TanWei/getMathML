
// getMathMLDlg.cpp : implementation file
//

#include "stdafx.h"
#include "getMathML.h"
#include "getMathMLDlg.h"
#include "afxdialogex.h"

#include <codecvt>

#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//----myheaderS---
#include "MathTypeProcessor.h"
using namespace std;

//----myheaderE---



// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CgetMathMLDlg dialog




CgetMathMLDlg::CgetMathMLDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CgetMathMLDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CgetMathMLDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CgetMathMLDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CgetMathMLDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CgetMathMLDlg message handlers

BOOL CgetMathMLDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CgetMathMLDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CgetMathMLDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CgetMathMLDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CgetMathMLDlg::OnBnClickedButton1()
{
	CString filename;//保存路径
	CFileDialog opendlg (TRUE);
	if (opendlg.DoModal()==IDOK)
	{
		filename=opendlg.GetPathName();
	}
	//文件名
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> conv;
	std::string stylesheet = conv.to_bytes(filename.GetBuffer());
	

	char *fn = new char[stylesheet.length() + 1];
	strcpy(fn, stylesheet.c_str());

	CMathTypeProcessor mathTypeProcessor;
	//for (int i=0; i<100; i++)
	{
		mathTypeProcessor.ConvertToXml(fn);
	}
	
	//CMathTypeHelper mathTypeHelper;
	//vector<BYTE> v2 = mathTypeHelper.ReadFileIntoBinary(fn);
	//WORD cbHdr = v2[0];
	//int idx = 0;
	//MTEF::EQNOLEFILEHDR header = mathTypeHelper.GetEQN_OLE_Header(v2, idx);
	//std::vector<BYTE> MTEFBYTE(v2.begin()+(UINT)header.cbHdr, v2.begin()+(UINT)header.cbObject);
	//if (MTEFBYTE[0] != (BYTE)(0x05) || MTEFBYTE[1] != (BYTE)(0x01))
	//{
	//	//mtef版本不是5，程序版本不是windows，return
	//	return;
	//}

	delete[] fn;
	//mathTypeHelper.WriteBinaryIntoFile(fn1, v2);
	int n = 0;
}
