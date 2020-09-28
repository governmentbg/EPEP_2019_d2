//{{TDialog = TEditCitizenDialog}}
class TEditCitizenDialog : public TEditNamedDialog
{
public:
	TEditCitizenDialog(TWindow* parent, TCitizen *citizen, bool edit, bool full, bool tMustIdentify = false);

protected:
	TCheckFace *backup;
	TFindLongAliasFace *rank;	// 2011:031 Long -> FindLong
	TStatic *rankLabel;
	TNameFace *reName;
	TNameFace *family;
	TNameFace *reFamily;
	// 2011:353 for valid checks
	TStringAliasFace *country;
	TStringAliasFace *reCountry;
	TLNCFace *lnc;			// 2012:115
	TOldCitizen oldCitizen;		// 2013:076 FIX: don't rely on unsaved Citizen()
	TCharAliasFace *ucnType;
	bool mustIdentify;		// 2016:252 LPR: must have UCN or LNC

	DEFINE_GROUP_TYPE(TCitizen, Citizen)
	virtual bool IsValid();
	bool CommitBackup();

//{{TEditCitizenDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TEditCitizenDialogVIRTUAL_END}}

//{{TEditCitizenDialogRSP_TBL_BEGIN}}
	void OKBNClicked();
//{{TEditCitizenDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TEditCitizenDialog);
};    //{{TEditCitizenDialog}}