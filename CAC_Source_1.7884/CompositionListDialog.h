//{{TDialog = TCompositionListDialog}}
class TCompositionListDialog : public TGroupListDialog
{
public:
	TCompositionListDialog(TWindow* parent, int resId);

protected:
	TCharAliasFace *college;
	TCompositionListWindow *compositions;
	TButton *active;

	void ActiveChanged();
	
//{{TCompositionListDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TCompositionListDialogVIRTUAL_END}}

//{{TCompositionListDialogRSP_TBL_BEGIN}}
	void CollegeCBNSelChange();
	void CompositionsLVNItemChanged(TLwNotify &lwn);
	void PrintBNClicked();
	void ActiveBNClicked();
//{{TCompositionListDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TCompositionListDialog);
};    //{{TCompositionListDialog}}
