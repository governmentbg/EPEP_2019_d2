class TReceiveInstance
{
public:
	TReceiveInstance(char connectType, TOutReg *outReg);

	TConnectKindAliasFace *lawsuitKind;
	TXferKindAliasFace *xferKind;		// 2013:029 LPR/LRQ: +related
	TLongFace *lawsuitNo;
	TYearFace *lawsuitYear;
	TStringAliasFace *court;
	TCharAliasFace *decisionKind;		// 2012:004 LPR/LRQ: +related
	TDateFace *decisionDate;		// 2012:004 LPR/LRQ: +related
	TStringFace *remark;

	void CourtChange(bool setDefaultKind);
	void LawsuitKindChange();
	void ResultChange();
	bool Valid();
	void Try();
	void Save();

protected:
	TConnect connect;
	char lawsuitType;
	bool hasConnect;
};

bool receive_out(TWindow *parent, TOutReg *outReg);
bool edit_outreg(TWindow *parent, TOutReg *outReg);
void return_out(TWindow *parent, TRCDKeyContainer *container, TOriginContainer *origin, bool lower);
void send_nosave(TWindow *parent, TOutReg *outReg, TInReg *inReg, TRCDKeyContainer *event);
void fill_fixeivers(TInReg *inReg, TFixeiver &fixeiver, char *receiver);
void send_appeal(TWindow *parent, TOutReg *outReg, TInReg *inReg);
void send_confhigh(TWindow *parent);
void send_juriconf(TWindow *parent);
void send_other(TWindow *parent, TOutReg *outReg, TRCDKeyGroup *group);
