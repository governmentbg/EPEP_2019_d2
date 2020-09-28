//{{TEdit = TTimeFace}}
class TTimeFace : public TStringFace
{
public:
	TTimeFace(TWindow* parent, int resourceId, const char *name, bool mandatory = true);

	const CTime &Time() const;
	void SetTime(const CTime &tTime);
	void NoZeros() { zeros = false; }

protected:
	mutable CTime time;
	bool zeros;

//{{TTimeFaceRSP_TBL_BEGIN}}
	void EvChar(uint key, uint repeatCount, uint flags);
//{{TTimeFaceRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TTimeFace);
};    //{{TTimeFace}}
