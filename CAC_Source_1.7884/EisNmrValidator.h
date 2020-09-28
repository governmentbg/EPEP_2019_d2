//{{TPXPictureValidator = TEisNmrValidator}}
class TEisNmrValidator : public TPXPictureValidator
{
public:
	TEisNmrValidator(char letter);

protected:
	char letter;

//{{TEisNmrValidatorVIRTUAL_BEGIN}}
	virtual void Error(TWindow* owner);
	virtual bool IsValid(const char far* input);
//{{TEisNmrValidatorVIRTUAL_END}}
};    //{{TEisNmrValidator}}
