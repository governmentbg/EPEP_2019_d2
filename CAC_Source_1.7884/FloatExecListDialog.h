//{{TDialog = TFloatExecListDialog}}
class TFloatExecListDialog : public TFloatDialog
{
public:
	TFloatExecListDialog(TWindow* parent, TFloatExecListGroup *group, int resId = IDD_FLOAT_EXEC_LIST);

protected:
	TCharListFace *lawKinds;
	TLongFace *minNo;
	TLongFace *maxNo;
	TYearFace *minYear;
	TYearFace *maxYear;
	TCharListFace *compositions;
	TRadioFace *execListAlls;
	TRadioFace *execListJudicials;	
	TRadioFace *execListCountry;
	TRadioFace *execListPrivate;
	TLongFace *minExecListNo;
	TLongFace *maxExecListNo;
	TDateFace *minExecListDate;
	TDateFace *maxExecListDate;
	TCharListFace *sumTypes;
	TAliasListBox *senders;
	TDateFace *minCollectDate;
	TDateFace *maxCollectDate;
	TCheckFace *voluntary;
	TSenderListBox *receivers;
	TDateFace *minSendDate;
	TDateFace *maxSendDate;

	DEFINE_GROUP_TYPE(TFloatExecListGroup, FloatExecListGroup)
	virtual bool IsValid();
	virtual void Action(bool execute);

//{{TFloatExecListDialogRSP_TBL_BEGIN}}
	void ExecListKindBNClicked();
//{{TFloatExecListDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TFloatExecListDialog);
};    //{{TFloatExecListDialog}}
