//{{TFilterValidator = TTransIdValidator}}
class TTransIdValidator : public TFilterValidator
{
public:
	TTransIdValidator(bool tPartial);

protected:
	bool partial;

//{{TTransIdValidatorVIRTUAL_BEGIN}}
	virtual void Error(TWindow* owner);
	virtual bool IsValid(const char far* input);
//{{TTransIdValidatorVIRTUAL_END}}
};    //{{TTransIdValidator}}
