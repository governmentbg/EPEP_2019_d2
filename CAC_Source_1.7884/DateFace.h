//{{TEdit = TDateFace}}
class TDateFace : public TStringFace
{
public:
	TDateFace(TWindow* parent, int resourceId, const char *name, sign mandatory = true);

	virtual bool IsValid(bool reportErr = false);

	const CDate &Date() const;
	void SetDate(const CDate &tDate);

protected:
	mutable CDate date;

//{{TDateFaceVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TDateFaceVIRTUAL_END}}

//{{TDateFaceRSP_TBL_BEGIN}}
	long EvFaceValid(TParam1, TParam2);
	void EvChar(uint key, uint repeatCount, uint flags);
//{{TDateFaceRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TDateFace);
};  //{{TDateFace}}
