#include "cac.h"

#include "GetEisppNmrDialog.h"
#include "GetEisppNmrDialog.cpp"

#include "EisppLawsListBox.h"
#include "EisppLawsListBox.cpp"

#include "EisppLawsListDialog.h"
#include "EisppLawsListDialog.cpp"

void eispp_laws_list(TWindow *parent)
{
	TLawsuit lawsuit;

	if (TGetEisppNmrDialog(parent, &lawsuit).Execute() == IDOK)
		TEisppLawsListDialog(parent, &lawsuit).Execute();
}

#include "CancelEisppDialog.h"
#include "CancelEisppDialog.cpp"

bool eispp_wait(TWindow *parent, const char *nprnmr, const char *xmlName, const char *errName, const char *tmpName)
{
	if (exist(errName) && unlink(errName))
		return error("%s: ������ %d ��� ��������� �� XML ����.", errName, errno);

	if (exist(tmpName) && unlink(tmpName))
		return error("%s: ������ %d ��� ��������� �� �������� ����.", tmpName, errno);

	switch (TCancelEisppDialog(parent, nprnmr, xmlName, errName, tmpName, EISPP_QUERY_TIMEOUT).Execute())
	{
		case IDIGNORE : return error("������ ��� ������������ �� ������� �� %s.", nprnmr);
		case IDRETRY : return error("�� � ������� ������� �� ��������� �� %s.", nprnmr);
		case IDOK : return true;
	}

	return false;
}
