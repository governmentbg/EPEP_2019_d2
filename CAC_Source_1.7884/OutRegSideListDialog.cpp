//{{TOutRegSideListDialog Implementation}}

TOutRegSideListDialog::TOutRegSideListDialog(TWindow* parent, TOutReg *outReg, TLawsuit *lawsuit, long sideMode)
:
	TSideWindListDialog(parent, outReg, lawsuit, sideMode | SIDE_LIST_KIND | SIDE_LIST_DATE, "���������",
		IDD_OUTREG_SIDE_LIST)
{
	primaries = "����������";
}

void TOutRegSideListDialog::Header2(mstr &m)
{
	if (OutReg()->lawsuit.no)
		m.printf("%s %ld/%ld�.", Type->Seek(OutReg()->lawsuit.type), OutReg()->lawsuit.no, OutReg()->lawsuit.year);

	if (OutReg()->inNo)
	{
		m.sep(", ");
		m.printf("������ ����� %ld/%ld�.", OutReg()->inNo, OutReg()->inYear);
	}
}

void TOutRegSideListDialog::Accept()
{
	eportal_send(this, OutReg(), sideWinds->Lawsuit(), LOG_WRITE);
}
