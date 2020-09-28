//{{TDialog = TAccusationDialog}}
class TAccusationDialog : public TEditDialog
{
public:
	TAccusationDialog(TWindow* parent, TRCDKeyContainer *tContainer, TAccusation *accusation, bool edit);

protected:
	TRCDKeyContainer *container;

	DEFINE_GROUP_TYPE(TAccusation, Accusation)

//{{TAccusationDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TAccusationDialogVIRTUAL_END}}
};    //{{TAccusationDialog}}
