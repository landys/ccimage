; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CDlgHistography
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "CCImage.h"
LastPage=0

ClassCount=10
Class1=CCCImageApp
Class2=CCCImageDoc
Class3=CCCImageView
Class4=CMainFrame

ResourceCount=7
Resource1=IDD_DLG_HISTOGRAPHY
Resource2=IDD_DLG_SETANGLE
Class5=CChildFrame
Class6=CAboutDlg
Resource3=IDR_MAINFRAME
Resource4=IDD_ABOUTBOX
Resource5=IDD_DLG_GEOMETRYZOOM
Class7=CDlgGeomZoom
Resource6=IDR_CCIMAGTYPE
Class8=CDlgSetAngle
Class9=CDlgSetOffset
Class10=CDlgHistography
Resource7=IDD_DLG_TRANSLATION

[CLS:CCCImageApp]
Type=0
HeaderFile=CCImage.h
ImplementationFile=CCImage.cpp
Filter=N

[CLS:CCCImageDoc]
Type=0
HeaderFile=CCImageDoc.h
ImplementationFile=CCImageDoc.cpp
Filter=N

[CLS:CCCImageView]
Type=0
HeaderFile=CCImageView.h
ImplementationFile=CCImageView.cpp
Filter=C
LastObject=CCCImageView
BaseClass=CScrollView
VirtualFilter=VWC


[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T


[CLS:CChildFrame]
Type=0
HeaderFile=ChildFrm.h
ImplementationFile=ChildFrm.cpp
Filter=M


[CLS:CAboutDlg]
Type=0
HeaderFile=CCImage.cpp
ImplementationFile=CCImage.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=?
ControlCount=5
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Control5=IDC_STATIC,static,1342308352

[MNU:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_PRINT_SETUP
Command4=ID_FILE_MRU_FILE1
Command5=ID_APP_EXIT
Command6=ID_VIEW_TOOLBAR
Command7=ID_VIEW_STATUS_BAR
Command8=ID_APP_ABOUT
CommandCount=8

[TB:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_COPY
Command5=ID_EDIT_PASTE
Command6=ID_FILE_PRINT
Command7=ID_APP_ABOUT
CommandCount=7

[MNU:IDR_CCIMAGTYPE]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_REOPEN
Command4=ID_FILE_CLOSE
Command5=ID_FILE_SAVE
Command6=ID_FILE_SAVE_AS
Command7=ID_FILE_PRINT
Command8=ID_FILE_PRINT_PREVIEW
Command9=ID_FILE_PRINT_SETUP
Command10=ID_FILE_MRU_FILE1
Command11=ID_APP_EXIT
Command12=ID_EDIT_COPY
Command13=ID_EDIT_PASTE
Command14=ID_VIEW_TOOLBAR
Command15=ID_VIEW_STATUS_BAR
Command16=ID_GEOMETRY_ENLARGE
Command17=ID_GEOMETRY_LINEARENLARGE
Command18=ID_GEOMETRY_TRANSLATE
Command19=ID_GEOMETRY_ROTATE
Command20=ID_GEOMETRY_LINEARROTATE
Command21=ID_GEOMETRY_HORIMIRROR
Command22=ID_GEOMETRY_VERTMIRROR
Command23=ID_HISTOGRAPHY
Command24=ID_HISTOEQU
Command25=ID_ORTHO_FFT
Command26=ID_FILTERS_SMOOTH
Command27=ID_FILTERS_MEDIAN
Command28=ID_FILTERS_SHARP
Command29=ID_WINDOW_NEW
Command30=ID_WINDOW_CASCADE
Command31=ID_WINDOW_TILE_HORZ
Command32=ID_WINDOW_ARRANGE
Command33=ID_APP_ABOUT
CommandCount=33

[ACL:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_PRINT
Command5=ID_EDIT_UNDO
Command6=ID_EDIT_CUT
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_UNDO
Command10=ID_EDIT_CUT
Command11=ID_EDIT_COPY
Command12=ID_EDIT_PASTE
Command13=ID_NEXT_PANE
Command14=ID_PREV_PANE
CommandCount=14

[DLG:IDD_DLG_GEOMETRYZOOM]
Type=1
Class=CDlgGeomZoom
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDC_CVALUE,edit,1350631552
Control3=IDC_STATIC,static,1342308352

[CLS:CDlgGeomZoom]
Type=0
HeaderFile=DlgGeomZoom.h
ImplementationFile=DlgGeomZoom.cpp
BaseClass=CDialog
Filter=D
LastObject=ID_GEOMETRY_TRANSLATE
VirtualFilter=dWC

[DLG:IDD_DLG_SETANGLE]
Type=1
Class=CDlgSetAngle
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDC_PARAM,edit,1350631552
Control3=IDC_STATIC,static,1342308352

[CLS:CDlgSetAngle]
Type=0
HeaderFile=DlgSetAngle.h
ImplementationFile=DlgSetAngle.cpp
BaseClass=CDialog
Filter=D
LastObject=CDlgSetAngle
VirtualFilter=dWC

[DLG:IDD_DLG_TRANSLATION]
Type=1
Class=CDlgSetOffset
ControlCount=5
Control1=IDOK,button,1342242817
Control2=IDC_XOFFSET,edit,1350631552
Control3=IDC_YOFFSET,edit,1350631552
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,static,1342308352

[CLS:CDlgSetOffset]
Type=0
HeaderFile=DlgSetOffset.h
ImplementationFile=DlgSetOffset.cpp
BaseClass=CDialog
Filter=D
LastObject=ID_INTENSITY_BALANCE
VirtualFilter=dWC

[DLG:IDD_DLG_HISTOGRAPHY]
Type=1
Class=CDlgHistography
ControlCount=1
Control1=IDC_COORD,static,1342308352

[CLS:CDlgHistography]
Type=0
HeaderFile=DlgHistography.h
ImplementationFile=DlgHistography.cpp
BaseClass=CDialog
Filter=D
LastObject=CDlgHistography
VirtualFilter=dWC

