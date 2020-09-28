//{{TFilterValidator = TFzlNameValidator}}
class TFzlNameValidator : public TFilterValidator
{
public:
	TFzlNameValidator(sign tCyrillic, const char *tErrorMsg);

protected:
	sign cyrillic;	// -1 = concatenated cyrillic
	const char *errorMsg;

//{{TFzlNameValidatorVIRTUAL_BEGIN}}
	virtual void Error(TWindow* owner);
	virtual bool IsValid(const char far* input);
//{{TFzlNameValidatorVIRTUAL_END}}
};  //{{TFzlNameValidator}}
