//{{TDialog = TOutRegSideListDialog}}
class TOutRegSideListDialog : public TSideWindListDialog
{
public:
	TOutRegSideListDialog(TWindow* parent, TOutReg *outReg, TLawsuit *lawsuit, long sideMode);

protected:
	DEFINE_SIDES_TYPE(TOutReg, OutReg)

	virtual void Header2(mstr &m);
	virtual void Accept();
};    //{{TOutRegSideListDialog}}
