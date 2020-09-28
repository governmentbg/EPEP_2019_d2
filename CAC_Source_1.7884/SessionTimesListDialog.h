//{{TDialog = TSessionTimesListDialog}}
class TSessionTimesListDialog : public TGroupListDialog
{
public:
	TSessionTimesListDialog(TWindow* parent, TGSArray<TSessionTimes> &tTimes, bool *edited);

protected:
	TSessionTimesListWindow *times;

//{{TSessionTimesListDialogRSP_TBL_BEGIN}}
	void CancelBNClicked();
	void EvClose();
//{{TSessionTimesListDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TSessionTimesListDialog);
};    //{{TSessionTimesListDialog}}
