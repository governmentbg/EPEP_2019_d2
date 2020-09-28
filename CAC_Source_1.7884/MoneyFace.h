//{{TEdit = TMoneyFace}}
class TMoneyFace : public TStringFace
{
public:
	TMoneyFace(TWindow* parent, int resourceId, const char *name, uint textLimit);

	double M() const;
	void SetM(double m);

protected:
//{{TMoneyFaceRSP_TBL_BEGIN}}
	void EvChar(uint key, uint repeatCount, uint flags);
//{{TMoneyFaceRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TMoneyFace);
};    //{{TMoneyFace}}
