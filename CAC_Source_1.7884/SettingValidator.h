//{{TFilterValidator = TSettingValidator}}
class TSettingValidator : public TValidator
{
public:
	TSettingValidator(TSettingAliasFace *tSetting);

protected:
	TSettingAliasFace *setting;

//{{TSettingValidatorVIRTUAL_BEGIN}}
	virtual void Error(TWindow* owner);
	virtual bool IsValid(const char far* str);
	virtual bool IsValidInput(char far* str, bool suppressFill);
//{{TSettingValidatorVIRTUAL_END}}
};  //{{TSettingValidator}}
