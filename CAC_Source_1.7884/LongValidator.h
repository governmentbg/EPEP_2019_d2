//{{TFilterValidator = TLongValidator}}
class TLongValidator : public TFilterValidator
{
public:
	TLongValidator(bool tPositive = true);

	void SetPositive(bool tPositive);

protected:
	bool positive;	// 2008:135 LPR: for precise disabled check

//{{TLongValidatorVIRTUAL_BEGIN}}
	virtual void Error(TWindow* owner);
	virtual bool IsValid(const char far* input);
//{{TLongValidatorVIRTUAL_END}}
};  //{{TLongValidator}}
