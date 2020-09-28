//{{TFilterValidator = TTimeValidator}}
class TTimeValidator : public TFilterValidator
{
public:
	TTimeValidator(bool tMandatory);

protected:
	bool mandatory;

//{{TTimeValidatorVIRTUAL_BEGIN}}
	virtual void Error(TWindow* owner);
	virtual bool IsValid(const char far* input);
//{{TTimeValidatorVIRTUAL_END}}
};  //{{TTimeValidator}}
