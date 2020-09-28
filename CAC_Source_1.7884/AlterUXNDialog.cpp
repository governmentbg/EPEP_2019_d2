#include "cac.h"

//{{TAlterUXNDialog Implementation}}

TAlterUXNDialog::TAlterUXNDialog(TWindow* parent, TUCNGroup *tSource, TUCNGroup *tTarget, const char *sources,
	const char *targets, int resId)
:
	TQuickDialog(parent, resId), source(tSource), target(tTarget)
{
	sourceType = new TCharAliasFace(this, IDC_ALTER_UXN_SOURCE_TYPE, "F_SOURCE_TYPE", UCNType, sources);
	sourceCode = new TUCNFace(this, IDC_ALTER_UXN_SOURCE_CODE, "F_SOURCE_CODE", sourceType);
	targetType = new TCharAliasFace(this, IDC_ALTER_UXN_TARGET_TYPE, "F_TARGET_TYPE", UCNType, targets);
	targetCode = new TUCNFace(this, IDC_ALTER_UXN_TARGET_CODE, "F_TARGET_CODE", targetType);
}

bool TAlterUXNDialog::IsValid()
{
	bool isValid = TQuickDialog::IsValid();

	if (isValid)
	{
		source->ucnType = sourceType->C();
		strcpy(source->ucn, sourceCode->S());
		target->ucnType = targetType->C();
		strcpy(target->ucn, targetCode->S());

		if (!any(source->ucn) || !any(target->ucn))
			isValid = error("Кодовете на лицата са задължителни.");
		else if (source->ucnType == target->ucnType && !strcmp(source->ucn, target->ucn))
			isValid = error("Избрано е едно и също лице.");
	}

	return isValid;
}
