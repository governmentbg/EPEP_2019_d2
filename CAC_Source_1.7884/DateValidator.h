//{{TPXPictureValidator = TDateValidator}}
class TDateValidator : public TPXPictureValidator
{
public:
	TDateValidator(sign tMandatory);

protected:
	sign mandatory;

//{{TDateValidatorVIRTUAL_BEGIN}}
	virtual void Error(TWindow* owner);
	virtual bool IsValid(const char far* input);
//{{TDateValidatorVIRTUAL_END}}
};    //{{TDateValidator}}
