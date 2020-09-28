//{{TDialog = TGetEisppNmrDialog}}
class TGetEisppNmrDialog : public TGroupDialog
{
public:
	TGetEisppNmrDialog(TWindow* parent, TGroup *group, int resId = IDD_GET_EISPP_NMR);

protected:
	TEisNmrFace *eispp_nmr;

	virtual bool IsValid();
};    //{{TGetEisppNmrDialog}}
