// CCImageDoc.cpp : implementation of the CCCImageDoc class
//

#include "stdafx.h"
#include "CCImage.h"

#include "CCImageDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCCImageDoc

IMPLEMENT_DYNCREATE(CCCImageDoc, CDocument)

BEGIN_MESSAGE_MAP(CCCImageDoc, CDocument)
	//{{AFX_MSG_MAP(CCCImageDoc)
	ON_COMMAND(ID_FILE_REOPEN, OnFileReopen)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCCImageDoc construction/destruction

CCCImageDoc::CCCImageDoc()
{	
	// 初始化变量
	m_hDIB = NULL;
	m_palDIB = NULL;
	m_sizeDoc = CSize(1, 1);
	m_strCurrentFile = "";
}

CCCImageDoc::~CCCImageDoc()
{
	// 判断DIB对象是否存在
	if (m_hDIB != NULL)
	{
		// 清除DIB对象
		::GlobalFree((HGLOBAL) m_hDIB);
	}
	
	// 判断调色板是否存在
	if (m_palDIB != NULL)
	{
		// 清除调色板
		delete m_palDIB;
	}
}

BOOL CCCImageDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	m_strCurrentFile = "";

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CCCImageDoc serialization

void CCCImageDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

///////////////////////////////////////////
//	重新打开文档						 //
///////////////////////////////////////////

void CCCImageDoc::OnFileReopen() 
{
 	// 重新打开图像，放弃所有修改
 
	ReopenFile();
}


///////////////////////////////////////////
//	打开文档							 //
///////////////////////////////////////////

BOOL CCCImageDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	CFile file;
	CFileException fe;

	m_strCurrentFile = CString(lpszPathName);

	// 打开文件
	if (!file.Open(lpszPathName, CFile::modeRead | CFile::shareDenyWrite, &fe))
	{
		// 失败
		ReportSaveLoadException(lpszPathName, &fe, FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);
		
		// 返回FALSE
		return FALSE;
	}
	
	// 清除文档类所有的数据
	DeleteContents();
	
	// 更改光标形状
	BeginWaitCursor();
	
	// 尝试调用ReadDIBFile()读取图像，并捕获异常
	TRY
	{
		m_hDIB = m_clsDIB.ReadDIBFile(file);
	}
	CATCH (CFileException, eLoad)
	{
		// 读取失败
		file.Abort();
		
		// 恢复光标形状
		EndWaitCursor();
		
		// 报告失败
		ReportSaveLoadException(lpszPathName, eLoad, FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);
		
		// 设置DIB为空
		m_hDIB = NULL;
		
		// 返回FALSE
		return FALSE;
	}
	END_CATCH
	
	// 初始化DIB
	InitDIBData();
	
	// 恢复光标形状
	EndWaitCursor();
	
	// 判断读取文件是否成功
	if (m_hDIB == NULL)
	{
		// 失败，可能非BMP格式
		CString strMsg;
		strMsg = "读取文件错误. 该文件可能不是位图文件.";
		
		// 提示出错
		MessageBox(NULL, strMsg, "系统警告", MB_ICONINFORMATION | MB_OK);
		
		// 返回FALSE
		return FALSE;
	}
	
	// 将打开文件路径添加到最近使用（MRU）的文件列表
	SetPathName(lpszPathName);
	
	// 初始化胀标记为FALSE
	SetModifiedFlag(FALSE);
	
	// 返回TRUE
	return TRUE;
}


///////////////////////////////////////////
//	保存文档							 //
///////////////////////////////////////////

BOOL CCCImageDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	CFile file;
	CFileException fe;

	m_strCurrentFile = CString(lpszPathName);

	// 打开文件
	if (!file.Open(lpszPathName, CFile::modeCreate | 
		CFile::modeReadWrite | CFile::shareExclusive, &fe))
	{
		// 失败
		ReportSaveLoadException(lpszPathName, &fe, TRUE, AFX_IDP_INVALID_FILENAME);
		
		// 返回FALSE
		return FALSE;
	}

	// 尝试调用SaveDIB保存图像
	BOOL bSuccess = FALSE;
	TRY
	{		
		// 更改光标形状
		BeginWaitCursor();

		// 尝试保存图像
		bSuccess = m_clsDIB.SaveDIB(m_hDIB, file);

		// 关闭文件
		file.Close();
	}			
	CATCH (CException, eSave)
	{
		// 失败
		file.Abort();
		
		// 恢复光标形状
		EndWaitCursor();
		ReportSaveLoadException(lpszPathName, eSave, TRUE, AFX_IDP_FAILED_TO_SAVE_DOC);
		
		// 返回FALSE
		return FALSE;
	}
	END_CATCH

	// 恢复光标形状
	EndWaitCursor();
	
	// 重置脏标记为FALSE
	SetModifiedFlag(FALSE);

	if (!bSuccess)
	{
		// 保存失败，可能是其它格式的DIB，可以读取但是不能保存
		// 或者是SaveDIB函数有误

		CString strMsg;
		strMsg = "Cannot save BMP file！";
		
		// 提示出错
		MessageBox(NULL, strMsg, "系统提示", MB_ICONINFORMATION | MB_OK);
	}
	
	return bSuccess;
}


///////////////////////////////////////////
//	替换DIB，在功能粘贴中用到该函数		 //
///////////////////////////////////////////

void CCCImageDoc::ReplaceHDIB(HDIB hDIB)
{
	// 判断DIB是否为空
	if (m_hDIB != NULL)
	{
		// 非空，则清除
		::GlobalFree((HGLOBAL) m_hDIB);
	}

	// 替换成新的DIB对象
	m_hDIB = hDIB;
}


///////////////////////////////////////////
//	初始化DIB对象						 //
///////////////////////////////////////////

void CCCImageDoc::InitDIBData()
{
	// 判断调色板是否为空
	if (m_palDIB != NULL)
	{
		// 删除调色板对象
		delete m_palDIB;

		// 重置调色板为空
		m_palDIB = NULL;
	}
	
	// 如果DIB对象为空，直接返回
	if (m_hDIB == NULL)
	{
		// 返回
		return;
	}
	
	LPBYTE lpDIB = (LPBYTE) ::GlobalLock((HGLOBAL) m_hDIB);
	
	// 判断图像是否过大（INT_MAX:2,147,483,647）
	if (m_clsDIB.DIBWidth(lpDIB) > INT_MAX || m_clsDIB.DIBHeight(lpDIB) > INT_MAX)
	{
		// 解锁DIB对象
		::GlobalUnlock((HGLOBAL) m_hDIB);
		
		// 释放DIB对象
		::GlobalFree((HGLOBAL) m_hDIB);
		
		// 设置DIB为空
		m_hDIB = NULL;
		
		CString strMsg;
		strMsg = "BMP文件过大! ";
		
		// 提示用户
		MessageBox(NULL, strMsg, "系统警告", MB_ICONINFORMATION | MB_OK);
		
		// 返回
		return;
	}
	
	// 设置文档大小
	m_sizeDoc = CSize((int) m_clsDIB.DIBWidth(lpDIB), (int) m_clsDIB.DIBHeight(lpDIB));
	
	// 解锁DIB对象
	::GlobalUnlock((HGLOBAL) m_hDIB);
	
	// 创建新调色板
	m_palDIB = new CPalette;
	
	// 判断是否创建成功
	if (m_palDIB == NULL)
	{
		// 失败，可能是内存不足
		::GlobalFree((HGLOBAL) m_hDIB);
		
		// 设置DIB对象为空
		m_hDIB = NULL;
		
		// 返回
		return;
	}
	
	// 调用CreateDIBPalette来创建调色板
	if (m_clsDIB.CreateDIBPalette(m_hDIB, m_palDIB) == NULL)
	{
		// 返回空，可能该DIB对象没有调色板
		
		// 删除
		delete m_palDIB;
		
		// 设置为空
		m_palDIB = NULL;
		
		// 返回
		return;
	}
}

void CCCImageDoc::ReopenFile()
{
	// 判断当前图像是否已经被改动
	if (IsModified())
	{
		// 提示用户该操作将丢失所有当前的修改
		if (MessageBox(NULL, "重新打开文件将导致全部修改丢失. 是否继续?", "系统提示", MB_ICONQUESTION | MB_YESNO) == IDNO)
		{
			// 用户取消操作，直接返回
			return;
		}
	}
	
	CFile file;
	CFileException fe;
	
	CString strPathName;
	
	// 获取当前文件路径
	strPathName = GetPathName();
	
	// 重新打开文件
	if (!file.Open(strPathName, CFile::modeRead | CFile::shareDenyWrite, &fe))
	{
		// 失败
		ReportSaveLoadException(strPathName, &fe, FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);
		
		// 返回
		return;
	}
	
	// 更改光标形状
	BeginWaitCursor();
	
	// 尝试调用ReadDIBFile()读取图像
	TRY
	{
		m_hDIB = m_clsDIB.ReadDIBFile(file);
	}
	CATCH (CFileException, eLoad)
	{
		// 读取失败
		file.Abort();
		
		// 恢复光标形状
		EndWaitCursor();
		
		// 报告失败
		ReportSaveLoadException(strPathName, eLoad,	FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);
		
		// 设置DIB为空
		m_hDIB = NULL;
		
		// 返回
		return;
	}
	END_CATCH
		
		// 初始化DIB
		InitDIBData();
	
	// 判断读取文件是否成功
	if (m_hDIB == NULL)
	{
		// 失败，可能非BMP格式
		CString strMsg;
		strMsg = "读取文件错误. 该文件可能不是位图文件.";
		
		// 提示出错
		MessageBox(NULL, strMsg, "系统警告", MB_ICONINFORMATION | MB_OK);
		
		// 恢复光标形状
		EndWaitCursor();
		
		// 返回
		return;
	}
	
	// 初始化脏标记为FALSE
	SetModifiedFlag(FALSE);
	
	// 刷新
	UpdateAllViews(NULL);
	
	// 恢复光标形状
	EndWaitCursor();
}

/////////////////////////////////////////////////////////////////////////////
// CCCImageDoc diagnostics

#ifdef _DEBUG
void CCCImageDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CCCImageDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCCImageDoc commands
