//{{TFilterValidator = TYearValidator}}
class TYearValidator : public TLongValidator
{
public:
	TYearValidator();

//{{TYearValidatorVIRTUAL_BEGIN}}
	virtual void Error(TWindow* owner);
	virtual bool IsValid(const char far* input);
//{{TYearValidatorVIRTUAL_END}}
};  //{{TYearValidator}}
