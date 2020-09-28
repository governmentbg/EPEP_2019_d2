//{{TEdit = TEisNmrFace}}
class TEisNmrFace : public TStringFace
{
public:
	TEisNmrFace(TWindow* parent, int resourceId, const char *name, char letter = EISPP_NPR_KERNEL);

	void SetStartHere(bool tStartHere);
	void SetKinds(char requestKind, char lawsuitKind, bool has_eispp_nmr);

protected:
	bool startHere;

	virtual bool IsValid();
};    //{{TEisNmrFace}}
