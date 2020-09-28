//{{TFilterValidator = TNameValidator}}
class TNameValidator : public TFilterValidator
{
public:
	TNameValidator(const TCharSet& validChars = "�-�A-Z-");

//{{TNameValidatorVIRTUAL_BEGIN}}
	virtual void Error(TWindow* owner);
	virtual bool IsValid(const char far* input);
//{{TNameValidatorVIRTUAL_END}}
};  //{{TNameValidator}}
