//{{TFilterValidator = TIndexValidator}}
class TIndexValidator : public TLongValidator
{
public:
	TIndexValidator(const char *tKinds);

protected:
	const char *kinds;	

//{{TIndexValidatorVIRTUAL_BEGIN}}
	virtual void Error(TWindow* owner);
	virtual bool IsValid(const char far* input);
//{{TIndexValidatorVIRTUAL_END}}
};    //{{TIndexValidator}}
