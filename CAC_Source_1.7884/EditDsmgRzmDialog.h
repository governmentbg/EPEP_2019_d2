//{{TDialog = TEditDsmgRzmDialog}}
class TEditDsmgRzmDialog : public TFzlDialog
{
public:
	TEditDsmgRzmDialog(TWindow* parent, FZL *fzl, TXMLGroup *group, const char *nmrnpr, int resId);

protected:
	TEisAliasFace *vid;
	TMoneyFace *rzm;
	TLongFace *den;
	TLongFace *sdc;
	TLongFace *msc;
	TLongFace *gdn;

	virtual bool IsValid();

//{{TEditDsmgRzmDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TEditDsmgRzmDialogVIRTUAL_END}}

//{{TEditDsmgRzmDialogRSP_TBL_BEGIN}}
	void VidCBNSelChange();
//{{TEditDsmgRzmDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TEditDsmgRzmDialog);
};    //{{TEditDsmgRzmDialog}}
