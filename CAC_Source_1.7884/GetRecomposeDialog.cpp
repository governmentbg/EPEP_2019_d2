#include "cac.h"

//{{TGetRecomposeDialog Implementation}}

TGetRecomposeDialog::TGetRecomposeDialog(TWindow* parent, TLawsuit *lawsuit, int resId)
:
	TGroupDialog(parent, lawsuit, resId)
{
	new TCharAliasFace(this, IDC_GET_RECOMPOSE_DOC_TYPE, "F_TYPE", Type, NULL);
	new TLongFace(this, IDC_GET_RECOMPOSE_DOC_NO, "F_NO", SIZE_OF_NO);
	new TYearFace(this, IDC_GET_RECOMPOSE_DOC_YEAR, "F_YEAR");
	oldComposition = new TLongAliasFace(this, IDC_GET_RECOMPOSE_OLD_COMPOSITION, "F_OLD_COMPOSITION", Composition, Default->compositions, FLAG_NULL);
	composition = new TLongAliasFace(this, IDC_GET_RECOMPOSE_COMPOSITION, "F_COMPOSITION", Composition, Default->compositions, FLAG_NULL);
}

void TGetRecomposeDialog::SetupWindow()
{
	TGroupDialog::SetupWindow();
	oldComposition->SetL(Lawsuit()->composition);
}
