//{{TFilterValidator = TMoneyValidator}}
class TMoneyValidator : public TFilterValidator
{
public:
	TMoneyValidator(int nLen);

protected:
	int len;

//{{TMoneyValidatorVIRTUAL_BEGIN}}
	virtual void Error(TWindow* owner);
	virtual bool IsValid(const char far* input);
//{{TMoneyValidatorVIRTUAL_END}}
};    //{{TMoneyValidator}}
