//{{TPXPictureValidator = TLNCValidator}}
class TLNCValidator : public TPXPictureValidator
{
public:
	TLNCValidator();

protected:
//{{TLNCValidatorVIRTUAL_BEGIN}}
	virtual void Error(TWindow* owner);
	virtual bool IsValid(const char far* input);
//{{TLNCValidatorVIRTUAL_END}}
};    //{{TLNCValidator}}
