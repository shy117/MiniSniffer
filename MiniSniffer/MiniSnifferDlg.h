
// MiniSnifferDlg.h: 头文件
//

#pragma once
#include"pcap.h"
#include"protocol.h"
#include"util.h"


// CMiniSnifferDlg 对话框
class CMiniSnifferDlg : public CDialogEx
{
// 构造
public:
	CMiniSnifferDlg(CWnd* pParent = nullptr);	// 标准构造函数

	/* 实现功能funcs */
	bool initCap();		//初始化
	int start();		//开始抓包
	int updateTree(int index);	//展开解析树
	int updateEdit(int index);	//展开详细信息
	int savefile();
	int readfile(CString fpath);
	bool iprecombine(int index);//重组
	int updateTree1(pkt_T* pakeage_T, int len); //展示重组树
	int updateEdit1(int len, u_char* pkt_data); //展示重组详细

	/* data */
	char errbuf[PCAP_ERRBUF_SIZE];//内置errorbuf
	int n_dev;					//网卡数
	pcap_if_t* alldev;			//所有网卡
	pcap_if_t* dev;				//选定的网卡
	pcap_t* handle;				//pcap 创建的【捕获句柄】
	CString filter;				//filter
	int n_pkt;				    //抓包数
	struct pktcount pkcount_T;	// 各类包计数结构体

	pcap_dumper_t* myfile;		//存储的文件
	char filepath[512];
	char filename[512];

	CPtrList pk_list;			//抓包链表
	CPtrList m_localDataList;	//pkt_T链表，存储规范化网络包
	CPtrList m_netDataList;		//char*链表，存储网络包数据

	HANDLE m_threadhandle;		//线程

	int upload;					//上传
	int	download;				//下载

	CString file_dir = _T("H:\\Workspace\\VS_MFC\\MiniSniffer\\MiniSniffer\\SavedDir\\");
	CString b_IP = _T("192.168.31.11");// _T("10.5.81.173");

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MINISNIFFER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	/*控件*/
	HICON m_hIcon;
	CMenu m_menu;
	

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()


	/* 初始化相关函数 */
	void initialMenuBar();				// 菜单栏初始化
	bool initialComboBoxDevList();		// 网卡列表初始化
	void initialListCtrlPacketList();	// 列表控件（数据包列表）初始化
	void initialStatusBar();			// 状态栏初始化

	void updateStatusBar(const CString& status, int pktTotalNum, int pktDisplayNum); // 更新状态栏
	int lixsniff_updateNPacket();		// 更新统计数据

private:
	int cursor_index = -1;
public:
	//各控件
	CListCtrl m_listCtrl;	//包列表
	CComboBox m_comboBox;	//网卡选择控件
	CTreeCtrl m_treeCtrl;	//包解析
	CEdit m_edit;			//包详细
	CButton m_buttonStart;	//开始按钮
	CButton m_buttonStop;	//结束
	CStatusBar  m_statusBar;
	CEdit m_edit_up;
	CEdit m_edit_down;
	CEdit m_edit_IP;
	CEdit m_editNIpv4;
	CEdit m_editNIpv6;
	CEdit m_editNArp;
	CEdit m_editNTcp;
	CEdit m_editNUdp;
	CEdit m_editNIcmp;
	CEdit m_editNIcmpv6;
	CEdit m_editNOther;
	CEdit m_editNSum;

	//触发函数
	afx_msg void OnBnClickedButton1();	//开始
	afx_msg void OnBnClickedButton2();	//结束
	afx_msg void OnLvnItemchangedList2(NMHDR* pNMHDR, LRESULT* pResult);//包列表
	afx_msg void OnNMCustomdrawList2(NMHDR* pNMHDR, LRESULT* pResult);	//染色
	afx_msg void OnBnClickedButton5();	//过滤器设置
	afx_msg void OnMenuFileOpen();		//保存
	afx_msg void OnMenuFileClose();		//关闭
	afx_msg void OnMenuFileSaveAs();	//保存
	afx_msg void OnMenuFileExit();		//退出
	afx_msg void OnIpCz();				//IP重组
};
