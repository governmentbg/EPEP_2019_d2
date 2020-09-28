//{{TDialog = TEditSessionTimesDialog}}
class TEditSessionTimesDialog : public TGroupDialog
{
public:
	TEditSessionTimesDialog(TWindow* parent, TSessionTimes *sessionTimes, int tIndex, int tCount);

protected:
	int index, count;
	TDateFace *stardate;
	TTimeFace *startime;
	TTimeFace *entime;
	TLongFace *hours;

	void RecalcHours();

	DEFINE_GROUP_TYPE(TSessionTimes, STimes)
	virtual bool IsValid();

//{{TEditSessionTimesDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TEditSessionTimesDialogVIRTUAL_END}}

//{{TEditSessionTimesDialogRSP_TBL_BEGIN}}
	void StarTimeENUpdate();
	void EnTimeENUpdate();
	void OKBNClicked();
//{{TEditSessionTimesDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TEditSessionTimesDialog);
};    //{{TEditSessionTimesDialog}}
