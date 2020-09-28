DEFINE_RESPONSE_TABLE1(TChooseRelationDialog, TScaleDialog)
//{{TChooseRelationDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDC_CHOOSE_RELATION_INDIRECT_AS_ANY, IndirectAsAnyBNClicked),
	EV_BN_CLICKED(IDC_CHOOSE_RELATION_CONSENT_OF_INDIRECT, ConsentOfIndirectBNClicked),
	EV_BN_CLICKED(IDC_CHOOSE_RELATION_INDIRECT_AS_ANY_1, IndirectAsAnyBNClicked_1),
	EV_BN_CLICKED(IDC_CHOOSE_RELATION_INDIRECT_AS_ANY_2, IndirectAsAnyBNClicked_2),
	EV_BN_CLICKED(IDC_CHOOSE_RELATION_INDIRECT_AS_ANY_3, IndirectAsAnyBNClicked_3),
	EV_BN_CLICKED(IDC_CHOOSE_RELATION_NORMAL_BY_INDIRECT, NormalByIndirectBNClicked),
	EV_BN_CLICKED(IDC_CHOOSE_RELATION_AS_WITH_INVOLVEMENT, AsWithInvolvementBNClicked),
//{{TChooseRelationDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TChooseRelationDialog Implementation}}

TChooseRelationDialog::TChooseRelationDialog(TWindow *parent, TSideWind *tOrdinary, TSideWind *tIndirect, int resId)
:
	TScaleDialog(parent, resId), ordinary(tOrdinary), indirect(tIndirect)
{
	if (strchr(SUMMONS_FOR_SIDE_WITH_INVS, indirect->involvement))
	{
		normalByIndirect = new TButton(this, IDC_CHOOSE_RELATION_NORMAL_BY_INDIRECT);
		asWithInvolvement = new TButton(this, IDC_CHOOSE_RELATION_AS_WITH_INVOLVEMENT);
	}
	else
	{
		consentOfIndirect = new TButton(this, IDC_CHOOSE_RELATION_CONSENT_OF_INDIRECT);
		indirectAsAny = new TButton(this, IDC_CHOOSE_RELATION_INDIRECT_AS_ANY);
		indirectAsAny1 = new TButton(this, IDC_CHOOSE_RELATION_INDIRECT_AS_ANY_1);
		indirectAsAny2 = new TButton(this, IDC_CHOOSE_RELATION_INDIRECT_AS_ANY_2);	// 20080221
		indirectAsAny3 = new TButton(this, IDC_CHOOSE_RELATION_INDIRECT_AS_ANY_3);	// 20080221
	}
}


void TChooseRelationDialog::IndirectAsAnyBNClicked()
{
	CloseDialog(IDC_CHOOSE_RELATION_INDIRECT_AS_ANY);
}

void TChooseRelationDialog::IndirectAsAnyBNClicked_1()
{
	CloseDialog(IDC_CHOOSE_RELATION_INDIRECT_AS_ANY_1);
}

void TChooseRelationDialog::IndirectAsAnyBNClicked_2()	// 20080121
{
	CloseDialog(IDC_CHOOSE_RELATION_INDIRECT_AS_ANY_2);
}

void TChooseRelationDialog::IndirectAsAnyBNClicked_3()	// 20080121
{
	CloseDialog(IDC_CHOOSE_RELATION_INDIRECT_AS_ANY_3);
}

void TChooseRelationDialog::ConsentOfIndirectBNClicked()
{
	CloseDialog(IDC_CHOOSE_RELATION_CONSENT_OF_INDIRECT);
}

void TChooseRelationDialog::NormalByIndirectBNClicked()	// 2014:216
{
	CloseDialog(IDC_CHOOSE_RELATION_NORMAL_BY_INDIRECT);
}

void TChooseRelationDialog::AsWithInvolvementBNClicked()	// 2014:216
{
	CloseDialog(IDC_CHOOSE_RELATION_AS_WITH_INVOLVEMENT);
}

void TChooseRelationDialog::SetupWindow()
{
 TScaleDialog::SetupWindow();

 const size_t tmpSz = 512 ;
 const size_t tmpL = tmpSz - 1 ;
 char * const tmp = new char [tmpSz] ;
 TCharPtr CP(tmp);
 size_t l = 0 ;

 tmp [tmpL] = '\0' ;

 if (strchr(SUMMONS_FOR_SIDE_WITH_INVS, indirect->involvement))
 {
  ucnAndUcnTypeToName (ordinary->ucn, ordinary->ucnType, tmp + l, tmpL - l) ;
  l = strlen (tmp) ;
  ostrstream (tmp + l, tmpL - l) << " чрез " << ends ;
  l = strlen (tmp) ;
  involvementToString (indirect->involvement, tmp + l, tmpL - l) ;
  l = strlen (tmp) ;
  ostrstream (tmp + l, tmpL - l) << " " << ends ;
  l = strlen (tmp) ;
  ucnAndUcnTypeToName (indirect->ucn, indirect->ucnType, tmp + l, tmpL - l) ;
  normalByIndirect->SetCaption(tmp);

  l = 0;
  ucnAndUcnTypeToName (indirect->ucn, indirect->ucnType, tmp + l, tmpL - l) ;
  l = strlen (tmp) ;
  ostrstream (tmp + l, tmpL - l) << " като " << ends ;
  l = strlen (tmp) ;
  involvementToString (indirect->involvement, tmp + l, tmpL - l) ;
  l = strlen (tmp) ;
  ostrstream (tmp + l, tmpL - l) << " на " << ends ;
  l = strlen (tmp) ;
  involvementToString (ordinary->involvement, tmp + l, tmpL - l) ;
  l = strlen (tmp) ;
  ostrstream (tmp + l, tmpL - l) << " " << ends ;
  l = strlen (tmp) ;
  ucnAndUcnTypeToName (ordinary->ucn, ordinary->ucnType, tmp + l, tmpL - l) ;
  asWithInvolvement->SetCaption(tmp);
 }
 else
 {
  ucnAndUcnTypeToName (indirect->ucn, indirect->ucnType, tmp + l, tmpL - l) ;
  l = strlen (tmp) ;
  ostrstream (tmp + l, tmpL - l) << " като " << ends ;
  l = strlen (tmp) ;
  involvementToString (indirect->involvement, tmp + l, tmpL - l) ;
  l = strlen (tmp) ;
  ostrstream (tmp + l, tmpL - l) << " на " << ends ;
  l = strlen (tmp) ;
  ucnAndUcnTypeToName (ordinary->ucn, ordinary->ucnType, tmp + l, tmpL - l) ;
  indirectAsAny->SetCaption(tmp);

  l = 0 ;
  ucnAndUcnTypeToName (ordinary->ucn, ordinary->ucnType, tmp + l, tmpL - l) ;
  l = strlen (tmp) ;
  ostrstream (tmp + l, tmpL - l) << " със съгл. на " << ends ;
  l = strlen (tmp) ;
  ucnAndUcnTypeToName (indirect->ucn, indirect->ucnType, tmp + l, tmpL - l) ;
  l = strlen (tmp) ;
  ostrstream (tmp + l, tmpL - l) << " като " << ends ;
  l = strlen (tmp) ;
  involvementToString (indirect->involvement, tmp + l, tmpL - l) ;
  consentOfIndirect->SetCaption(tmp);

  // 20070730 -->
  l = 0;
  ucnAndUcnTypeToName (ordinary->ucn, ordinary->ucnType, tmp + l, tmpL - l) ;
  l = strlen (tmp) ;
  ostrstream (tmp + l, tmpL - l) << ", представляван(а) от " << ends ;
  l = strlen (tmp) ;
  involvementToString (indirect->involvement, tmp + l, tmpL - l) ;
  l = strlen (tmp) ;
  ostrstream (tmp + l, tmpL - l) << " " << ends ;
  l = strlen (tmp) ;
  ucnAndUcnTypeToName (indirect->ucn, indirect->ucnType, tmp + l, tmpL - l) ;
  indirectAsAny1->SetCaption(tmp);

  // 20080221 -->
  l = 0;
  ucnAndUcnTypeToName (ordinary->ucn, ordinary->ucnType, tmp + l, tmpL - l) ;
  l = strlen (tmp) ;
  ostrstream (tmp + l, tmpL - l) << " чрез " << ends ;
  l = strlen (tmp) ;
  involvementToString (indirect->involvement, tmp + l, tmpL - l) ;
  l = strlen (tmp) ;
  ostrstream (tmp + l, tmpL - l) << " " << ends ;
  l = strlen (tmp) ;
  ucnAndUcnTypeToName (indirect->ucn, indirect->ucnType, tmp + l, tmpL - l) ;
  indirectAsAny2->SetCaption(tmp);

  l = 0;
  ucnAndUcnTypeToName (ordinary->ucn, ordinary->ucnType, tmp + l, tmpL - l) ;
  l = strlen (tmp) ;
  ostrstream (tmp + l, tmpL - l) << " със съгласието на " << ends ;
  l = strlen (tmp) ;
  involvementToString (indirect->involvement, tmp + l, tmpL - l) ;
  l = strlen (tmp) ;
  ostrstream (tmp + l, tmpL - l) << " " << ends ;
  l = strlen (tmp) ;
  ucnAndUcnTypeToName (indirect->ucn, indirect->ucnType, tmp + l, tmpL - l) ;
  indirectAsAny3->SetCaption(tmp);
  // 20080221 <--
 }
}
