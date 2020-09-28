//{{TDialog = TSurroundmentListDialog}}
class TSurroundmentListDialog : public TGroupListDialog
{
public:
	TSurroundmentListDialog(TWindow* parent, TSession *tSession, TDecision *decision, TGArray<TSurroundment> *tSurroundments, int resId = IDD_EDIT_SURROUNDMENT);

protected:
	TSession *session;
	TSurroundmentListWindow *surroundments;

//{{TSurroundmentListDialogRSP_TBL_BEGIN}}
	void PrintBNClicked();
//{{TSurroundmentListDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TSurroundmentListDialog);
};    //{{TSurroundmentListDialog}}
