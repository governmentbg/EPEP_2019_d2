//{{TDialog = TPrinterDialog}}
class TPrinterDialog : public TGroupDialog
{
public:
	TPrinterDialog(TWindow* parent, TPrinterGroup *group, int resId = IDD_PRINTER);

protected:
	TCheckFace *windows;
	TLongFace *pagelen;

	TLongFace *port;
	TCheckFace *cp1251;
	TCheckFace *check;
	TCheckFace *pseudo;

	TCheckFace *setupex;
	TLongFace *justify;
	TLongFace *alignto;
	TLongFace *linewidth;

	virtual bool IsValid();

//{{TPrinterDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TPrinterDialogVIRTUAL_END}}

//{{TPrinterDialogRSP_TBL_BEGIN}}
	void OKBNClicked();
	void WindowsBNClicked();
//{{TPrinterDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TPrinterDialog);
};    //{{TPrinterDialog}}
