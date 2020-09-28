class TRenumberSideWindDialog : public TJoinUXNDialog
{
public:
	TRenumberSideWindDialog(TWindow* parent, TUCNGroup *source, TUCNGroup *target, TRenumberSideLawsuit *lawsuit);

protected:
	TRenumberSideLawsuit *lawsuit;
	TCharAliasFace *type;
	TLongFace *no;
	TYearFace *year;
	TStringFace *fileName;

	virtual bool IsValid();

	void FileNameENUpdate();
	void AttachBNClicked();
DECLARE_RESPONSE_TABLE(TRenumberSideWindDialog);
};
