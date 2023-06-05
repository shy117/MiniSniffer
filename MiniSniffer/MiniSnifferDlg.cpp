
// MiniSnifferDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "MiniSniffer.h"
#include "MiniSnifferDlg.h"
#include "afxdialogex.h"

using namespace std;

//vs安全性

#pragma warning(disable : 4996)
#define strcpy(a,b) strcpy_s((a),strlen((b))+1, (b)) 
#define strcat(a,b) strcat_s((a), strlen((a))+strlen((b))+1, (b))

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//被调用函数的定义
DWORD WINAPI capture(LPVOID lpParameter);

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CMiniSnifferDlg 对话框



CMiniSnifferDlg::CMiniSnifferDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MINISNIFFER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMiniSnifferDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, m_listCtrl);
	DDX_Control(pDX, IDC_COMBO1, m_comboBox);
	DDX_Control(pDX, IDC_TREE1, m_treeCtrl);
	DDX_Control(pDX, IDC_EDIT1, m_edit);
	DDX_Control(pDX, IDC_BUTTON1, m_buttonStart);
	DDX_Control(pDX, IDC_BUTTON2, m_buttonStop);
	DDX_Control(pDX, IDC_EDIT3, m_edit_IP);
	DDX_Control(pDX, IDC_EDIT4, m_edit_up);
	DDX_Control(pDX, IDC_EDIT5, m_edit_down);
	DDX_Control(pDX, IDC_EDIT6, m_editNIpv4);
	DDX_Control(pDX, IDC_EDIT7, m_editNIpv6);
	DDX_Control(pDX, IDC_EDIT8, m_editNArp);
	DDX_Control(pDX, IDC_EDIT9, m_editNTcp);
	DDX_Control(pDX, IDC_EDIT10, m_editNUdp);
	DDX_Control(pDX, IDC_EDIT11, m_editNIcmp);
	DDX_Control(pDX, IDC_EDIT12, m_editNIcmpv6);
	DDX_Control(pDX, IDC_EDIT13, m_editNOther);
	DDX_Control(pDX, IDC_EDIT14, m_editNSum);
}

BEGIN_MESSAGE_MAP(CMiniSnifferDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()

	ON_BN_CLICKED(IDC_BUTTON1, &CMiniSnifferDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CMiniSnifferDlg::OnBnClickedButton2)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST2, &CMiniSnifferDlg::OnLvnItemchangedList2)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST2, &CMiniSnifferDlg::OnNMCustomdrawList2)
	ON_BN_CLICKED(IDC_BUTTON5, &CMiniSnifferDlg::OnBnClickedButton5)
	ON_COMMAND(ID_MENU_FILE_OPEN, &CMiniSnifferDlg::OnMenuFileOpen)
	ON_COMMAND(ID_MENU_FILE_CLOSE, &CMiniSnifferDlg::OnMenuFileClose)
	ON_COMMAND(ID_MENU_FILE_SAVEAS, &CMiniSnifferDlg::OnMenuFileSaveAs)
	ON_COMMAND(ID_MENU_FILE_EXIT, &CMiniSnifferDlg::OnMenuFileExit)
	ON_COMMAND(ID_IP_CZ, &CMiniSnifferDlg::OnIpCz)
END_MESSAGE_MAP()


// CMiniSnifferDlg 消息处理程序

BOOL CMiniSnifferDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	initialMenuBar();					// 菜单栏初始化
	initialComboBoxDevList();			// 网卡列表初始化
	initialListCtrlPacketList();		// 列表控件（数据包列表）初始化
	initialStatusBar();					// 状态栏初始化
	
	m_buttonStop.EnableWindow(FALSE);
	
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMiniSnifferDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMiniSnifferDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMiniSnifferDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/*************************************************************
*
*		控件初始化
*
*************************************************************/

//菜单栏初始化
void CMiniSnifferDlg::initialMenuBar()
{
	m_menu.LoadMenu(IDR_MENU1);
	SetMenu(&m_menu);

	/* 菜单项禁用 */
//	CMenu* pMenu = this->GetMenu();
	if (m_menu)
	{
		m_menu.EnableMenuItem(ID_MENU_FILE_CLOSE, MF_GRAYED);	// 禁用菜单项"关闭"
		m_menu.EnableMenuItem(ID_MENU_FILE_SAVEAS, MF_GRAYED);	// 禁用菜单项"另存为"
	}
}

//获取本地机器网卡列表,并打印网卡描述到下拉列表中

bool CMiniSnifferDlg::initialComboBoxDevList()
{
	if (initCap())//initCap返回1则初始化异常
		return FALSE;
	m_comboBox.AddString(_T("选择网卡"));
	char errbuf[PCAP_ERRBUF_SIZE + 1];
	if (pcap_findalldevs(&alldev, errbuf) == -1)
	{
		AfxMessageBox(_T("pcap_findalldevs错误!"), MB_OK);
		return FALSE;
	}
	for (dev = alldev; dev != NULL; dev = dev->next)
	{
		if (dev->description != NULL)
			m_comboBox.AddString(CString(dev->description));
	}
	m_comboBox.SetCurSel(0);
	m_edit_IP.SetWindowText(b_IP);
	return TRUE;
}

//列表控件（数据包列表）初始化
void CMiniSnifferDlg::initialListCtrlPacketList()
{
	m_listCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_listCtrl.InsertColumn(0, _T("编号"), 3, 50);
	m_listCtrl.InsertColumn(1, _T("时间"), 3, 170);
	m_listCtrl.InsertColumn(2, _T("长度"), 3, 70);
	m_listCtrl.InsertColumn(3, _T("源MAC地址"), 3, 150);
	m_listCtrl.InsertColumn(4, _T("目的MAC地址"), 3, 150);
	m_listCtrl.InsertColumn(5, _T("包类型"), 3, 80);
	m_listCtrl.InsertColumn(6, _T("源IP地址"), 3, 145);
	m_listCtrl.InsertColumn(7, _T("目的IP地址"), 3, 145);
}

//状态栏初始化
void CMiniSnifferDlg::initialStatusBar()
{
	if (m_statusBar.Create(this))	// 创建菜单栏
	{
		static UINT indicators[] =
		{
			ID_INDICATOR_STATUS,
			ID_INDICATOR_PKT_TOTAL_NUM,
			ID_INDICATOR_PKT_DISPLAY_NUM
		};
		int indicatorsSize = sizeof(indicators) / sizeof(UINT);
		m_statusBar.SetIndicators(indicators, indicatorsSize);
		CRect rect;
		GetClientRect(rect);
		int index = 0;
		m_statusBar.SetPaneInfo(index, ID_INDICATOR_STATUS, SBPS_STRETCH, rect.Width() * 0.6);
		m_statusBar.SetPaneInfo(++index, ID_INDICATOR_PKT_TOTAL_NUM, SBPS_NORMAL, rect.Width() * 0.2);
		m_statusBar.SetPaneInfo(++index, ID_INDICATOR_PKT_DISPLAY_NUM, SBPS_NORMAL, rect.Width() * 0.15);
		RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0); // 显示状态栏
	}
}

/**
*	@brief	更新状态栏
*	@param [in]	status	状态
*	@param [in]	pktTotalNum	数据包总数	值为非负数时更新该字段
*	@param [in]	pktDisplayNum	数据包显示个数	值为非负数时更新该字段
*	@return	-
*/
void CMiniSnifferDlg::updateStatusBar(const CString& status, int pktTotalNum, int pktDisplayNum)
{
	if (!status.IsEmpty())
	{
		int index = m_statusBar.CommandToIndex(ID_INDICATOR_STATUS);
		m_statusBar.SetPaneText(index, status, TRUE);
	}
	if (pktTotalNum >= 0)
	{
		int index = m_statusBar.CommandToIndex(ID_INDICATOR_PKT_TOTAL_NUM);
		CString text;
		text.Format(_T("数据包：%d"), pktTotalNum);
		m_statusBar.SetPaneText(index, text, TRUE);
	}
	if (pktDisplayNum >= 0)
	{
		int index = m_statusBar.CommandToIndex(ID_INDICATOR_PKT_DISPLAY_NUM);
		CString text;
		double percentage = (pktDisplayNum == 0 || pktTotalNum == 0) ?
			0.0 : ((double)pktDisplayNum / pktTotalNum * 100);
		text.Format(_T("已显示：%d (%.1f%%)"), pktDisplayNum, percentage);
		m_statusBar.SetPaneText(index, text, TRUE);
	}
}

/*************************************************************
*
*		实现触发函数
*
*************************************************************/

//开始抓包按钮
void CMiniSnifferDlg::OnBnClickedButton1()
{
	/* 是否保存上次抓包数据 */
	if (!this->m_localDataList.IsEmpty())
	{
		if (MessageBox(_T("是否存储当前抓包数据？"), _T("警告"), MB_YESNO) == IDYES)
			this->savefile();
	}
	/* init */
	this->n_pkt = 0; //重新计数
	this->m_localDataList.RemoveAll(); //列表清空
	this->m_netDataList.RemoveAll();
	memset(&(this->pkcount_T), 0, sizeof(struct pktcount));
	GetDlgItem(IDC_EDIT3)->GetWindowTextW(this->b_IP);
	upload = 0;		//初始化
	download = 0;
	this->m_edit_up.SetWindowText(_T("0"));
	this->m_edit_down.SetWindowText(_T("0"));
	/* 开始抓包 */
	if (this->start() == -1)
		return;//异常在start里已经处理 这里不处理
	/* 清除窗口 */
	this->m_listCtrl.DeleteAllItems();
	this->m_treeCtrl.DeleteAllItems();
	this->m_edit.SetWindowTextW(_T(""));
	
	this->m_buttonStart.EnableWindow(FALSE);	//禁止重复开始
	this->m_buttonStop.EnableWindow(TRUE);		//停止按钮使能
	//更新数据包
	lixsniff_updateNPacket();
	updateStatusBar(CString("开始抓包"), 0, 0);
}

//停止抓包按钮
void CMiniSnifferDlg::OnBnClickedButton2()
{
	/* 没有线程不需要处理 */
	if (this->m_threadhandle == NULL)
		return;
	/* 关闭线程，异常处理 */
	if (handle)
		pcap_close(handle);
	if (TerminateThread(this->m_threadhandle, -1) == 0)
	{
		MessageBox(_T("错误：关闭线程失败"));
		return;
	}
	this->m_threadhandle = NULL;
	/* 处理按钮 */
	this->m_buttonStart.EnableWindow(TRUE);//开始允许
	this->m_buttonStop.EnableWindow(FALSE);//禁止两次停止

	m_menu.EnableMenuItem(ID_MENU_FILE_CLOSE, MF_ENABLED);	// 启用菜单项"关闭"
	m_menu.EnableMenuItem(ID_MENU_FILE_SAVEAS, MF_ENABLED);	// 启用菜单项"保存"
	
	upload = 0;		//初始化
	download = 0;
	updateStatusBar(CString("停止抓包"), -1, -1);
}


//选定包列表中某一项-》触发显示详情和包内容展示
void CMiniSnifferDlg::OnLvnItemchangedList2(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	/* 如果光标选择的包不在存储列表 */
	int n;
	n = this->m_listCtrl.GetHotItem();
	if (n >= this->m_localDataList.GetCount() || n == -1)
		return;

	/* 更新详细 和 展示包内容 */
	cursor_index = n;
	this->updateEdit(n);
	this->updateTree(n);
	/*自动展开树*/
	HTREEITEM tmp = this->m_treeCtrl.GetRootItem();
	this->m_treeCtrl.Expand(tmp, TVE_EXPAND);
	tmp = this->m_treeCtrl.GetNextItem(tmp, TVGN_CHILD);
	while (tmp)
	{
		this->m_treeCtrl.Expand(tmp, TVE_EXPAND);
		tmp = this->m_treeCtrl.GetNextItem(tmp, TVGN_NEXT);
	}
	*pResult = 0;
}

//确认过滤器
void CMiniSnifferDlg::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_EDIT2)->GetWindowTextW(this->filter);
}

/*************************************************************
*
*		实现定义的funcs 
*
*************************************************************/

//初始化wincap: 成功返回0 不成功返回1
bool CMiniSnifferDlg::initCap()
{
	n_dev = 0;
	if (pcap_findalldevs(&alldev, errbuf) == -1) //dev = 网卡链表头，遍历网卡链表
		return 1; //初始化出错

	for (dev = alldev; dev; dev = dev->next)
		n_dev++;
	return 0;
}

//开始抓包 异常返回-1 
int CMiniSnifferDlg::start()
{
	int dev_index;
	int i = 0;
	u_int mask;
	struct bpf_program fcode;
	char* filter_ch;

	/* dev := 选择的网卡, 创建【抓包句柄】handle */
	initCap();
	//.GetCurSel:返回光标选中的[下标]，如果没有选中返回CB_ERR
	dev_index = this->m_comboBox.GetCurSel(); //鼠标点击的网卡
	if (dev_index == 0 || dev_index == CB_ERR)//没选或没得选
	{
		MessageBox(_T("请选择一个网卡"));
		return -1;
	}
	dev = alldev;
	for (i = 1; i < dev_index; i++)
		dev = dev->next;
	
	//当前网卡，最大包长65536，【混杂模式】，持续1000ms, errbuf
	if ((handle = pcap_open_live(dev->name, 65536, 1, 1000, errbuf)) == NULL)
	{
		MessageBox(_T("无法使用网卡:" + CString(dev->description)));
		pcap_freealldevs(alldev);
		return -1;
	}
	//返回链路层类型，只分析常规的以太网
	if (pcap_datalink(handle) != DLT_EN10MB)
	{
		MessageBox(_T("当前选择网卡不是以太网卡"));
		pcap_freealldevs(alldev);
		return -1;
	}

	if (dev->addresses != NULL)
		mask = ((struct sockaddr_in*)(dev->addresses->netmask))->sin_addr.S_un.S_addr;
	else
		mask = 0xffffffff;//default = /32

	int len = this->filter.GetLength() + 1;
	if (len != 0)
	{
		filter_ch = (char*)malloc(len);
		for (i = 0; i < len; i++)
			filter_ch[i] = this->filter.GetAt(i);
		if (pcap_compile(handle, &fcode, filter_ch, 1, mask) < 0)
		{
			MessageBox(_T("编译器规则错误，请重新输入后再次确认"));
			pcap_freealldevs(alldev);
			return -1;
		}
		if (pcap_setfilter(handle, &fcode) < 0)
		{
			MessageBox(_T("编译器编译通过但设置出错"));
			pcap_freealldevs(alldev);
			return -1;
		}
	}

	/* myfile:= 包存储位置 */
	CFileFind file;
	char thistime[30];
	struct tm* ltime = NULL;
	memset(filepath, 0, 512);
	memset(filename, 0, 512);
	if (!file.FindFile(_T("SavedDir")))
	{
		CreateDirectory(_T("SavedDir"), NULL);
	}
	time_t nowtime;
	time(&nowtime);
	ltime = localtime(&nowtime);
	strftime(thistime, sizeof(thistime), "%Y%m%d %H%M%S", ltime);
	strcpy(filepath, "SavedDir\\");
	strcat(filename, thistime);
	strcat(filename, ".pcap");

	strcat(filepath, filename);
	myfile = pcap_dump_open(handle, filepath);
	if (myfile == NULL)
	{
		MessageBox(_T("文件创建错误！"));
		return -1;
	}

	pcap_freealldevs(alldev);

	/* m_threadhandle:= 抓包线程句柄 创建抓包线程 */
	LPDWORD threadcap = NULL;
	if ((m_threadhandle = CreateThread(NULL, 0, capture, this, 0, threadcap)) == NULL)
	{
		MessageBox(_T("创建抓包线程失败"));
		return -1;
	}
	return 1;
}

//数据包另存为
int CMiniSnifferDlg::savefile()
{
	CFileFind findfile;
	if (findfile.FindFile(CString(filepath)) == NULL)
	{
		MessageBox(_T("没有找到文件保存路径"));
		return -1;
	}
	//false 表示另存为
	CFileDialog FileDlg(FALSE, _T("pcap"), NULL, OFN_OVERWRITEPROMPT, _T("pcap文件 (*.pcap)|*.pcap|所有文件 (*.*)|*.*||"), NULL);
	FileDlg.m_ofn.lpstrInitialDir = file_dir;
	if (FileDlg.DoModal() == IDOK)
		CopyFile(CString(filepath), FileDlg.GetPathName(), TRUE);
	return 0;
}

//CString转char* 用于readfile转换文件地址为char*才能openoffline
void CString2char(CString s, int len, char* c)
{
	int i;
	memset(c, 0, len);
	for (i = 0; i < len; i++)
		c[i] = (char)s.GetAt(i);
}
//读取数据包文件
int CMiniSnifferDlg::readfile(CString filepath)
{
	int indexItem;
	struct tm* ltime = NULL;
	time_t time_T;
	CString timestring, buf;
	
	struct pcap_pkthdr* head;	  //获取网络包头部
	const u_char* pktdata = NULL; //获取当前网络包数据部分
	u_char* pktdata_bk;			  //网络包数据部分备份
	pcap_t* fp;					//文件指针

	/* 转换文件路径格式为char* */
	int len = filepath.GetLength() + 1;
	char* charpath = (char*)malloc(len);
	if (charpath == NULL)
		return -1;
	CString2char(filepath, len, charpath);

	/* 打开文件 */
	if ((fp = pcap_open_offline(charpath, errbuf)) == NULL)
	{
		MessageBox(_T("打开文件失败") + CString(errbuf));
		return -1;
	}
	free(charpath);
	/* 遍历文件中的每个包 数据存到pktdata */
	while ((pcap_next_ex(fp, &head, &pktdata)) >= 0)
	{
		/* 申请空当前包规范结构体 */
		struct pkt_T* package_T = (struct pkt_T*)malloc(sizeof(struct pkt_T));
		if (package_T)
			memset(package_T, 0, sizeof(struct pkt_T));
		//异常：内存不够
		if (package_T == NULL)
		{
			MessageBox(_T("分配内存失败：分析包申请内存失败"));
			return -1;
		}
		/* 从外向内分析各层数据 如果失败就continue分析下个包 */
		/* 将 pktdata 不断向 package_T 中各个结构体赋值，使得抓包数据规范化 */
		if (analyze_frame(pktdata, package_T, &(this->pkcount_T)))
			continue;
		//更新统计信息
		this->n_pkt++;
		//备份包数据加入链表
		pktdata_bk = (u_char*)malloc(head->len);
		if (pktdata_bk)
			memcpy(pktdata_bk, pktdata, head->len);
		//加入链表
		this->m_localDataList.AddTail(package_T);
		this->m_netDataList.AddTail(pktdata_bk);
		//时间 长度
		package_T->len = head->len;
		time_T = head->ts.tv_sec;
		ltime = localtime(&time_T);
		package_T->time[0] = ltime->tm_year - 100; // 这里年从1900计数
		package_T->time[1] = ltime->tm_mon + 1;
		package_T->time[2] = ltime->tm_mday;
		package_T->time[3] = ltime->tm_hour;
		package_T->time[4] = ltime->tm_min;
		package_T->time[5] = ltime->tm_sec;
		/* window显示 */
		//序号
		buf.Format(_T("%d"), this->n_pkt); //插入item需要字符串
		indexItem = this->m_listCtrl.InsertItem(this->n_pkt, buf);
		//时间
		timestring.Format(_T("%d.%d.%d-%d:%d:%d"), package_T->time[0], package_T->time[1],
			package_T->time[2], package_T->time[3], package_T->time[4], package_T->time[5]);
		this->m_listCtrl.SetItemText(indexItem, 1, timestring);
		//长度
		buf.Empty();
		buf.Format(_T("%d"), package_T->len);
		this->m_listCtrl.SetItemText(indexItem, 2, buf);
		//源Mac
		buf.Empty();
		buf.Format(_T("%02x-%02x-%02x-%02x-%02x-%02x"), package_T->ethh->src[0], package_T->ethh->src[1]
			, package_T->ethh->src[2], package_T->ethh->src[3], package_T->ethh->src[4]
			, package_T->ethh->src[5]);
		this->m_listCtrl.SetItemText(indexItem, 3, buf);
		//目的Mac
		buf.Empty();
		buf.Format(_T("%02x-%02x-%02x-%02x-%02x-%02x"), package_T->ethh->dest[0], package_T->ethh->dest[1]
			, package_T->ethh->dest[2], package_T->ethh->dest[3], package_T->ethh->dest[4]
			, package_T->ethh->dest[5]);
		this->m_listCtrl.SetItemText(indexItem, 4, buf);
		//协议类型
		this->m_listCtrl.SetItemText(indexItem, 5, CString(package_T->pktType));
		//源IP
		buf.Empty();
		if (package_T->ethh->type == MACTYPE_IP) //ip包
		{
			struct in_addr ip; //ip地址的结构体
			ip.S_un.S_addr = package_T->iph->saddr;
			buf = CString(inet_ntoa(ip));	// 转十进制字符串
			if (buf == b_IP)		//显示上传
			{
				upload += package_T->len;
				CString upload_str;
				upload_str.Format(_T("%d"), upload);
				this->m_edit_up.SetWindowText(upload_str);
			}
		}
		else if (package_T->ethh->type == MACTYPE_ARP) //arp包
		{
			buf.Format(_T("%d.%d.%d.%d"), package_T->arph->ar_srcip[0], package_T->arph->ar_srcip[1],
				package_T->arph->ar_srcip[2], package_T->arph->ar_srcip[3]);
		}
		else if (package_T->ethh->type == MACTYPE_IP6) //ipv6包
		{
			buf.Format(_T("%x:%x:%x:%x:%x:%x:%x:%x"), package_T->iph6->saddr[0], package_T->iph6->saddr[1],
				package_T->iph6->saddr[2], package_T->iph6->saddr[3], package_T->iph6->saddr[4],
				package_T->iph6->saddr[5], package_T->iph6->saddr[6], package_T->iph6->saddr[7]);
		}
		this->m_listCtrl.SetItemText(indexItem, 6, buf);
		//目的ip
		buf.Empty();
		if (package_T->ethh->type == MACTYPE_IP) //ip包
		{
			struct in_addr ip; //ip地址的结构体
			ip.S_un.S_addr = package_T->iph->daddr;
			buf = CString(inet_ntoa(ip));	// 转十进制字符串
			if (buf == b_IP)		//显示下载
			{
				download += package_T->len;
				CString download_str;
				download_str.Format(_T("%d"), download);
				this->m_edit_down.SetWindowText(download_str);
			}
		}
		else if (package_T->ethh->type == MACTYPE_ARP) //arp包
		{
			buf.Format(_T("%d.%d.%d.%d"), package_T->arph->ar_destip[0], package_T->arph->ar_destip[1],
				package_T->arph->ar_destip[2], package_T->arph->ar_destip[3]);
		}
		else if (package_T->ethh->type == MACTYPE_IP6) //ipv6包
		{
			buf.Format(_T("%x:%x:%x:%x:%x:%x:%x:%x"), package_T->iph6->daddr[0], package_T->iph6->daddr[1],
				package_T->iph6->daddr[2], package_T->iph6->daddr[3], package_T->iph6->daddr[4],
				package_T->iph6->daddr[5], package_T->iph6->daddr[6], package_T->iph6->daddr[7]);
		}
		this->m_listCtrl.SetItemText(indexItem, 7, buf);
		//更新数据包
		lixsniff_updateNPacket();
		updateStatusBar(CString(""), m_listCtrl.GetItemCount(), m_listCtrl.GetItemCount());
	}
	pcap_close(fp);
	return 1;
}

//抓包线程 被strat调用
DWORD WINAPI capture(LPVOID lpParameter)
{
	int indexItem, res;
	CString timestring, buf;
	time_t time_T;
	struct tm* ltime;
	struct pcap_pkthdr* head;
	const u_char* pkt_data = NULL;
	u_char* pktdata_bk; //备份包数据到链表

	CMiniSnifferDlg* bpthis = (CMiniSnifferDlg*)lpParameter;
	if (bpthis->m_threadhandle == NULL)
	{
		MessageBox(NULL, _T("线程句柄错误"), _T("提示"), MB_OK);
		return -1;
	}
	/* 遍历文件中的每个包 数据存到pktdata */
	while ((res = pcap_next_ex(bpthis->handle, &head, &pkt_data)) >= 0)
	{
		//抓包超时
		if (res == 0)
			continue;
		/* 申请空当前包规范结构体 */
		struct pkt_T* package_T = (struct pkt_T*)malloc(sizeof(struct pkt_T));
		if (package_T)
			memset(package_T, 0, sizeof(struct pkt_T));
		//异常：内存不够
		if (package_T == NULL)
		{
			MessageBox(NULL, _T("空间已满，无法接收新的数据包"), _T("Error"), MB_OK);
			return -1;
		}
		/* 从外向内分析各层数据 如果失败就continue分析下个包 */
		/* 将 pktdata 不断向 package_T 中各个结构体赋值，使得抓包数据规范化 */
		if (analyze_frame(pkt_data, package_T, &(bpthis->pkcount_T)))
		{
			MessageBox(NULL, _T("当前解析包异常！"), _T("Error"), MB_OK);
			continue;
		}
		//将数据包保存到打开的文件中
		if (bpthis->myfile != NULL)
		{
			pcap_dump((unsigned char*)bpthis->myfile, head, pkt_data);
		}
		//更新统计信息
		bpthis->n_pkt++;
		//备份包数据加入链表
		pktdata_bk = (u_char*)malloc(head->len);
		if (pktdata_bk)
			memcpy(pktdata_bk, pkt_data, head->len);
		//加入链表
		bpthis->m_localDataList.AddTail(package_T);
		bpthis->m_netDataList.AddTail(pktdata_bk);
		//时间 长度
		package_T->len = head->len;
		time_T = head->ts.tv_sec;
		ltime = localtime(&time_T);
		package_T->time[0] = ltime->tm_year - 100; // 这里年从1900计数
		package_T->time[1] = ltime->tm_mon + 1;
		package_T->time[2] = ltime->tm_mday;
		package_T->time[3] = ltime->tm_hour;
		package_T->time[4] = ltime->tm_min;
		package_T->time[5] = ltime->tm_sec;
		/* window显示 */
		//序号
		buf.Format(_T("%d"), bpthis->n_pkt); //插入item需要字符串
		indexItem = bpthis->m_listCtrl.InsertItem(bpthis->n_pkt, buf);
		//时间
		timestring.Format(_T("%d.%d.%d-%d:%d:%d"), package_T->time[0], package_T->time[1],
			package_T->time[2], package_T->time[3], package_T->time[4], package_T->time[5]);
		bpthis->m_listCtrl.SetItemText(indexItem, 1, timestring);
		//长度
		buf.Empty();
		buf.Format(_T("%d"), package_T->len);
		bpthis->m_listCtrl.SetItemText(indexItem, 2, buf);

		//源Mac
		buf.Empty();
		buf.Format(_T("%02x-%02x-%02x-%02x-%02x-%02x"), package_T->ethh->src[0], package_T->ethh->src[1]
			, package_T->ethh->src[2], package_T->ethh->src[3], package_T->ethh->src[4]
			, package_T->ethh->src[5]);
		bpthis->m_listCtrl.SetItemText(indexItem, 3, buf);
		//目的Mac
		buf.Empty();
		buf.Format(_T("%02x-%02x-%02x-%02x-%02x-%02x"), package_T->ethh->dest[0], package_T->ethh->dest[1]
			, package_T->ethh->dest[2], package_T->ethh->dest[3], package_T->ethh->dest[4]
			, package_T->ethh->dest[5]);
		bpthis->m_listCtrl.SetItemText(indexItem, 4, buf);
		//协议类型
		bpthis->m_listCtrl.SetItemText(indexItem, 5, CString(package_T->pktType));
		//源IP
		buf.Empty();
		if (package_T->ethh->type == MACTYPE_IP) //ip包
		{
			struct in_addr ip; //ip地址的结构体
			ip.S_un.S_addr = package_T->iph->saddr;
			buf = CString(inet_ntoa(ip)); // 转十进制字符串
			if (buf == bpthis->b_IP)		//显示上传
			{
				bpthis->upload += package_T->len;
				CString upload_str;
				upload_str.Format(_T("%d"), bpthis->upload);
				bpthis->m_edit_up.SetWindowText(upload_str);
			}
		}
		else if (package_T->ethh->type == MACTYPE_ARP) //arp包
		{
			buf.Format(_T("%d.%d.%d.%d"), package_T->arph->ar_srcip[0], package_T->arph->ar_srcip[1],
				package_T->arph->ar_srcip[2], package_T->arph->ar_srcip[3]);
		}
		else if (package_T->ethh->type == MACTYPE_IP6) //ipv6包
		{
			buf.Format(_T("%x:%x:%x:%x:%x:%x:%x:%x"), package_T->iph6->saddr[0], package_T->iph6->saddr[1],
				package_T->iph6->saddr[2], package_T->iph6->saddr[3], package_T->iph6->saddr[4],
				package_T->iph6->saddr[5], package_T->iph6->saddr[6], package_T->iph6->saddr[7]);
		}
		bpthis->m_listCtrl.SetItemText(indexItem, 6, buf);
		//目的ip
		buf.Empty();
		if (package_T->ethh->type == MACTYPE_IP) //ip包
		{
			struct in_addr ip; //ip地址的结构体
			ip.S_un.S_addr = package_T->iph->daddr;
			buf = CString(inet_ntoa(ip)); // 转十进制字符串
			if (buf == bpthis->b_IP)		//显示下载
			{
				bpthis->download += package_T->len;
				CString download_str;
				download_str.Format(_T("%d"), bpthis->download);
				bpthis->m_edit_down.SetWindowText(download_str);
			}
		}
		else if (package_T->ethh->type == MACTYPE_ARP) //arp包
		{
			buf.Format(_T("%d.%d.%d.%d"), package_T->arph->ar_destip[0], package_T->arph->ar_destip[1],
				package_T->arph->ar_destip[2], package_T->arph->ar_destip[3]);
		}
		else if (package_T->ethh->type == MACTYPE_IP6) //ipv6包
		{
			buf.Format(_T("%x:%x:%x:%x:%x:%x:%x:%x"), package_T->iph6->daddr[0], package_T->iph6->daddr[1],
				package_T->iph6->daddr[2], package_T->iph6->daddr[3], package_T->iph6->daddr[4],
				package_T->iph6->daddr[5], package_T->iph6->daddr[6], package_T->iph6->daddr[7]);
		}
		bpthis->m_listCtrl.SetItemText(indexItem, 7, buf);
				
	}
	return 1;
}

//设置调色板
void SetColor(char* tmp, COLORREF& cr)
{
	if (strcmp(tmp, "IPV6") == 0)
		cr = RGB(254, 224, 254);		
	else if (strcmp(tmp, "UDP") == 0)
		cr = RGB(100, 149, 237);		
	else if (strcmp(tmp, "TCP") == 0)
		cr = RGB(144, 238, 144);		
	else if (strcmp(tmp, "ARP") == 0)
		cr = RGB(255, 182, 193);		
	else if (strcmp(tmp, "ICMP") == 0)
		cr = RGB(186, 85, 211);			
	else if (strcmp(tmp, "HTTP") == 0)
		cr = RGB(238, 232, 180);		
	else if (strcmp(tmp, "ICMPv6") == 0)
		cr = RGB(204, 204, 255);
	else if (strcmp(tmp, "HTTPS") == 0)
		cr = RGB(189, 254, 76);			
	else if (strcmp(tmp, "DNS") == 0)
		cr = RGB(135, 206, 250);		
	else if (strcmp(tmp, "FTP") == 0)
		cr = RGB(60, 200, 230);
}

//列表染色函数
void CMiniSnifferDlg::OnNMCustomdrawList2(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLVCUSTOMDRAW pNMCD = reinterpret_cast<LPNMLVCUSTOMDRAW>(pNMHDR);
	
	*pResult = 0;
	if (pNMCD->nmcd.dwDrawStage == CDDS_PREPAINT)
	{
		*pResult = CDRF_NOTIFYITEMDRAW;
	}
	else if (pNMCD->nmcd.dwDrawStage == CDDS_ITEMPREPAINT)
	{
		COLORREF color = NULL;
		char tmp[8];
		memset(tmp, 0, 8);
		POSITION index = this->m_localDataList.FindIndex(pNMCD->nmcd.dwItemSpec);
		struct pkt_T* pkt = (struct pkt_T*)this->m_localDataList.GetAt(index);
		strcpy(tmp, pkt->pktType);
		//染色,调用调色板
		SetColor(tmp, color);
		pNMCD->clrTextBk = color;
		*pResult = CDRF_DODEFAULT;
		//更新数据包
		lixsniff_updateNPacket();
		//更新抓包数
		updateStatusBar(CString(""), m_listCtrl.GetItemCount(), m_listCtrl.GetItemCount());
	}
	
}

//更新包解析树
int CMiniSnifferDlg::updateTree(int index)
{
	/* 清空树 */
	this->m_treeCtrl.DeleteAllItems();
	/* 初始化 */
	CString buf;
	POSITION localpos = this->m_localDataList.FindIndex(index);
	struct pkt_T* package_T = (struct pkt_T*)(this->m_localDataList.GetAt(localpos));
	/* 树根 */
	buf.Format(_T("第%d个包："), index + 1);
	HTREEITEM root = this->m_treeCtrl.GetRootItem();
	HTREEITEM data = this->m_treeCtrl.InsertItem(buf, root);
	/* dl层 */
	HTREEITEM frame = this->m_treeCtrl.InsertItem(_T("链路层数据："), data);
	//src mac
	buf.Empty();
	buf.Format(_T("源MAC: "));
	buf.AppendFormat(_T("%02x-%02x-%02x-%02x-%02x-%02x"), package_T->ethh->src[0], package_T->ethh->src[1]
		, package_T->ethh->src[2], package_T->ethh->src[3], package_T->ethh->src[4]
		, package_T->ethh->src[5]);
	this->m_treeCtrl.InsertItem(buf, frame);
	//dst mac
	buf.Empty();
	buf.Format(_T("目的MAC: "));
	buf.AppendFormat(_T("%02x-%02x-%02x-%02x-%02x-%02x"), package_T->ethh->dest[0], package_T->ethh->dest[1]
		, package_T->ethh->dest[2], package_T->ethh->dest[3], package_T->ethh->dest[4]
		, package_T->ethh->dest[5]);
	this->m_treeCtrl.InsertItem(buf, frame);
	//type
	buf.Empty();
	buf.Format(_T("类型：0x%04x"), package_T->ethh->type);
	this->m_treeCtrl.InsertItem(buf, frame);
	/* ip层 */
	switch (package_T->ethh->type)
	{
		/* IPv4 */
	case MACTYPE_IP: {
		HTREEITEM ip = this->m_treeCtrl.InsertItem(_T("IPv4协议头："), data);

		buf.Format(_T("版本号：%d"), package_T->iph->version);
		this->m_treeCtrl.InsertItem(buf, ip);
		buf.Format(_T("IPv4头长：%d*4字节"), package_T->iph->ihl);
		this->m_treeCtrl.InsertItem(buf, ip);
		buf.Format(_T("服务类型：%d"), package_T->iph->tos);
		this->m_treeCtrl.InsertItem(buf, ip);
		buf.Format(_T("IPv4包总长度：%d字节"), package_T->iph->tlen);
		this->m_treeCtrl.InsertItem(buf, ip);
		buf.Format(_T("ID标识：0x%02x"), package_T->iph->id);
		this->m_treeCtrl.InsertItem(buf, ip);
		int mask_frag = 0b11100000;
		buf.Format(_T("标志位：0x%02x"), ((package_T->iph->frag_off) & mask_frag) >> 5);
		//this->m_treeCtrl.InsertItem(buf, ip);
		HTREEITEM ipFlag = this->m_treeCtrl.InsertItem(buf, ip);
		buf = "RSV：0";
		this->m_treeCtrl.InsertItem(buf, ipFlag);
		buf.Format(_T("DF：%d"), ((ntohs(package_T->iph->frag_off) >> 13) & 0x0001));
		this->m_treeCtrl.InsertItem(buf, ipFlag);
		buf.Format(_T("MF：%d"), ((ntohs(package_T->iph->frag_off) >> 14) & 0x0001));
		this->m_treeCtrl.InsertItem(buf, ipFlag);

		buf.Format(_T("段偏移：%d"),
			((((package_T->iph->frag_off) & 0b11111) << 8) + (((package_T->iph->frag_off) >> 8) & 0xff)) << 3);
		this->m_treeCtrl.InsertItem(buf, ip);
		buf.Format(_T("TTL：%d"), package_T->iph->ttl);
		this->m_treeCtrl.InsertItem(buf, ip);
		buf.Format(_T("上层协议号：%d"), package_T->iph->proto);
		this->m_treeCtrl.InsertItem(buf, ip);
		buf.Format(_T("头部校验和：0x%04x"), package_T->iph->check);
		this->m_treeCtrl.InsertItem(buf, ip);

		struct in_addr ip_addr;
		ip_addr.S_un.S_addr = package_T->iph->saddr;
		buf.Format(_T("源IP："));
		buf.AppendFormat(CString(inet_ntoa(ip_addr)));
		this->m_treeCtrl.InsertItem(buf, ip);

		ip_addr.S_un.S_addr = package_T->iph->daddr;
		buf.Format(_T("目的IP："));
		buf.AppendFormat(CString(inet_ntoa(ip_addr)));
		this->m_treeCtrl.InsertItem(buf, ip);

		/* 传输层 */
		const u_char* type = &package_T->iph->proto; //上层协议类型
		switch (*type)
		{
			/* ICMP */
		case 1: {
			HTREEITEM icmp = this->m_treeCtrl.InsertItem(_T("ICMP协议头："), data);
			buf.Format(_T("类型：%d"), package_T->icmph->type);
			this->m_treeCtrl.InsertItem(buf, icmp);
			buf.Format(_T("代码：%d"), package_T->icmph->code);
			this->m_treeCtrl.InsertItem(buf, icmp);
			buf.Format(_T("序号：%d"), package_T->icmph->seq);
			this->m_treeCtrl.InsertItem(buf, icmp);
			buf.Format(_T("校验和：%d"), package_T->icmph->chksum);
			this->m_treeCtrl.InsertItem(buf, icmp);
			break; }
			  /* TCP */
		case 6: {
			HTREEITEM tcp = this->m_treeCtrl.InsertItem(_T("TCP协议头："), data);
			buf.Format(_T("源端口：%d"), package_T->tcph->sport);
			this->m_treeCtrl.InsertItem(buf, tcp);
			buf.Format(_T("目的端口：%d"), package_T->tcph->dport);
			this->m_treeCtrl.InsertItem(buf, tcp);
			buf.Format(_T("序列号：0x%04x"), package_T->tcph->seq);
			this->m_treeCtrl.InsertItem(buf, tcp);
			buf.Format(_T("确认号：0x%04x"), package_T->tcph->ack_seq);
			this->m_treeCtrl.InsertItem(buf, tcp);
			buf.Format(_T("头部长度：%d*4字节"), package_T->tcph->doff);
			this->m_treeCtrl.InsertItem(buf, tcp);

			HTREEITEM flag = this->m_treeCtrl.InsertItem(_T(" 标志位："), tcp);
			buf.Format(_T("cwr窗口拥挤减少 = %d"), package_T->tcph->cwr);
			this->m_treeCtrl.InsertItem(buf, flag);
			buf.Format(_T("ece显式拥塞提醒回应 = %d"), package_T->tcph->ece);
			this->m_treeCtrl.InsertItem(buf, flag);
			buf.Format(_T("urg紧急 = %d"), package_T->tcph->urg);
			this->m_treeCtrl.InsertItem(buf, flag);
			buf.Format(_T("ack应答 = %d"), package_T->tcph->ack);
			this->m_treeCtrl.InsertItem(buf, flag);
			buf.Format(_T("push立即推送 = %d"), package_T->tcph->psh);
			this->m_treeCtrl.InsertItem(buf, flag);
			buf.Format(_T("rst重链接 = %d"), package_T->tcph->rst);
			this->m_treeCtrl.InsertItem(buf, flag);
			buf.Format(_T("syn同步 = %d"), package_T->tcph->syn);
			this->m_treeCtrl.InsertItem(buf, flag);
			buf.Format(_T("fin终止 = %d"), package_T->tcph->fin);
			this->m_treeCtrl.InsertItem(buf, flag);
			buf.Format(_T("窗口大小：%d"), package_T->tcph->window);
			this->m_treeCtrl.InsertItem(buf, tcp);
			buf.Format(_T("报文校验和：0x%04x"), package_T->tcph->check);
			this->m_treeCtrl.InsertItem(buf, tcp);
			buf.Format(_T("紧急指针：%d"), package_T->tcph->urg_ptr);
			this->m_treeCtrl.InsertItem(buf, tcp);
			buf.Format(_T("选项：%d"), package_T->tcph->opt);
			this->m_treeCtrl.InsertItem(buf, tcp);
			break; }
			  /* UDP */
		case 17: {
			HTREEITEM udp = this->m_treeCtrl.InsertItem(_T("UDP协议头："), data);
			buf.Format(_T("源端口：%d"), package_T->udph->sport);
			this->m_treeCtrl.InsertItem(buf, udp);
			buf.Format(_T("目的端口：%d"), package_T->udph->dport);
			this->m_treeCtrl.InsertItem(buf, udp);
			buf.Format(_T("报文长度：%d"), package_T->udph->len);
			this->m_treeCtrl.InsertItem(buf, udp);
			buf.Format(_T("校验和：0x%04x"), package_T->udph->check);
			this->m_treeCtrl.InsertItem(buf, udp);
			break; }
		default: {
			HTREEITEM other = this->m_treeCtrl.InsertItem(_T("IP上层非常见协议："), data);
			buf.Format(_T("协议号：%d"), package_T->iph->proto);
			this->m_treeCtrl.InsertItem(buf, other);
			break; }
		}
		break; }
				   /* ARP */
	case MACTYPE_ARP: {
		HTREEITEM arp = this->m_treeCtrl.InsertItem(_T("ARP协议头："), data);

		buf.Format(_T("硬件接口类型：%d"), package_T->arph->ar_hrd);
		this->m_treeCtrl.InsertItem(buf, arp);
		buf.Format(_T("依附协议类型：0x%04x"), package_T->arph->ar_pro);
		this->m_treeCtrl.InsertItem(buf, arp);
		buf.Format(_T("硬件地址长度：%d*字节"), package_T->arph->ar_hln);
		this->m_treeCtrl.InsertItem(buf, arp);
		buf.Format(_T("协议地址长度：%d*字节"), package_T->arph->ar_pln);
		this->m_treeCtrl.InsertItem(buf, arp);
		if (package_T->arph->ar_op == 1)
		{
			buf.Format(_T("操作码：%d(ARP请求)"), package_T->arph->ar_op);
			this->m_treeCtrl.InsertItem(buf, arp);
		}
		else if (package_T->arph->ar_op == 2)
		{
			buf.Format(_T("操作码：%d(ARP响应)"), package_T->arph->ar_op);
			this->m_treeCtrl.InsertItem(buf, arp);
		}
		else
		{
			buf.Format(_T("操作码：%d"), package_T->arph->ar_op);
			this->m_treeCtrl.InsertItem(buf, arp);
		}
		buf.Format(_T("发送方MAC: "));
		buf.AppendFormat(_T("%02x-%02x-%02x-%02x-%02x-%02x"), package_T->arph->ar_srcmac[0], package_T->arph->ar_srcmac[1]
			, package_T->arph->ar_srcmac[2], package_T->arph->ar_srcmac[3], package_T->arph->ar_srcmac[4]
			, package_T->arph->ar_srcmac[5]);
		this->m_treeCtrl.InsertItem(buf, arp);
		buf.Format(_T("发送方IP: "));
		buf.AppendFormat(_T("%d.%d.%d.%d"), package_T->arph->ar_srcip[0], package_T->arph->ar_srcip[1],
			package_T->arph->ar_srcip[2], package_T->arph->ar_srcip[3]);
		this->m_treeCtrl.InsertItem(buf, arp);
		buf.Format(_T("接收方MAC: "));
		buf.AppendFormat(_T("%02x-%02x-%02x-%02x-%02x-%02x"), package_T->arph->ar_destmac[0], package_T->arph->ar_destmac[1]
			, package_T->arph->ar_destmac[2], package_T->arph->ar_destmac[3], package_T->arph->ar_destmac[4]
			, package_T->arph->ar_destmac[5]);
		this->m_treeCtrl.InsertItem(buf, arp);
		buf.Format(_T("接受方IP: "));
		buf.AppendFormat(_T("%d.%d.%d.%d"), package_T->arph->ar_destip[0], package_T->arph->ar_destip[1],
			package_T->arph->ar_destip[2], package_T->arph->ar_destip[3]);
		this->m_treeCtrl.InsertItem(buf, arp);
		break; }
		/* IPv6 */
	case MACTYPE_IP6: {
		HTREEITEM ip6 = this->m_treeCtrl.InsertItem(_T("IPv6协议头："), data);

		buf.Format(_T("版本号：%d"), package_T->iph6->version);
		this->m_treeCtrl.InsertItem(buf, ip6);
		buf.Format(_T("流服务类型：%d"), package_T->iph6->flowtype);
		this->m_treeCtrl.InsertItem(buf, ip6);
		buf.Format(_T("流标签：%d"), package_T->iph6->flowid);
		this->m_treeCtrl.InsertItem(buf, ip6);
		buf.Format(_T("净荷长度：%d*字节"), package_T->iph6->plen);
		this->m_treeCtrl.InsertItem(buf, ip6);
		buf.Format(_T("扩展头部/上层协议头：0x%02x"), package_T->iph6->nh);
		this->m_treeCtrl.InsertItem(buf, ip6);
		buf.Format(_T("TTL：%d"), package_T->iph6->hlim);
		this->m_treeCtrl.InsertItem(buf, ip6);
		buf.Format(_T("源IPv6："));
		buf.AppendFormat(_T("%x:%x:%x:%x:%x:%x:%x:%x"), package_T->iph6->saddr[0], package_T->iph6->saddr[1],
			package_T->iph6->saddr[2], package_T->iph6->saddr[3], package_T->iph6->saddr[4],
			package_T->iph6->saddr[5], package_T->iph6->saddr[6], package_T->iph6->saddr[7]);
		this->m_treeCtrl.InsertItem(buf, ip6);
		buf.Format(_T("目的IPv6："));
		buf.AppendFormat(_T("%x:%x:%x:%x:%x:%x:%x:%x"), package_T->iph6->daddr[0], package_T->iph6->daddr[1],
			package_T->iph6->daddr[2], package_T->iph6->daddr[3], package_T->iph6->daddr[4],
			package_T->iph6->daddr[5], package_T->iph6->daddr[6], package_T->iph6->daddr[7]);
		this->m_treeCtrl.InsertItem(buf, ip6);
		/* 传输层 */
		switch (package_T->iph6->nh)
		{
			/* TCP */
		case 0x06: {
			HTREEITEM tcp = this->m_treeCtrl.InsertItem(_T("TCP协议头："), data);
			buf.Format(_T("源端口：%d"), package_T->tcph->sport);
			this->m_treeCtrl.InsertItem(buf, tcp);
			buf.Format(_T("源端口：%d"), package_T->tcph->dport);
			this->m_treeCtrl.InsertItem(buf, tcp);
			buf.Format(_T("序列号：%d"), package_T->tcph->seq);
			this->m_treeCtrl.InsertItem(buf, tcp);
			buf.Format(_T("确认号：%d"), package_T->tcph->ack_seq);
			this->m_treeCtrl.InsertItem(buf, tcp);
			buf.Format(_T("头部长度：%d*4字节"), package_T->tcph->doff);
			this->m_treeCtrl.InsertItem(buf, tcp);

			HTREEITEM flag = this->m_treeCtrl.InsertItem(_T(" 标志位："), tcp);
			buf.Format(_T("cwr窗口拥挤减少 = %d"), package_T->tcph->cwr);
			this->m_treeCtrl.InsertItem(buf, flag);
			buf.Format(_T("ece显式拥塞提醒回应 = %d"), package_T->tcph->ece);
			this->m_treeCtrl.InsertItem(buf, flag);
			buf.Format(_T("urg紧急 = %d"), package_T->tcph->urg);
			this->m_treeCtrl.InsertItem(buf, flag);
			buf.Format(_T("ack应答 = %d"), package_T->tcph->ack);
			this->m_treeCtrl.InsertItem(buf, flag);
			buf.Format(_T("push立即推送 = %d"), package_T->tcph->psh);
			this->m_treeCtrl.InsertItem(buf, flag);
			buf.Format(_T("rst重链接 = %d"), package_T->tcph->rst);
			this->m_treeCtrl.InsertItem(buf, flag);
			buf.Format(_T("syn同步 = %d"), package_T->tcph->syn);
			this->m_treeCtrl.InsertItem(buf, flag);
			buf.Format(_T("fin终止 = %d"), package_T->tcph->fin);
			this->m_treeCtrl.InsertItem(buf, flag);

			buf.Format(_T("报文校验和：0x%04x"), package_T->tcph->check);
			this->m_treeCtrl.InsertItem(buf, tcp);
			buf.Format(_T("紧急指针：%d"), package_T->tcph->urg_ptr);
			this->m_treeCtrl.InsertItem(buf, tcp);
			buf.Format(_T("选项：%d"), package_T->tcph->opt);
			this->m_treeCtrl.InsertItem(buf, tcp);
			break; }
				 /* ICMPv6 */
		case 0x3a: {
			HTREEITEM icmp6 = this->m_treeCtrl.InsertItem(_T("ICMPv6协议头："), data);
			buf.Format(_T("类型：%d"), package_T->icmph6->type);
			this->m_treeCtrl.InsertItem(buf, icmp6);
			buf.Format(_T("代码：%d"), package_T->icmph6->code);
			this->m_treeCtrl.InsertItem(buf, icmp6);
			buf.Format(_T("序号：%d"), package_T->icmph6->seq);
			this->m_treeCtrl.InsertItem(buf, icmp6);
			buf.Format(_T("校验和：%d"), package_T->icmph6->chksum);
			this->m_treeCtrl.InsertItem(buf, icmp6);
			buf.Format(_T("选项-类型：%d"), package_T->icmph6->op_type);
			this->m_treeCtrl.InsertItem(buf, icmp6);
			buf.Format(_T("选项-长度%d"), package_T->icmph6->op_len);
			this->m_treeCtrl.InsertItem(buf, icmp6);
			buf.Format(_T("选项-链路层地址："));
			int i;
			for (i = 0; i < 6; i++)
			{
				if (i <= 4)
					buf.AppendFormat(_T("%02x-"), package_T->icmph6->op_ethaddr[i]);
				else
					buf.AppendFormat(_T("%02x"), package_T->icmph6->op_ethaddr[i]);
			}
			this->m_treeCtrl.InsertItem(buf, icmp6);
			break; }
				 /* UDP */
		case 0x11: {
			HTREEITEM udp = this->m_treeCtrl.InsertItem(_T("UDP协议头："), data);
			buf.Format(_T("源端口：%d"), package_T->udph->sport);
			this->m_treeCtrl.InsertItem(buf, udp);
			buf.Format(_T("目的端口：%d"), package_T->udph->dport);
			this->m_treeCtrl.InsertItem(buf, udp);
			buf.Format(_T("报文长度：%d"), package_T->udph->len);
			this->m_treeCtrl.InsertItem(buf, udp);
			buf.Format(_T("校验和：0x%04x"), package_T->udph->check);
			this->m_treeCtrl.InsertItem(buf, udp);
			break; }
		default:
			break;
		}
		break; }
	default:
		this->m_treeCtrl.InsertItem(_T("奇怪的帧，不能解析"), data);
		break;
	}
	return 1;
}

//更新包的详细信息
int CMiniSnifferDlg::updateEdit(int index)
{
	CString buf;
	POSITION localpos = this->m_localDataList.FindIndex(index);
	POSITION netpos = this->m_netDataList.FindIndex(index);
	struct pkt_T* package_T = (struct pkt_T*)(this->m_localDataList.GetAt(localpos));
	u_char* package_data = (u_char*)(this->m_netDataList.GetAt(netpos));
	print_packet_hex(package_data, package_T->len, &buf);
	this->m_edit.SetWindowText(buf); //自适应编码
	return 1;
}

struct pkt_T* FindFstIp(struct pkt_T* tmp, int& index, LPVOID lpParameter);

//重组返回1，否则返回0
bool CMiniSnifferDlg::iprecombine(int index)
{
	POSITION localpos = this->m_localDataList.FindIndex(index);
	struct pkt_T* package_T = (struct pkt_T*)(this->m_localDataList.GetAt(localpos));
	POSITION netpos;
	u_char* pkt_data;
	int i = 0;

	if (package_T->iph == NULL)
	{
		MessageBox(_T("该数据包不是个IPv4包。。。"));
		return 0;
	}

	int flag = (package_T->iph->frag_off & 0b11100000) >> 5;
	int offset = ((((package_T->iph->frag_off) & 0b11111) << 8) + (((package_T->iph->frag_off) >> 8) & 0xff)) << 3;

	if (flag == 0b010)
	{
		MessageBox(_T("该IP包为不可分段IP包"));
		return 0;
	}
	else if (flag == 0 && offset == 0)
	{
		MessageBox(_T("该IP包可分段，但是实际没有分段"));
		return 0;
	}
	else
	{
		//找第一个ip包 以及其Index
		struct pkt_T* fst;
		if ((fst = FindFstIp(package_T, index, this)) == NULL)
		{
			MessageBox(_T("重组失败，有可能是由于没抓全包"));
			return 0;
		}
		flag = 1;
		offset = 0;
		netpos = this->m_netDataList.FindIndex(index);
		pkt_data = (u_char*)this->m_netDataList.GetAt(netpos);

		/* res:=存重组包规则结构体(第一个包的结构体) */
		struct pkt_T* res = fst;
		/* res_data := 存最后data的字符数组 */
		u_char* res_data;
		u_char* tmp;
		int prelen = 0, sumlen = 0;

		//拼接第一个包
		res_data = (u_char*)malloc(fst->len);
		if (res_data)
			memcpy(res_data, pkt_data, fst->len);
		sumlen = fst->len;

		//如果找到了同id && 最后一个分段 && 退出
		while (!(flag == 0 && offset != 0 && res->iph->id == fst->iph->id))
		{

			index++;
			localpos = this->m_localDataList.FindIndex(index);
			fst = (struct pkt_T*)this->m_localDataList.GetAt(localpos);

			//同id需要合并 && 更新flag offset
			if (fst->iph != NULL && fst->iph->id == res->iph->id)
			{
				flag = (fst->iph->frag_off & 0b11100000) >> 5;
				offset = ((((fst->iph->frag_off) & 0b11111) << 8) + (((fst->iph->frag_off) >> 8) & 0xff)) << 3;
				//更新长度 sumlen += iplen - ipheadlen
				prelen = sumlen;
				sumlen += fst->iph->tlen - fst->iph->ihl * 4;
				tmp = (u_char*)malloc(sumlen);

				//获取数据
				netpos = this->m_netDataList.FindIndex(index);
				pkt_data = (u_char*)this->m_netDataList.GetAt(netpos);

				//追加数据
				if (tmp)
					memcpy(tmp, res_data, prelen);
				memcpy(tmp + prelen, pkt_data + 14 + fst->iph->ihl * 4, sumlen - prelen);
				free(res_data);
				res_data = tmp;
			}
			//不同id 直接放过
		}

		//新的ip包长度 = 包总长 - 14
		if (res_data)
		{
			int iplen = sumlen - 14;
			itoa(iplen, (char*)res_data + 14 + 2, 10);
			//memcpy(res_data + 14 + 2, &iplen, 2);
			memset(res_data + 14 + 10, 0, 2);
		}
		updateTree1(res, sumlen - 14);
		updateEdit1(sumlen, res_data);
		updateStatusBar(CString("IP重组成功"), -1, -1);
	}
	return 1;
}
//找第一个Ip包
//特征：flag == 1 && offset == 0 && id == tmp.id
struct pkt_T* FindFstIp(struct pkt_T* tmp, int& index, LPVOID lpParameter)
{
	struct pkt_T* fst = NULL;
	CMiniSnifferDlg* bpthis = (CMiniSnifferDlg*)lpParameter; //this指针

	int flag = (tmp->iph->frag_off & 0b11100000) >> 5;
	int offset = ((((tmp->iph->frag_off) & 0b11111) << 8) + (((tmp->iph->frag_off) >> 8) & 0xff)) << 3;

	if (flag == 1 && offset == 0)
		fst = tmp;
	else
	{
		POSITION localpos;
		POSITION netpos;

		while (!(flag == 1 && offset == 0))
		{
			//上找一个规范包
			index--;
			localpos = bpthis->m_localDataList.FindIndex(index);
			struct pkt_T* tmp1 = (struct pkt_T*)(bpthis->m_localDataList.GetAt(localpos));

			//如果是同id说明是同个分段的，更新tmp,flag,offset
			if (tmp1->iph != NULL && tmp1->iph->id == tmp->iph->id)
			{
				tmp = tmp1;
				flag = (tmp->iph->frag_off & 0b11100000) >> 5;
				offset = ((((tmp->iph->frag_off) & 0b11111) << 8) + (((tmp->iph->frag_off) >> 8) & 0xff)) << 3;
			}

			//如果到头了 而且也不是
			if (bpthis->m_localDataList.GetHead() == tmp1 && !(flag == 1 && offset == 0))
				return NULL;
		}

		fst = tmp;
	}
	return fst;
}

int CMiniSnifferDlg::updateTree1(pkt_T* package_T, int len) //展示重组树
{
	/* 清空树 */
	this->m_treeCtrl.DeleteAllItems();
	/* 初始化 */
	CString buf;
	/* 树根 */
	buf.Format(_T("重组包：包类型为:") + CString(package_T->pktType));
	HTREEITEM root = this->m_treeCtrl.GetRootItem();
	HTREEITEM data = this->m_treeCtrl.InsertItem(buf, root);
	/* dl层 */
	HTREEITEM frame = this->m_treeCtrl.InsertItem(_T("链路层数据："), data);
	//src mac
	buf.Empty();
	buf.Format(_T("源MAC: "));
	buf.AppendFormat(_T("%02x-%02x-%02x-%02x-%02x-%02x"), package_T->ethh->src[0], package_T->ethh->src[1]
		, package_T->ethh->src[2], package_T->ethh->src[3], package_T->ethh->src[4]
		, package_T->ethh->src[5]);
	this->m_treeCtrl.InsertItem(buf, frame);
	//dst mac
	buf.Empty();
	buf.Format(_T("目的MAC: "));
	buf.AppendFormat(_T("%02x-%02x-%02x-%02x-%02x-%02x"), package_T->ethh->dest[0], package_T->ethh->dest[1]
		, package_T->ethh->dest[2], package_T->ethh->dest[3], package_T->ethh->dest[4]
		, package_T->ethh->dest[5]);
	this->m_treeCtrl.InsertItem(buf, frame);
	//type
	buf.Empty();
	buf.Format(_T("类型：0x%04x"), package_T->ethh->type);
	this->m_treeCtrl.InsertItem(buf, frame);
	/* ip层 */

	{
		HTREEITEM ip = this->m_treeCtrl.InsertItem(_T("IPv4协议头："), data);

		buf.Format(_T("版本号：%d"), package_T->iph->version);
		this->m_treeCtrl.InsertItem(buf, ip);
		buf.Format(_T("IPv4头长：%d*4字节"), package_T->iph->ihl);
		this->m_treeCtrl.InsertItem(buf, ip);
		buf.Format(_T("服务类型：%d"), package_T->iph->tos);
		this->m_treeCtrl.InsertItem(buf, ip);
		buf.Format(_T("IPv4包总长度：%d字节"), len);//改为总长度
		this->m_treeCtrl.InsertItem(buf, ip);
		buf.Format(_T("ID标识：0x%02x"), package_T->iph->id);
		this->m_treeCtrl.InsertItem(buf, ip);
		buf.Format(_T("标志位：%d"), 1);			//改为分段标志
		this->m_treeCtrl.InsertItem(buf, ip);
		buf.Format(_T("段偏移：%d"), 0);			//改为无偏移
		this->m_treeCtrl.InsertItem(buf, ip);
		buf.Format(_T("TTL：%d"), package_T->iph->ttl);
		this->m_treeCtrl.InsertItem(buf, ip);
		buf.Format(_T("上层协议号：%d"), package_T->iph->proto);
		this->m_treeCtrl.InsertItem(buf, ip);
		buf.Format(_T("头部校验和：0x0000"));
		this->m_treeCtrl.InsertItem(buf, ip);

		struct in_addr ip_addr;
		ip_addr.S_un.S_addr = package_T->iph->saddr;
		buf.Format(_T("源IP："));
		buf.AppendFormat(CString(inet_ntoa(ip_addr)));
		this->m_treeCtrl.InsertItem(buf, ip);

		ip_addr.S_un.S_addr = package_T->iph->daddr;
		buf.Format(_T("目的IP："));
		buf.AppendFormat(CString(inet_ntoa(ip_addr)));
		this->m_treeCtrl.InsertItem(buf, ip);

		/* 传输层 */
		const u_char* type = &package_T->iph->proto; //上层协议类型
		switch (*type)
		{
			/* ICMP */
		case 1: {
			HTREEITEM icmp = this->m_treeCtrl.InsertItem(_T("ICMP协议头："), data);
			buf.Format(_T("类型：%d"), package_T->icmph->type);
			this->m_treeCtrl.InsertItem(buf, icmp);
			buf.Format(_T("代码：%d"), package_T->icmph->code);
			this->m_treeCtrl.InsertItem(buf, icmp);
			buf.Format(_T("序号：%d"), package_T->icmph->seq);
			this->m_treeCtrl.InsertItem(buf, icmp);
			buf.Format(_T("校验和：0x%04x"), package_T->icmph->chksum);
			this->m_treeCtrl.InsertItem(buf, icmp);
			break; }
			  /* TCP */
		case 6: {
			HTREEITEM tcp = this->m_treeCtrl.InsertItem(_T("TCP协议头："), data);
			buf.Format(_T("源端口：%d"), package_T->tcph->sport);
			this->m_treeCtrl.InsertItem(buf, tcp);
			buf.Format(_T("目的端口：%d"), package_T->tcph->dport);
			this->m_treeCtrl.InsertItem(buf, tcp);
			buf.Format(_T("序列号：0x%04x"), package_T->tcph->seq);
			this->m_treeCtrl.InsertItem(buf, tcp);
			buf.Format(_T("确认号：0x%04x"), package_T->tcph->ack_seq);
			this->m_treeCtrl.InsertItem(buf, tcp);
			buf.Format(_T("头部长度：%d*4字节"), package_T->tcph->doff);
			this->m_treeCtrl.InsertItem(buf, tcp);

			HTREEITEM flag = this->m_treeCtrl.InsertItem(_T(" 标志位："), tcp);
			buf.Format(_T("cwr窗口拥挤减少 = %d"), package_T->tcph->cwr);
			this->m_treeCtrl.InsertItem(buf, flag);
			buf.Format(_T("ece显式拥塞提醒回应 = %d"), package_T->tcph->ece);
			this->m_treeCtrl.InsertItem(buf, flag);
			buf.Format(_T("urg紧急 = %d"), package_T->tcph->urg);
			this->m_treeCtrl.InsertItem(buf, flag);
			buf.Format(_T("ack应答 = %d"), package_T->tcph->ack);
			this->m_treeCtrl.InsertItem(buf, flag);
			buf.Format(_T("push立即推送 = %d"), package_T->tcph->psh);
			this->m_treeCtrl.InsertItem(buf, flag);
			buf.Format(_T("rst重链接 = %d"), package_T->tcph->rst);
			this->m_treeCtrl.InsertItem(buf, flag);
			buf.Format(_T("syn同步 = %d"), package_T->tcph->syn);
			this->m_treeCtrl.InsertItem(buf, flag);
			buf.Format(_T("fin终止 = %d"), package_T->tcph->fin);
			this->m_treeCtrl.InsertItem(buf, flag);
			buf.Format(_T("窗口大小：%d"), package_T->tcph->window);
			this->m_treeCtrl.InsertItem(buf, tcp);
			buf.Format(_T("报文校验和：0x%04x"), package_T->tcph->check);
			this->m_treeCtrl.InsertItem(buf, tcp);
			buf.Format(_T("紧急指针：%d"), package_T->tcph->urg_ptr);
			this->m_treeCtrl.InsertItem(buf, tcp);
			buf.Format(_T("选项：%d"), package_T->tcph->opt);
			this->m_treeCtrl.InsertItem(buf, tcp);
			break; }
			  /* UDP */
		case 17: {
			HTREEITEM udp = this->m_treeCtrl.InsertItem(_T("UDP协议头："), data);
			buf.Format(_T("源端口：%d"), package_T->udph->sport);
			this->m_treeCtrl.InsertItem(buf, udp);
			buf.Format(_T("目的端口：%d"), package_T->udph->dport);
			this->m_treeCtrl.InsertItem(buf, udp);
			buf.Format(_T("报文长度：%d"), package_T->udph->len);
			this->m_treeCtrl.InsertItem(buf, udp);
			buf.Format(_T("校验和：0x%04x"), package_T->udph->check);
			this->m_treeCtrl.InsertItem(buf, udp);
			break; }
		default: {
			HTREEITEM other = this->m_treeCtrl.InsertItem(_T("IP上层非常见协议："), data);
			buf.Format(_T("协议号：%d"), package_T->iph->proto);
			this->m_treeCtrl.InsertItem(buf, other);
			break; }
		}
	}
	/*自动展开树*/
	HTREEITEM tmp = this->m_treeCtrl.GetRootItem();
	this->m_treeCtrl.Expand(tmp, TVE_EXPAND);
	tmp = this->m_treeCtrl.GetNextItem(tmp, TVGN_CHILD);
	while (tmp)
	{
		this->m_treeCtrl.Expand(tmp, TVE_EXPAND);
		tmp = this->m_treeCtrl.GetNextItem(tmp, TVGN_NEXT);
	}
	return 1;
}

int CMiniSnifferDlg::updateEdit1(int len, u_char* pkt_data) //展示重组详细
{
	CString buf;
	print_packet_hex(pkt_data, len, &buf);
	this->m_edit.SetWindowText(buf); //自适应编码
	return 1;
}

/*************************************************************
*
*		菜单栏实现
*
*************************************************************/

//菜单栏 - 文件 - 打开
void CMiniSnifferDlg::OnMenuFileOpen()
{
	// TODO: 在此添加控件通知处理程序代码
	/* 清空window 清空包链表 */
	this->m_listCtrl.DeleteAllItems();
	this->n_pkt = 0;
	this->m_localDataList.RemoveAll();
	this->m_netDataList.RemoveAll();
	memset(&(this->pkcount_T), 0, sizeof(struct pktcount));
	upload = 0;		//初始化
	download = 0;
	GetDlgItem(IDC_EDIT3)->GetWindowTextW(this->b_IP);
	/* 清空窗口 */
	this->m_treeCtrl.DeleteAllItems();
	this->m_edit.SetWindowTextW(_T(""));
	/* 打开文件列表选择文件 */
	CFileDialog FileDlg(TRUE, _T(".pcap"), NULL, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, _T("pcap文件 (*.pcap)|*.pcap|所有文件 (*.*)|*.*||"), NULL);
	FileDlg.m_ofn.lpstrInitialDir = file_dir;
	if (FileDlg.DoModal() == IDOK)
	{
		if (this->readfile(FileDlg.GetPathName()) < 0)
			return;
	}

	m_menu.EnableMenuItem(ID_MENU_FILE_OPEN, MF_ENABLED);	// 启用菜单项"打开"
	m_menu.EnableMenuItem(ID_MENU_FILE_CLOSE, MF_ENABLED);	// 启用菜单项"关闭"
	m_menu.EnableMenuItem(ID_MENU_FILE_SAVEAS, MF_ENABLED);	// 启用菜单项"保存"
	
	CString openFileName = FileDlg.GetFileName();
	CString status = _T("已打开文件：" + openFileName);
	//更新数据包
	lixsniff_updateNPacket();
	updateStatusBar(status, m_listCtrl.GetItemCount(), m_listCtrl.GetItemCount());

}

//菜单栏 - 文件 - 关闭
void CMiniSnifferDlg::OnMenuFileClose()
{
	this->m_listCtrl.DeleteAllItems();
	this->n_pkt = 0;
	this->m_localDataList.RemoveAll();
	this->m_netDataList.RemoveAll();
	this->m_treeCtrl.DeleteAllItems();
	this->m_edit.SetWindowTextW(_T(""));
	upload = 0;		//初始化
	download = 0;
	this->m_edit_up.SetWindowText(_T("0"));
	this->m_edit_down.SetWindowText(_T("0"));
	memset(&(this->pkcount_T), 0, sizeof(struct pktcount));
	m_menu.EnableMenuItem(ID_MENU_FILE_CLOSE, MF_GRAYED);	// 禁用菜单项"关闭"
	m_menu.EnableMenuItem(ID_MENU_FILE_SAVEAS, MF_GRAYED);	// 禁用菜单项"保存"
	//更新数据包
	lixsniff_updateNPacket();
	updateStatusBar(CString("就绪"), 0, 0);
}

//菜单栏 - 文件 - 保存

void CMiniSnifferDlg::OnMenuFileSaveAs()
{
	/* 调用定义的保存函数保存 */
	if (this->savefile() < 0)
		return;
}

//菜单栏 - 文件 - 退出
void CMiniSnifferDlg::OnMenuFileExit()
{
	exit(0);
}

//菜单栏 - 扩展 - IP重组
void CMiniSnifferDlg::OnIpCz()
{
	int index = cursor_index;
	if (index >= this->m_localDataList.GetCount() || index == -1)
		return;
	this->iprecombine(index);
}


//更新统计数据
int CMiniSnifferDlg::lixsniff_updateNPacket()
{
	CString str_num;
	str_num.Format(_T("%d"), this->pkcount_T.n_ip);
	this->m_editNIpv4.SetWindowText(str_num);
	
	str_num.Format(_T("%d"), this->pkcount_T.n_ip6);
	this->m_editNIpv6.SetWindowText(str_num);
	
	str_num.Format(_T("%d"), this->pkcount_T.n_arp);
	this->m_editNArp.SetWindowText(str_num);

	str_num.Format(_T("%d"), this->pkcount_T.n_tcp);
	this->m_editNTcp.SetWindowText(str_num);

	str_num.Format(_T("%d"), this->pkcount_T.n_udp);
	this->m_editNUdp.SetWindowText(str_num);

	str_num.Format(_T("%d"), this->pkcount_T.n_icmp);
	this->m_editNIcmp.SetWindowText(str_num);

	str_num.Format(_T("%d"), this->pkcount_T.n_icmp6);
	this->m_editNIcmpv6.SetWindowText(str_num);

	str_num.Format(_T("%d"), this->pkcount_T.n_other);
	this->m_editNOther.SetWindowText(str_num);

	str_num.Format(_T("%d"), this->pkcount_T.n_sum);
	this->m_editNSum.SetWindowText(str_num);

	return 1;
}
