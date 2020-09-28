//{{TFilterValidator = TKeyWordsValidator}}
class TKeyWordsValidator : public TFilterValidator
{
public:
	TKeyWordsValidator();

protected:
//{{TKeyWordsValidatorVIRTUAL_BEGIN}}
	virtual void Error(TWindow* owner);
	virtual bool IsValid(const char far* input);
//{{TKeyWordsValidatorVIRTUAL_END}}
};    //{{TKeyWordsValidator}}
