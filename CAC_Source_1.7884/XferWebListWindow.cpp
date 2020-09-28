//{{TXferWebListWindow Implementation}}

TXferWebListWindow::TXferWebListWindow(TWindow* parent, int resourceId, TXferWebGroup *tWebGroup)
:
	TGroupListWindow(parent, resourceId, NULL, 348), webGroup(tWebGroup)
{
	Add(
		new TGroupListColumn("Трансфер", xferweb_xfer_date, 52, LVCFMT_RIGHT),
		new TGroupListColumn("Вид дело", xfer_xfer_alias, 60),
		new TGroupListColumn("Номер/Година", xfer_no_year, 56, LVCFMT_RIGHT),
		new TGroupListColumn("ДОД", xferweb_limited_short, 24, LVCFMT_CENTER),		
		new TGroupListColumn("Вид акт", xferweb_decision_alias, 60),
		new TGroupListColumn("Номер", "F_DECISION_NO", 32, LVCFMT_RIGHT),
		new TGroupListColumn("Дата", xferweb_decision_date, 52, LVCFMT_RIGHT),
		NULL
	);

	initialRefresh = false;
}

char TXferWebListWindow::XferWebType()
{
	return XferWeb()->type ? XferWeb()->type : kind2type(xferKind2Kind(XferWeb()->key.xferKind));
}

void TXferWebListWindow::Search()
{
	*webGroup << Parent;
	Refresh();
}

void TXferWebListWindow::Query()
{
	TXferWebPrint *xferWeb = XferWeb();
	TRCDKey key;

	key.no = xferWeb->key.no;
	key.year = xferWeb->key.year;
	key.type = XferWebType();

	query_key(this, key);
}

void TXferWebListWindow::View()
{
	TXferWebPrint *xferWeb = XferWeb();
	TWaitWindow wait(this, WAIT);

	if (any(xferWeb->decisionFile))
		THTMLFile::Show(xferWeb->decisionFile, &wait, false);
	if (any(xferWeb->motivesFile))
		THTMLFile::Show(xferWeb->motivesFile, &wait, false);
}

void TXferWebListWindow::Print()
{
	print_webs(webGroup, array);
}

TGroup *TXferWebListWindow::NewGroup() const
{
	return new TXferWebPrint;
}

void TXferWebListWindow::Criteria(msql &m, TGroup *)
{
	webGroup->FormCriteria(m);
}

bool TXferWebListWindow::DeleteGroup(TGroup *group)
{
	TXferWebPrint *xferWeb = (TXferWebPrint *) group;

	if (xferWeb->Delete(true))
	{
		log(LOG_DELETE, CM_ADMIN_QUERY_XFER_WEB, xferWeb->key.no, xferWeb->key.year, XferWebType(),
			xferKind2Kind(xferWeb->decisionKind), NULL, "на съдебен акт за публикуване");
		return true;
	}

	return false;
}
