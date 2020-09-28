#include "cac.h"

void export_known(TWindow *parent)
{
	TTextFile tf;
	FILE *f;

	if ((f = tf.OpenWrite(parent, OUT_PATH)) != NULL)
	{
		TWaitWindow *wait = new TWaitWindow(parent, WAIT);
		TKnownPerson known;
		msql m(known.Name);
		TCitizen citizen;
		int count = 0;

		m.AddChar("F_UCN_TYPE", UCN_CITIZEN_UCN);
		m.AddChars("F_INVOLVEMENT", INVOLVEMENT_EXPORTS);
		TQuery q(str(m));

		while (q.Read())
		{
			known << q;

			citizen.ucnType = known.ucnType;
			strcpy(citizen.ucn, known.ucn);
			if (citizen.Try())
			{
				fprintf(f,
					"%c%10s%c%5ld%5ld%5ld%-14s%-22s%-15s%-22s\n",
					known.ucnType, known.ucn, known.involvement,
					COURT_AREA, known.flags, known.csjid,
					citizen.name, citizen.family, citizen.reName,
					citizen.reFamily
				);
				count++;
			}
			else
				error("%s %s: липсват данни за имената на лицето.", UCNType->Seek(known.ucnType), known.ucn);
		}

		delete wait;
		tf.Close();
		message("%d експортирани лица.", count);
	}
}

void export_bankrupt(TWindow *parent, TLawsuit *lawsuit, TGArray<TRCDKeyContainer> *events)
{
	TWaitWindow wait(parent, WAIT);
	xfer_bank_export(lawsuit, events);
}

#include "BankruptSendListBox.h"
#include "BankruptSendListBox.cpp"

#include "BankruptSendDialog.h"
#include "BankruptSendDialog.cpp"

void export_bankrupt(TWindow *parent, TLawsuit *lawsuit)
{
	TBankruptSendDialog(parent, lawsuit).Execute();
}

void export_subpoenas(TWindow *parent, const CDate &subpoenaDate, TGArray<TUserAliasGroup> &deliverers)
{
	TTextFile tf;
	FILE *f;
	int leader = 0;

	for (int i = 0; i < deliverers.Count(); i++)
	{
		if (!is_real_ucn(UCN_CITIZEN_UCN, deliverers[i]->ucn))
		{
			leader = i;
			break;
		}
	}

	mstr defname;
	char translit[SIZE_OF_TRANSLIT_DELIVER];

	defname.printf("%ld%02d%02d-%03ld-", subpoenaDate.year, subpoenaDate.month, subpoenaDate.day, COURT_CODE);
	transliterate(deliverers[leader]->NameField->S(), translit, LENGTH_OF_TRANSLIT_DELIVER);
	defname.cat(translit);

	if ((f = tf.OpenWrite(parent, OUT_PATH, str(defname))) != NULL)
	{
		TWaitWindow *wait = new TWaitWindow(parent, WAIT);
		TSubpoena subpoena;
		msql m(subpoena.Name);
		TRCDKey origin;
		int count = 0;

		m.AddDate("F_SUBPOENA_DATE", subpoenaDate);
		m.AddArray("F_UCN", deliverers, "F_DELIVERER_UCN");
		m.AddIsNull("F_UCLP", false);
		m.AddChar("NOT F_MULTI", SUBPOENA_MULTI);		// 2012:076 TRQ/LRQ
		m.AddIsNull("F_SUBPOENA_RECEIVED", true);
		m.AddNonElectron("");

		TQuery q(str(m));
		while (q.Read())
		{
			subpoena << q;
			SubpoenaKind->Seek(subpoena.subpoenaKind);

			// 2014:077 LPR: FindOrigin now returns INREG/OUTREG, so check for ORIGINS
			if (SubpoenaKind->genericKind && subpoena.FindOrigin(origin) && strchr(TYPE_ORIGINS, origin.type))
			{
				if (!subpoena.requestId)
				{
					subpoena.requestId = subpoena.GenerateRequestId(subpoena.DataBase);

					if (!subpoena.Update())
						continue;
				}

				fprintf(f, "%03d%05d%04d%c%c%05d%05d%-7s", COURT_CODE, origin.no, origin.year,
					type2ConnectKind(origin.type), SubpoenaKind->genericKind, subpoena.uclp,
					subpoena.street > 0 ? subpoena.street : 0, subpoena.adrNo);

				if (strchr(KIND_OPEN_SESSIONS, subpoena.kind) && subpoena.eventStatus == RESULT_FIXED)
					fprintf(f, "%#0D", &subpoena.date);
				else
					fprintf(f, "%#0D", &CDate());

				fprintf(f, "%010d%-1s%-2s%-2s%-3s%-5s", subpoena.sideNo, subpoena.subNo, subpoena.entrance,
					subpoena.floor, subpoena.apartment, subpoena.blockNo);

				m.clear();
				subpoena.PrintNotifieds(m, ", ", " ");
				m.cut(100);
				fprintf(f, "%-100s", str(m));
				// 2015:085 new format
				fprintf(f, "%10s%010d", atob(subpoena.delivererUcn) ? subpoena.delivererUcn : EMPTY,
					subpoena.requestId);
				fputc('\n', f);
				count++;
			}
		}

		delete wait;
		tf.Close();
		message("%d експортирани призовки/съобщения.", count);
	}
}

void export_announces(TWindow *parent, const CDate &subpoenaReturned, long court)
{
	TTextFile tf;
	FILE *f;

	mstr defname;
	defname.printf("%ld%02d%02d-%03ld-RETBACK", subpoenaReturned.year, subpoenaReturned.month, subpoenaReturned.day, court);

	if ((f = tf.OpenWrite(parent, OUT_PATH, str(defname))) != NULL)
	{
		TWaitWindow *wait = new TWaitWindow(parent, WAIT);
		TAnnounce announce;
		msql m(announce.Name);
		int count = 0;

		m.AddDate("F_SUBPOENA_RETURNED", subpoenaReturned);
		m.AddLong("F_COURT", court);
		m.AddIsNull("F_SUBPOENA_RECEIVED", false);
		m.AddIsNull("F_REQUEST_ID", false);

		TQuery q(str(m));
		while (q.Read())
		{
			announce << q;
			strrep(announce.receivedText, '\r', '\a');
			strrep(announce.receivedText, '\n', '\b');
			fprintf(f, "%03ld%010ld%#0D%#0T%c%-250s%#0D%05d%05d\n",
				court, announce.requestId, &announce.subpoenaReceived, &announce.receivedTime,
				announce.receivedStatus, announce.receivedText, &announce.subpoenaReturned,
				announce.key.no, announce.sideNo);
			count++;
		}

		delete wait;
		tf.Close();
		message("%d експортирани призовки/съобщения.", count);
	}
}
