//{{TDialog = TSessionTimesDialog}}
class TSessionTimesDialog : public TSessionCompositionDialog
{
public:
	TSessionTimesDialog(TWindow* parent, TBaseSession *session, TLawsuit *lawsuit, long tEdit, int resId);

protected:
	// 2012:257 +related, from open
	TTimeFace *startime;		// 2016:281 == F_TIME
	TTimeFace *entime;
	TDateFace *end;			// 2017:073 from open
	TPushButton *times;		// 2016:286
	sign singleDate;			// 2016:287
	bool initialized;			// 2016:288
	TCharAliasFace *result;		// 2017:074 from result
	// 2016:281
	bool SessionTimes() const;	// 2017:074 non-virtual
	TStatic *hoursLabel;
	TLongFace *hours;
	TSesTimesList sesTimesList;
	// 2016:286
	bool CheckSessionTimes() const;
	virtual bool SingleDate() const;
	void RecalcHours();
	void CommitTimes();
	// 2017:074
	sign sessionTimes;		// to watch changes
	void SingleDateChanged(bool force = false);
	void SessionTimesChanged();

	virtual bool IsValid();
	virtual bool IsClean();		// 2016:288

//{{TSessionTimesDialogRSP_TBL_BEGIN}}
	void JurorCBNSelChange();
	void StarTimeENUpdate();
	void EnTimeENUpdate();
	void TimesBNClicked();
	void EndENUpdate();
//{{TSessionTimesDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TSessionTimesDialog);
};    //{{TSessionTimesDialog}}
