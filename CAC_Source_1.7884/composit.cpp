#include "cac.h"

#include "EditCompositionDialog.h"
#include "EditCompositionDialog.cpp"

#include "CompositionListWindow.h"
#include "CompositionListWindow.cpp"

#include "CompositionListDialog.h"
#include "CompositionListDialog.cpp"

void edit_compositions(TWindow *parent)
{
	log(LOG_NOM, CM_ADMIN_COMPOSITIONS, "състави");
	TCompositionListDialog(parent, COURT_FLAT ? IDD_FLAT_COMPOSITION_LIST : IDD_COMPOSITION_LIST).Execute();

	if (COURT_FLAT)
	{
		Composition->ReloadCache();
		CharComposition->ReloadCache();
	}
}
