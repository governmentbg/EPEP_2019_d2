//{{TFilterValidator = TBulStatValidator}}
class TBulStatValidator : public TFilterValidator
{
public:
	TBulStatValidator();

protected:
//{{TBulStatValidatorVIRTUAL_BEGIN}}
	virtual void Error(TWindow* owner);
	virtual bool IsValid(const char far* input);
//{{TBulStatValidatorVIRTUAL_END}}
};    //{{TBulStatValidator}}
