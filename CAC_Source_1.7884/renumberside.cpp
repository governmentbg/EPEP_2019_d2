#include "cac.h"

static int renumber_sideWind(THTMLFile &h, TUCNGroup &source, TUCNGroup &target, const TRCDKey &key)
{
	int count = 0;

	for (const TWindIndex *wi = WIND_INDEXES; wi->name; wi++)
	{
		TBaseWind baseWind(wi->name, NULL);
		msql m(baseWind.Name, MQ_UPDATE);

		baseWind << target;
		baseWind.Quote(m, "F_UCN, F_UCN_TYPE");
		m.Begin(baseWind.Name, MQ_SUBSEQ);
		m.AddLong("F_NO", key.no);
		m.AddLong("F_YEAR", key.year);
		m.AddChar("F_TYPE", key.type);
		baseWind << source;
		m.Add(&baseWind, "F_UCN, F_UCN_TYPE");

		int result = AppName->ExecCount(str(m), ET_SILENT);

		if (result >= 0)
			count += result;
		else
		{
			h.printf("%s %s %ld/%ld: грешка при преномериране на страна\n", baseWind.Name, Type->Seek(key.type),
				key.no, key.year);
		}
	}

	return count;
}

static int renumber_sideWinds(THTMLFile &h, TUCNGroup &source, TUCNGroup &target, const TRCDKey &key)
{
	static constant RCDKEY_FIELDS = "F_NO, F_YEAR, F_TYPE";
	TSurround surround;
	TInReg inReg;
	TOutReg outReg;
	TGArray<TRCDKeyContainer> array;

	int count = renumber_sideWind(h, source, target, key);

	surround.lawsuit = key;
	surround.LoadFloat(array, RCDKEY_FIELDS);
	inReg.lawsuit = key;
	inReg.LoadFloat(array, RCDKEY_FIELDS);
	outReg.lawsuit = key;
	outReg.LoadFloat(array, RCDKEY_FIELDS);

	for (int i = 0; i < array.Count(); i++)
		count += renumber_sideWind(h, source, target, array[i]->key);

	return count;
}

static void renumber_sideWind(TWindow *parent, THTMLFile &h, TUCNGroup &source, TUCNGroup &target, TGArray<TLawsuit> &lawsuits)
{
	TWaitWindow wait(parent, WAIT);

	for (int i = 0; i < lawsuits.Count(); i++)
	{
		const TLawsuit *lawsuit = lawsuits[i];

		int count = renumber_sideWinds(h, source, target, lawsuit->key);

		if (lawsuit->source.type == TYPE_REQUEST)
			count += renumber_sideWinds(h, source, target, lawsuit->source);

		h.printf("%s %ld/%ld: %d преномерирани страни\n", Type->Seek(lawsuit->key.type), lawsuit->key.no,
			lawsuit->key.year, count);
	}

	h.Show(&wait, false);
}

class TRenumberSideLawsuit : public TLawsuit
{
public:
	TRenumberSideLawsuit();

	char fileName[SIZE_OF_PATH];
};

TRenumberSideLawsuit::TRenumberSideLawsuit()
{
	Add(new TString("F_FILE_NAME", fileName, sizeof fileName));
}

#include "RenumberSideWindDialog.h"
#include "RenumberSideWindDialog.cpp"

void renumber_sideWind(TWindow *parent)
{
	TFoundGroup source, target;
	TRenumberSideLawsuit lawsuit;

	if (TRenumberSideWindDialog(parent, &source, &target, &lawsuit).Execute() == IDOK)
	{
		TGArray<TLawsuit> lawsuits;
		THTMLFile h;
		mstr m;

		m.printf("страната %s %s", UCNType->Seek(source.ucnType), source.ucn);

		if (!ask("Желаете ли да преномерирате %s на %s %s?", str(m), UCNType->Seek(target.ucnType), target.ucn))
			return;

		h.OpenTemp();
		log(LOG_IMPORT, CM_ADMIN_RENUMBER_SIDE_WIND, "преномериране на страна");
		h.printf("Преномериране на %s -> %s %s\n\n", str(m), UCNType->name, target.ucn);

		if (any(lawsuit.fileName))
		{
			FILE *f = fopen(lawsuit.fileName, "r");
			char s[0x100];
			int lines = 0;
			int invalid = 0;

			if (!f)
			{
				error("%s: грешка %d при отваряне на текстов файл.");
				return;
			}

			while (fgets(s, sizeof s, f) != NULL)
			{
				lines++;

				if (sscanf(s, "%c %ld %ld", &lawsuit.key.type,  &lawsuit.key.no, &lawsuit.key.year) != 3)
				{
					h.printf("Ред %d: невалиден формат\n", lines);
					invalid++;
				}
				else if (!lawsuit.key.type || !strchr(TYPE_LAWSUITS, lawsuit.key.type))
				{
					h.printf("Ред %d: невалиден вид дело\n", lines);
					invalid++;
				}
				else if (lawsuit.key.no < 1 || lawsuit.key.no > NO_MAX)
				{
					h.printf("Ред %d: невалиден номер на дело\n", lines);
					invalid++;
				}
				else if (lawsuit.key.year < YEAR_MIN || lawsuit.key.year > YEAR_MAX)
				{
					h.printf("Ред %d: невалидна година\n", lines);
					invalid++;
				}
				else if (!lawsuit.Try("F_SOURCE_NO, F_SOURCE_YEAR, F_SOURCE_TYPE"))
				{
					h.printf("Ред %d: липсващо дело\n", lines);
					invalid++;
				}
				else
					lawsuits.AddCopy(lawsuit);
			}

			h.printf("%d прочетени реда, %d невалидни\n\n", lines, invalid);
		}
		else
			lawsuits.AddCopy(lawsuit);

		renumber_sideWind(parent, h, source, target, lawsuits);
	}
}