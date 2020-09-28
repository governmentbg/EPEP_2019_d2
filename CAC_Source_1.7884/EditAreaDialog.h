//{{TDialog = TEditAreaDialog}}
class TEditAreaDialog : public TEditDialog
{
public:
	TEditAreaDialog(TWindow* parent, TArea *tArea, bool edit);

protected:
	TLongFace *area;

//{{TEditAreaDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TEditAreaDialogVIRTUAL_END}}
};    //{{TEditAreaDialog}}
