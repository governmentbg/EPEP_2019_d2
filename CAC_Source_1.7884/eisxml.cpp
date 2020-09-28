#include "cac.h"

// ----- TEisFile ------------------------------------------------------------
TEisFile::TEisFile(long tSid)
	: TXMLFile("ЕИСПП"), sid(tSid)
{
}

void TEisFile::OpenWrite(const char *name)
{
	if (!TXMLFile::OpenWrite(name))
		xml_error(basename(name), "-open error %d", errno);
}

FILE *TEisFile::OpenQuery(const char *name)
{
	if (!TUserFile::Open(name, "wb"))
		xml_error(basename(name), "-open error %d", errno);

	return f;
}

void TEisFile::Close()
{
	if (!TXMLFile::Close())
		xml_error(basename(name), "-close error %d", errno);
}

// ----- TXMLItem ------------------------------------------------------------
TXMLItem::TXMLItem(const char *name)
	: Name(name), flags(0), value(NULL)
{
}

TXMLItem::~TXMLItem()
{
	if (flags & XML_UNKNOWN)
		delete[] Name;

	delete[] value;
}

void TXMLItem::Apply(TSbeDesc *desc)
{
	flags |= desc->flags;
	// 2012:320 error -> auto
	if (flags & XML_REQUIRED)
		flags |= XML_INCLUDED;

	if (desc->value && any(desc->value))
	{
		delete[] value;
		value = strnew(desc->value);
	}
}

bool TXMLItem::IsNULL()
{
	return RealNULL() && !(flags & XML_NULLABLE);
}

bool TXMLItem::IsValid(mstr &m, const char *what)
{
	if ((flags & XML_REQUIRED) && IsNULL())
	{
		m.printf("%s %s е задължителен", what, EisTransl->Seek(Name));
		return false;
	}

	return true;
}

// ----- TXMLAttrib ----------------------------------------------------------
char *TXMLAttrib::Read(char *text)
{
	char quot = *text;
	char *s;

	//if (!(flags & XML_INCLUDED))
	//	xml_error(text, "exc attr %s", Name);

	if (!strchr("'\"", quot))
		xml_error(Name, "attr ' or \" expected");

	text++;
	for (s = text; *s != quot; s++);
	*s++ = '\0';

	Parse(text);
	return s;
}

void TXMLAttrib::Write(TEisFile &x)
{
	x.OpenAttrib(Name);
	EmitX(x);
	x.CloseAttrib();
}

bool TXMLAttrib::IsValid(mstr &m)
{
	return TXMLItem::IsValid(m, "Атрибут");
}

// ----- TXMLString ----------------------------------------------------------
TXMLString::TXMLString(const char *name, char *tData, int tSize)
	: TXMLAttrib(name), data(tData), size(tSize)
{
	*data = '\0';
}

bool TXMLString::RealNULL()
{
	return !any(data);
}

void TXMLString::Parse(char *text)
{
	utf8_parse(text);

	if (strlen(text) >= size)
		text[size - 1] = '\0';

	strcpy(data, text);
}

void TXMLString::EmitX(TEisFile &x)
{
	x.Put(data);
}

// ----- TXMLShort -----------------------------------------------------------
TXMLShort::TXMLShort(const char *name, short *tData)
	: TXMLAttrib(name), data(tData)
{
	*data = 0;
}

void TXMLShort::Parse(char *text)
{
	int n;

	if (sscanf(text, "%hd%n", data, &n) != 1 || n != strlen(text))
		xml_error(text, "invalid short");
}

void TXMLShort::EmitX(TEisFile &x)
{
	x.Emit(NULL, (long) *data);
}

bool TXMLShort::RealNULL()
{
	return *data == 0;
}

// ----- TXMLInt -------------------------------------------------------------
TXMLInt::TXMLInt(const char *name, long *tData)
	: TXMLAttrib(name), data(tData)
{
	*data = 0;
}

void TXMLInt::Parse(char *text)
{
	int n;

	if (sscanf(text, "%ld%n", data, &n) != 1 || n != strlen(text))
		xml_error(text, "invalid int");
}

void TXMLInt::EmitX(TEisFile &x)
{
	x.Emit(NULL, *data);
}

bool TXMLInt::RealNULL()
{
	return *data == 0;
}

void TXMLInt::Apply(TSbeDesc *desc)
{
	TXMLItem::Apply(desc);
	lset.Clear();

	if ((flags & XML_INCLUDED) && value)
	{
		strvalue_to_lset(value, &lset, Name);

		if (*data == 0 && (flags & XML_REQUIRED) && lset.Count() == 1)
			*(long *) data = lset[0];
	}
}

bool TXMLInt::IsValid(mstr &m, long l)
{
	*data = l;

	if (TXMLAttrib::IsValid(m))
	{
		if (!l || !lset.Count() || lset.Contains(l))
			return true;

		m.printf("%s е извън възможните стойности", EisTransl->Seek(Name));
	}

	return false;
}

bool TXMLInt::IsValid(mstr &m)
{
	return IsValid(m, *data);
}

// ----- TXMLSid -------------------------------------------------------------
TXMLSid::TXMLSid(const char *name, long *data, char tPFix)
	: TXMLInt(name, data), pfix(tPFix)
{
	flags |= XML_INCLUDED | XML_REQUIRED | XML_NULLABLE;
}

void TXMLSid::Parse(char *text)
{
	if (isalpha(*text))
		text++; // *text = '-';

	TXMLInt::Parse(text);
}

void TXMLSid::EmitX(TEisFile &x)
{
	mstr m;

	if (pfix)
		m.cach(pfix);

	m.printf("%ld", x.sid == -1 ? *data - SID_MAXIMUM : *data > 0 ? -*data : -x.Generate());

	//if (pfix)
	//	*ncstr(m) = pfix;

	x.Put(str(m));
}

// ----- TXMLMoney -----------------------------------------------------------
TXMLMoney::TXMLMoney(const char *name, double *tData)
	: TXMLAttrib(name), data(tData)
{
	*data = 0.00;
}

void TXMLMoney::Parse(char *text)
{
	int n;

	if (sscanf(text, "%lf%n", data, &n) != 1 || n != strlen(text))
		xml_error(text, "invalid money");
}

void TXMLMoney::EmitX(TEisFile &x)
{
	if (*data == 0.00)
		x.Put("0");
	else
		x.EmitM(NULL, *data);
}

bool TXMLMoney::RealNULL()
{
	return *data == 0.00;
}

// ----- TXMLDate ------------------------------------------------------------
TXMLDate::TXMLDate(const char *name, CDate *tData)
	: TXMLAttrib(name), date(*tData)
{
}

void TXMLDate::Parse(char *text)
{
	int n;

	if (sscanf(text, "%04d-%02d-%02d%n", &date.year, &date.month, &date.day, &n) != 3 || n != strlen(text) ||
		!date.ValidEx())
	{
		xml_error(text, "invalid date");
	}
}

void TXMLDate::EmitX(TEisFile &x)
{
	x.Emit(NULL, date);
}

bool TXMLDate::RealNULL()
{
	return date.Empty();
}

// ----- TXMLTime ------------------------------------------------------------
TXMLTime::TXMLTime(const char *name, CTime *tTime)
	: TXMLAttrib(name), time(*tTime)
{
}

void TXMLTime::Parse(char *text)
{
	int n;

	if (sscanf(text, "%02d:%02d:%02d%n", &time.hour, &time.minute, &time.second, &n) != 3 || n != strlen(text) || !time.Valid())
		xml_error(text, "invalid time");
}

void TXMLTime::EmitX(TEisFile &x)
{
	x.Emit(NULL, time);
}

bool TXMLTime::RealNULL()
{
	return time.Empty();
}

// ----- TXMLUnknownAttrib ---------------------------------------------------
TXMLUnknownAttrib::TXMLUnknownAttrib(const char *name)
	: TXMLAttrib(strnew(name)), data(NULL)
{
	flags |= XML_UNKNOWN;
}

void TXMLUnknownAttrib::Parse(char *text)
{
	utf8_parse(text);
	data = strnew(text);
}

void TXMLUnknownAttrib::EmitX(TEisFile &x)
{
	x.Put(data ? data : "");
}

bool TXMLUnknownAttrib::RealNULL()
{
	return !data || !any(data);
}

// ----- TXMLStruct ----------------------------------------------------------
char *TXMLStruct::ReadStruct(char *text)
{
	if (*text != '<')
		xml_error(text, "object < expected");

	text++;
	int len = strlen(Name);

	if (!strncmp(text, Name, len) && !isalnum(text[len]))
		text = Read(text + len);
	else if (flags & XML_REQUIRED)
		xml_error(text, "object %s expected", Name);
	else
		text--;

	return text;
}

void TXMLStruct::Apply(TSbeDesc *desc)
{
	Apply(desc, !strcmp(desc->name, "."));
}

void TXMLStruct::Apply(TGArray<TSbeDesc> *descs)
{
	for (int i = 0; i < descs->Count(); i++)
		Apply((*descs)[i]);
}

void TXMLStruct::Apply(const char *name, int flags, const char *value)
{
	TSbeDesc desc(name, flags, value);
	Apply(&desc);
}

void TXMLStruct::Apply(TSbeDesc *desc, int skip)
{
	if (desc->name[skip])
		ApplyStandard(desc, skip);
	else
		TXMLItem::Apply(desc);
}

// ----- TXMLObject ----------------------------------------------------------
TXMLItem *TXMLObject::Find(const char *name, sign attr)
{
	if (attr)
	{
		for (int i = 0; i < attribs.Count(); i++)
			if (!strcmp(attribs[i]->Name, name))
				return attribs[i];
	}

	for (int i = 0; i < objects.Count(); i++)
	{
		TXMLStruct *object = objects[i];

		if (attr != 1 && !strcmp(object->Name, name))
			return object;

		if (dynamic_cast<TXMLObject *> (object))
		{
			TXMLItem *item = ((TXMLObject *) object)->Find(name, attr);

			if (item)
				return item;
		}
	}

	return NULL;
}

char *TXMLObject::Read(char *text)
{
	//if (!(flags & XML_INCLUDED))
	//	xml_error(text, "exc object %s", Name);

	if (*text == ' ')
		text++;

	while (isalpha(*text) || *text == '_')
	{
		char *s = strchr(text, '=');

		if (*s == NULL)
			xml_error(text, "attr = expected");

		*s = '\0';

		TXMLAttrib *attrib = FindAttrib(text);

		if (!attrib)
		{
			attrib = new TXMLUnknownAttrib(text);
			attribs.Add(attrib);
		}
		//else if (!(attrib->flags & XML_NULLABLE) && !attrib->RealNULL())
		//	xml_error(text, "dup attr %s", attrib->Name);

		text = attrib->Read(++s);
	}

	for (int i = 0; i < attribs.Count(); i++)
	{
		TXMLAttrib *attrib = attribs[i];

		if ((attrib->flags & XML_REQUIRED) && attrib->IsNULL())
			xml_error(Name, "attr %s missing/empty", attrib->Name);
	}

	bool end = *text == '/';

	text += end;
	if (*text++ != '>')
		xml_error(Name, "attr > expected");

	if (!end)
	{
		text = ReadStandard(text);

		for (;;)
		{
			if (*text++ != '<')
				xml_error(Name, "dynobj/end < expected");

			if (*text == '/')
			{
				int len = strlen(Name);

				if (strncmp(++text, Name, len) || text[len] != '>')
					xml_error(Name, "/%s> expected", Name);

				text += len + 1;
				break;
			}

			if (!isalpha(*text) && *text != '_')
				xml_error(text, "alpha or _ expected");

			char *s;
			for (s = text + 1; !strchr(" />", *s); s++);
			char c = *s;

			if (c == '\0')
				xml_error(Name, "dynobj > expected");

			*s = '\0';
			TXMLUnknownObject *object = new TXMLUnknownObject(text);
			*s = c;
			objects.Add(object);
			text = object->Read(s);
		}
	}
	else
	{
		for (int i = 0; i < objects.Count(); i++)
			if (objects[i]->flags & XML_REQUIRED)
				xml_error(Name, "object %s missing", objects[i]->Name);
	}

	return text;
}

void TXMLObject::Write(TEisFile &x)
{
	const char *end = NULL;

	x.OpenObject(Name);

	for (int i = 0; i < attribs.Count(); i++)
	{
		TXMLAttrib *attrib = attribs[i];

		if (!attrib->IsNULL())
		{
			if (attrib->flags & XML_INCLUDED)
			{
				x.Put(" ");
				attrib->Write(x);
			}
		}
		else if (attrib->flags & XML_REQUIRED)
			xml_error(Name, "attr %s empty", attrib->Name);
	}

	for (int i = 0; i < objects.Count(); i++)
	{
		TXMLStruct *object = objects[i];

		if (!object->IsNULL())
		{
			if (object->flags & XML_INCLUDED)
			{
				if (!end)
				{
					x.CloseHeader();
					end = Name;
				}

				object->Write(x);
			}
		}
		else if (object->flags & XML_REQUIRED)
			xml_error(Name, "object %s empty", object->Name);
	}

	x.CloseObject(end);
}

bool TXMLObject::RealNULL()
{
	for (int i = 0; i < attribs.Count(); i++)
		if (!attribs[i]->RealNULL())
			return false;

	for (int i = 0; i < objects.Count(); i++)
		if (!objects[i]->RealNULL())
			return false;

	return true;
}

bool TXMLObject::IsValid(mstr &m)
{
	if (!IsNULL())
	{
		for (int i = 0; i < attribs.Count(); i++)
			if (!attribs[i]->IsValid(m))
				return false;

		for (int i = 0; i < objects.Count(); i++)
			if (!objects[i]->IsValid(m))
				return false;
	}

	return TXMLStruct::IsValid(m, "Обект");
}

void TXMLObject::ApplyStandard(TSbeDesc *desc, int skip)
{
	const char *name = desc->name + skip;

	if (strcmp(name, "*"))
	{
		for (int i = 0; i < attribs.Count(); i++)
		{
			TXMLAttrib *attrib = FindAttrib(name);

			if (attrib)
			{
				attrib->Apply(desc);
				return;
			}
		}

		for (int i = 0; i < objects.Count(); i++)
		{
			TXMLStruct *object = objects[i];
			int len = strlen(object->Name);

			if (!strncmp(object->Name, name, len) && strchr(".", name[len]))
			{
				object->Apply(desc, skip + len + (name[len] == '.'));
				return;
			}
		}

		fatal("%s: missing XML path at %s", desc->name, name);
	}

	#if RANGECHECK
	if (!attribs.Count())
		fatal("%s applied to %s without attr", desc->name, Name);
	#endif  // RANGECHECK

	for (int i = 0; i < attribs.Count(); i++)
		attribs[i]->Apply(desc);
}

char *TXMLObject::ReadStandard(char *text)
{
	for (int i = 0; i < objects.Count(); i++)
		text = objects[i]->ReadStruct(text);

	return text;
}

// ----- TXMLGroup -----------------------------------------------------------
TXMLGroup::TXMLGroup(const char *xmlName, const char *groupName, TGroup *(*tClone)())
	: TXMLObject(xmlName), TGroup(groupName ? groupName : xmlName, tClone)
{
}

void TXMLGroup::LoadArray(TXMLArray *array, const char *criteria, TXMLArray *descript)
{
	TQuery q(criteria);

	if (!descript)
		descript = array;

	while (q.Read())
	{
		TXMLGroup *group = descript->NewObject();
		*group << q;
		array->Add(group);
	}
}

// ----- TXMLArray -----------------------------------------------------------
TXMLArray::TXMLArray(const char *name)
	: TXMLStruct(name)
{
}

char *TXMLArray::Read(char *text)
{
	TXMLGroup *object = NewObject();

	text = object->Read(text);
	Add(object);

	for (;;)
	{
		object = NewObject();
		char *tText = text;

		if ((text = object->ReadStruct(text)) == tText)
			break;

		if (object->IsNULL())
			delete object;
		else
			Add(object);
	}

	delete object;
	return text;
}

void TXMLArray::Write(TEisFile &x)
{
	for (int i = 0; i < Count(); i++)
		if (!ObjectAt(i)->IsNULL())
			ObjectAt(i)->Write(x);
}

bool TXMLArray::RealNULL()
{
	for (int i = 0; i < Count(); i++)
		if (!ObjectAt(i)->RealNULL())
			return false;

	return true;
}

bool TXMLArray::IsValid(mstr &m)
{
	for (int i = 0; i < Count(); i++)
		if (!ObjectAt(i)->IsValid(m))
			return false;

	return TXMLStruct::IsValid(m, "Обект");
}

TXMLGroup *TXMLArray::NewObject()
{
	TXMLGroup *object = NewTObject();

	object->flags = flags & XML_INCLUDED;
	ApplyDescs(object);

	return object;
}

void TXMLArray::ApplyDescs(TXMLGroup *group)
{
	for (int i = 0; i < descs.Count(); i++)
		group->ApplyStandard(descs[i], 0);
}

void TXMLArray::ApplyStandard(TSbeDesc *desc, int skip)
{
	for (int i = 0; i < Count(); i++)
		ObjectAt(i)->ApplyStandard(desc, skip);

	descs.Add(new TSbeDesc(desc->name + skip, desc->flags, desc->value));
}
