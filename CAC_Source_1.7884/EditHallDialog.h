//{{TDialog = TEditHallDialog}}
class TEditHallDialog : public TEditDialog
{
public:
	TEditHallDialog(TWindow* parent, THall *tHall, bool edit);

protected:
	bool edit;
	TLongFace *hall;

//{{TJudicialActDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TJudicialActDialogVIRTUAL_END}}
};    //{{TEditHallDialog}}
