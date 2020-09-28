//{{TDialog = TServeAnnounceDialog}}
class TServeAnnounceDialog : public TGroupDialog
{
public:
	TServeAnnounceDialog(TWindow *parent, TAnnounce *announce, TRCDKeyContainer *tContainer, const char *tScheds,
		const char *tStatuses, sign tAlready);

protected:
	TStatic *header;
	TCharAliasFace *receivedStatus;
	TDateFace *received;
	TTimeFace *receivedTime;
	TDateFaceUpDown *receivedUpDn;
	TRadioFace *schedNo;
	TRadioFace *schedYes;
	TRadioFace *sched7;
	TRadioFace *sched15;
	TStringFace *receivedText;
	TDateFace *returned;
	TDateFaceUpDown *returnedUpDn;
	TSubpoena *subpoena;
	TRCDKeyContainer *container;
	const char *scheds;
	const char *statuses;
	TCheckBox *official;
	TStringFace *imageName;
	TButton *attach;
	sign already;
	TStringFace *barcodeText; 

	DEFINE_GROUP_TYPE(TAnnounce, Announce)
	virtual bool IsValid();

//{{TServeAnnounceDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TServeAnnounceDialogVIRTUAL_END}}

//{{TServeAnnounceDialogRSP_TBL_BEGIN}}
	void ReceivedStatusCBNSelChange();
	void OKBNClicked();
	void OfficialBNClicked();
	void AttachBNClicked();
//{{TServeAnnounceDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TServeAnnounceDialog);
};    //{{TServeAnnounceDialog}}
