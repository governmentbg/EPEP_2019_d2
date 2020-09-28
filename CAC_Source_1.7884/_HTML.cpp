#include "cac.h"  // 18.may.2001
//# include "all.h"

// ========================== pair tags - begin =========================

void HTML_ :: SimpleBegin ()
 {
  if (tag && tag [0])
    os << HTML_TL << tag << HTML_TR ;
 }  // HTML_ :: SimpleBegin

HTML_ :: HTML_ (ostream & oS, const bool simple,
                const char * const tagStr, const bool newLine) :
           os (oS), tag (tagStr ? tagStr : ""), nL (newLine)
 {
  if (nL)
    os << endl ;
  if (simple)
    SimpleBegin () ;
 }  // HTML_ :: HTML_

HTML_ :: ~HTML_ ()
 {
  if (tag && tag [0])
    os << HTML_TL << HTML_TE << tag << HTML_TR ;
  if (nL)
    os << endl ;
 }  // HTML_ :: ~HTML_

HTML_html :: HTML_html (ostream & oS, const bool newLine) :
               HTML_ (oS, true, "html", newLine)
 {
 }  // HTML_html :: HTML_html

HTML_head :: HTML_head (ostream & oS, const bool newLine) :
               HTML_ (oS, true, "head", newLine)
 {
 }  // HTML_head :: HTML_head

HTML_title :: HTML_title (ostream & oS, const bool newLine) :
                HTML_ (oS, true, "title", newLine)
 {
 }  // HTML_title :: HTML_title

HTML_center :: HTML_center (ostream & oS, const bool newLine) :
                HTML_ (oS, true, "center", newLine)
 {
 }  // HTML_center :: HTML_center

HTML_pre :: HTML_pre (ostream & oS, const bool newLine) :
              HTML_ (oS, true, "pre", newLine)
 {
 }  // HTML_pre :: HTML_pre

HTML_b :: HTML_b (ostream & oS, const bool newLine) :
            HTML_ (oS, true, "b", newLine)
 {
 }  // HTML_b :: HTML_b

HTML_body :: HTML_body (ostream & oS, const char * const color,
                        const bool newLine) :
               HTML_ (oS, false, "body", newLine)
 {
  os << HTML_TL << tag ;
  if (color && color [0])
    os << " bgcolor=\"" << color << "\"" ;
  os << HTML_TR ;
 }  // HTML_body :: HTML_body

HTML_font :: HTML_font (ostream & oS, const char * const face,
                        const char * const color,
                        const char * const size,
                        const bool newLine) :
               HTML_ (oS, false, "font", newLine)
 {
  os << HTML_TL << tag ;

  const bool facePresent = face && face [0] ;
  const bool colorPresent = color && color [0] ;
  const bool sizePresent = size && size [0] ;

  if (facePresent || colorPresent || sizePresent)
   {
    if (facePresent)
      os << " face=\"" << face << "\"" ;
    if (colorPresent)
      os << " color=\"" << color << "\"" ;
    if (sizePresent)
      os << " size=" << size ;
   }
  else
    os << " size=+0" ;
  os << HTML_TR ;
 }  // HTML_font :: HTML_font

HTML_table :: HTML_table (ostream & oS, const bool border,
                          const char * const bgcolor,
                          const int cellspacing, const int cellpadding,
                          const char * const width, const bool newLine) :
                HTML_ (oS, false, "table", newLine)
 {
  os << HTML_TL << tag ;
  if (border)
    os << " border" ;
  if (bgcolor && bgcolor [0])
    os << " bgcolor=\"" << bgcolor << "\"" ;
  os << " cellspacing=" << cellspacing
     << " cellpadding=" << cellpadding ;
  if (width && width [0])
    os << " widht=\"" << width << "\"" ;
  os << HTML_TR ;
 }  // HTML_table :: HTML_table

HTML_tr::HTML_tr(ostream & oS, const bool newLine) : // 14.aug.2002 - begin
	HTML_ (oS, true, "tr", newLine)
{
}  // HTML_tr::HTML_tr

HTML_th_td::HTML_th_td(ostream & oS, const char* const tag_,
  const char* const valign, const char* const align,
  const char* const bgcolor, const size_t colspan, const size_t rowspan,
  const bool newLine) :
		HTML_ (oS, false, ((tag_ && tag_[0]) ? tag_ : "th"), newLine)
{ // 14.aug.2002
	os << HTML_TL << tag;
	if(valign && valign[0])
		os << " valign=" << valign;
	if(align && align[0])
		os << " align=" << align;
	if(bgcolor && bgcolor[0])
		os << " bgcolor=\"" << bgcolor << "\"";
	if(colspan)
		os << " colspan=" << colspan;
	if(rowspan)
		os << " rowspan=" << rowspan;
	os << HTML_TR ;
}  // HTML_th::HTML_th_td

HTML_th::HTML_th(ostream & oS, const char* const valign,
	const char* const align, const char* const bgcolor, const size_t colspan,
  const size_t rowspan, const bool newLine) :
		HTML_th_td(oS, "th", valign, align, bgcolor, colspan, rowspan, newLine)
{ // 14.aug.2002
}  // HTML_th::HTML_th

HTML_td::HTML_td(ostream & oS, const char* const valign,
	const char* const align, const char* const bgcolor, const size_t colspan,
  const size_t rowspan, const bool newLine) :
		HTML_th_td(oS, "td", valign, align, bgcolor, colspan, rowspan, newLine)
{ // 14.aug.2002
}  // HTML_td::HTML_td

HTML_select :: HTML_select (ostream & oS, const bool newLine) :
               HTML_ (oS, true, "select", newLine)
 {  // 20.nov.2002
 }  // HTML_select :: HTML_select

HTMLSelectWriter::HTMLSelectWriter(const StringObjectRoll& lst,
	const bool guardHTMLCode, ostream* pOs, const bool newLine,
	const int selectedInd) :
		roll(lst), switchHTMLCode(guardHTMLCode), selectedIndex(selectedInd),
		useNewLines(newLine)
{  // 20.nov.2002
	if(pOs)
		Write(*pOs);
}  // HTMLSelectWriter::HTMLSelectWriter

void HTMLSelectWriter::Write(ostream& os) const
{
  //info("%s::Write", typeid(*this).name());
	if(roll.ItemsCount())  // Is this necessary?
	{
		if(useNewLines)
			os << endl;
		if(switchHTMLCode)
			os << HTML_CODE_ON;
		{
			HTML_select select(os, useNewLines);
			if(switchHTMLCode)
				os << HTML_CODE_OFF;
			for(int i = 0; i < roll.ItemsCount(); i++)
			{
				if(useNewLines && i > 0)
					os << endl;
//				if(switchHTMLCode)
//					os << HTML_CODE_ON;
//				html_option(os, NULL, i == selectedIndex, false);
//				if(switchHTMLCode)
//					os << HTML_CODE_OFF;
        //info("%s::Write:\nAbout to call WriteOption", typeid(*this).name());
				WriteOption(os, i);  // Write some data...
			}
			if(switchHTMLCode)
				os << HTML_CODE_ON;  // Just before the HTML_select's destructor
					// is invoked
		}
		if(switchHTMLCode)
			os << HTML_CODE_OFF;
		if(useNewLines)
			os << endl;
	}
}  // HTMLSelectWriter::Write

void HTMLSelectWriter::WriteOption(ostream& os, const int ind) const
{
  if(ind < roll.ItemsCount())
  {
    if(switchHTMLCode)
      os << HTML_CODE_ON;
    html_option(os, NULL, ind == selectedIndex, false);
    if(switchHTMLCode)
      os << HTML_CODE_OFF;
    os << roll[ind];
  }
}  // HTMLSelectWriter::WriteOption

ostream& operator<<(ostream& os, const HTMLSelectWriter& writer)
{  // 20.nov.2002
  writer.Write(os);
  return os;
}  // HTMLSelectWriter::operator<<

CharMatchHTMLSelectWriter::CharMatchHTMLSelectWriter(
  const CharMatchStringObjectRoll& lst, const char test_,
  const bool guardHTMLCode, const bool newLine, const int selectedInd)
  : HTMLSelectWriter(lst, guardHTMLCode, NULL, newLine, selectedInd),
    test(test_)

{
}  // CharMatchHTMLSelectWriter::CharMatchHTMLSelectWriter

void CharMatchHTMLSelectWriter::WriteOption(ostream& os, const int ind) const
{
  const CharMatchStringObjectRoll* pCharMatchRoll =
    dynamic_cast<const CharMatchStringObjectRoll*>(&roll);

  if(pCharMatchRoll)
  {
    //info("%s::WriteOption:\npCharMatchRoll is Ok", typeid(*this).name());
    if(ind < pCharMatchRoll->ItemsCount())
      if(pCharMatchRoll->Match(test, ind))
      {
				if(switchHTMLCode)
					os << HTML_CODE_ON;
				html_option(os, NULL, ind == selectedIndex, false);
				if(switchHTMLCode)
					os << HTML_CODE_OFF;
        os << pCharMatchRoll->ActualData(ind);
      }
  }
  else
    HTMLSelectWriter::WriteOption(os, ind);
}  // CharMatchHTMLSelectWriter::WriteOption

// =========================== pair tags - end ==========================

// ========================= single tags - begin ========================
void html_meta (ostream & os, const char * const text,
                const bool nLBefore, const bool nLAfter)
 {
  if (nLBefore)
    os << endl ;
  if (text)
    os << HTML_TL << "meta " << text << HTML_TR ;
  else
    os << HTML_TL
       << "meta http-equiv=\"Content-Type\" content=\"text/html; "
          "charset=windows-1251\""
       << HTML_TR ;
  if (nLAfter)
    os << endl ;
 }  // html_meta

void html_comment (ostream & os, const char * const text,
                   const bool nLBefore, const bool nLAfter)
 {
  if (! (text && text [0]))
    return ;
  if (nLBefore)
    os << endl ;
  os << "<!-- " << text << "-->";  // 16.oct.2002 (was missing)
  if (nLAfter)
    os << endl ;
 }  // html_comment

void html_br (ostream & os, const bool nLBefore, const bool nLAfter)
 {
  if (nLBefore)
    os << endl ;
  os << HTML_TL << "br" << HTML_TR ;  
  if (nLAfter)
    os << endl ;
 }  // html_br

void html_option (ostream& os, const char* const text,
	const bool selected, const bool nLBefore, const bool nLAfter)
 {  // 20.nov.2002
  if (nLBefore)
    os << endl ;
  os << "<option" ;
  if (selected)
  	os << " selected" ;
  os << ">" ;
  if (text && text [0])
    os << text ;
  if (nLAfter)
    os << endl ;
 }  // html_option

// ========================== single tags - end =========================
const char * html_CER_blank ()
 {
  return "&nbsp;" ;
 }  // html_CER_blank

bool CER_used (ostream & os, const char c)
 {
  bool used = false ;

  switch (c)
   {
    case HTML_TL :
      os << "&lt;" ;
      used = true ;
      break ;
    case HTML_TR :
      os << "&gt;" ;
      used = true ;
      break ;
    case '&' :
      os << "&amp;" ;
      used = true ;
      break ;
   }
  return used ;
 }  // CER_used

void string_to_html_ostream_swap_CRLF (ostream & os,
                                       const char * const str)
 {
  if (str)
   {
    const size_t l = strlen (str) ;

    for (size_t i = 0 ; i < l ; i ++)
     {
      if (CER_used (os, str [i]))
        continue ;
      if (str [i] == '\r')
        continue ;
      if (str [i] == '\n')
        html_br (os) ;
      else
        os << str [i] ;
     }
   }
 }  // string_to_html_ostream_swap_CRLF

void string_to_html_ostream_allowCode(ostream& os, const char* const str)
{  // 20.nov.2002
	if (str)
	{
		const size_t l = strlen (str) ;
		bool rT = true;
		const char onBegin = HTML_CODE_ON[0];
		const char offBegin = HTML_CODE_OFF[0];

		if (onBegin == offBegin)
		{
			bool codeOn = false;
      volatile size_t i = 0;

			for(; i < l; i ++)
			{
				if(str[i] == onBegin)
				{
					const char* const switchOnPtr = strstr(&(str[i]), HTML_CODE_ON);
					const char* const switchOffPtr = strstr(&(str[i]), HTML_CODE_OFF);
					const bool switchToOn = (switchOnPtr && switchOnPtr == &(str[i]));
					const bool switchToOff = (switchOffPtr && switchOffPtr == &(str[i]));
					bool switched = false;

					if(switchToOn || switchToOff)
					{
						if(switchToOn != switchToOff)
						{
							if(codeOn == false && switchToOn == true)
							{
								codeOn = true;
								switched = true;
							}
							else
								if(codeOn == true && switchToOff == true)
								{
									codeOn = false;
									switched = true;
								}
								else
									rT = error("Nested HTML_CODE_ON / HTML_CODE_OFF"
										"are not supported!\n"
                    "codeOn = %s\n"
                    "switchToOn = %s\n"
                    "switchToOff = %s\n"
                    //"%.50s"
                    ,
                    codeOn ? "true" : "false",
                    switchToOn ? "true" : "false",
                    switchToOff ? "true" : "false"
                    //,
                    //&(str[i])
                    ) ;
						}
						else
							rT = error("HTML_CODE_ON and HTML_CODE_OFF "
								"must not be equal!");
					}
					if(switched)
					{
						if(switchToOn)
							i += (strlen(HTML_CODE_ON) - 1);
						else
							if(switchToOff)
								i += (strlen(HTML_CODE_OFF) - 1);
						continue;
					}
				}
				if(codeOn)
					os << str[i] ;
				else
				{
					if(CER_used(os, str[i]))
						continue ;
					if(str[i] == '\r')
						continue ;
					if(str[i] == '\n')
						html_br(os) ;
					else
						os << str[i] ;
				}
			}
			if(codeOn)
				error("The closing HTML_CODE_OFF is missing!");
		}
		else
			rT = error("string_to_html_ostream_allowCode:\nThe condition\n"
				"HTML_CODE_ON[0] == HTML_CODE_OFF[0]\n"
				"is not satisfied");
		if(rT == false)
			info("Ще имате грешки в получения HTML изход.");
	}
}  // string_to_html_ostream_allowCode

/*   // 20070706
static void add_adjustment_args(char* const args, const size_t argsSz)
{
  FUNC_NAME("add_adjustment_args");

  if(args && argsSz > 1)
  {
    static RECT r;
    static bool rInitialized = false;

    if(rInitialized == false)
      initialize_rect(r, rInitialized);
    if(rInitialized)
    {
      strConcat(args, " /sap", argsSz);
      concat_val_arg(args, r.left, argsSz);
      concat_val_arg(args, r.top, argsSz);
      concat_val_arg(args, r.right, argsSz);
      concat_val_arg(args, r.bottom, argsSz);
    }
    else
      strConcat(args, " /sa", argsSz);
  }
  else
    REPORT_F_INVALID_ARGS;
}  // add_adjustment_args

// 20070706  */

// ========================== XML_ - begin =========================

XML_::XML_(ostream& oS, const char* const tagStr, SimpleStringsArray* pAttribs,
  const bool empty_, const bool newLineBefore, const bool newLineAfter,
  const bool singleTag):
  os(oS),tag(tagStr ? tagStr: ""), empty(empty_ || singleTag),
  nlAfter(newLineAfter)
{
  if(newLineBefore)
  {
    os << endl;
  }
  os << HTML_TL << tag;
  if(pAttribs)
  {
    for(int i = 0; i < pAttribs->Count(); i++)
    {
	SimpleStrings ss = *(*pAttribs)[i];

      os << " "
        << (ss.code ? ss.code : "") << "="
        << "\"" << (ss.str ? ss.str : "") << "\""
        << " ";
    }
  }
  if(empty == true && singleTag == false)
  {
    os << HTML_TE;
  }
  os << HTML_TR ;
}  // XML_::XML_

XML_::~XML_()
{
  if(empty == false)
  {
    os << HTML_TL << HTML_TE << tag << HTML_TR;
  }
  if(nlAfter)
  {
    os << endl;
  }
}  // XML_::~XML_

// ========================== XML_ - end =========================

const char* const XML_LT_ESCAPE_STR = "&lt;";
const char* const XML_GT_ESCAPE_STR = "&gt;";
const char* const XML_AMP_ESCAPE_STR = "&amp;";
const char* const XML_APOS_ESCAPE_STR = "&apos;";
const char* const XML_QUOT_ESCAPE_STR = "&quot;";


XmlText::XmlText(const char* const txt):
  text(txt ? txt : "")
{
}

const char XML_LT = HTML_TL;
const char XML_GT = HTML_TR;
const char XML_AMP = '&';
const char XML_APOS = '\'';
const char XML_QUOT = '\"';

ostream& operator<<(ostream& os, const XmlText& text)
{
  const size_t len = strlen(text.text);

  for(size_t i = 0; i < len; i++)
  {
    char ch = text.text[i];

    switch(ch)
    {
	case XML_LT:  // '<':
	  os << XML_LT_ESCAPE_STR;  // "&lt;";
	  break;
	case XML_GT:  // '>':
	  os << XML_GT_ESCAPE_STR;  // "&gt;";
	  break;
	case XML_AMP:  // '&':
	  os << XML_AMP_ESCAPE_STR;  // "&amp;";
	  break;
	case XML_APOS:  // '\'':
	  os << XML_APOS_ESCAPE_STR;  // "&apos;";
	  break;
	case XML_QUOT:  // '\"':
	  os << XML_QUOT_ESCAPE_STR;  // "&quot;";
	  break;
	default:
	  os << ch;
	  break;
    }
  }
  return os;
}

size_t xml_text_len(const char* const text, const size_t symbols)
{
  FUNC_NAME("xml_text_len");
  size_t xmlTxtLen = 0;

  if(text)
  {
    const size_t ltLen = strlen(XML_LT_ESCAPE_STR);
    const size_t gtLen = strlen(XML_GT_ESCAPE_STR);
    const size_t ampLen = strlen(XML_AMP_ESCAPE_STR);
    const size_t aposLen = strlen(XML_APOS_ESCAPE_STR);
    const size_t quotLen = strlen(XML_QUOT_ESCAPE_STR);
    size_t prevStepLen = xmlTxtLen;

    for(size_t i = 0; text[i] && i < symbols; i++)
    {
      switch(text[i])
      {
        case XML_LT:  // '<':
          xmlTxtLen += ltLen;  // "&lt;";
          break;
        case XML_GT:  // '>':
          xmlTxtLen += gtLen;  // "&gt;";
          break;
        case XML_AMP:  // '&':
          xmlTxtLen += ampLen;  // "&amp;";
          break;
        case XML_APOS:  // '\'':
          xmlTxtLen += aposLen;  // "&apos;";
          break;
        case XML_QUOT:  // '\"':
          xmlTxtLen += quotLen;  // "&quot;";
          break;
        default:
          xmlTxtLen += 1;
          break;
      }
      if(prevStepLen > xmlTxtLen)
      {
        fatal("%s:\n%s", funcName,
          "The XML text is too long to be proceeded");
      }
      prevStepLen = xmlTxtLen;
    }
  }
  return xmlTxtLen;
}  // xml_text_len

size_t xml_text_len(const char* const text)
{
  return xml_text_len(text, -1);
}  // xml_text_len

const char* closing_tag(const char* const opening, char* const closing,
  const size_t closingSz)
{
  FUNC_NAME("closing_tag");

  if(opening && opening[0] && opening[1] && opening[1] != '/' &&
    closing && closingSz > (strlen(opening) + 1))
  {
    ZERO_LEN(closing);
    strConcatCh(closing, opening[0], closingSz);
    strConcatCh(closing, '/', closingSz);
    strConcat(closing, opening + 1, closingSz);
  }
  else
    REPORT_F_INVALID_ARGS;
  return closing;
}  // closing_tag

const char* opening_tag(const char* const closing, char* const opening,
  const size_t openingSz)
{
  FUNC_NAME("opening_tag");

  if(closing && closing[0] && closing[1] && closing[1] == '/' && closing[2] &&
    opening && openingSz > (strlen(closing) - 1))
  {
    ZERO_LEN(opening);
    strConcatCh(opening, closing[0], openingSz);
    strConcat(opening, closing + 2, openingSz);
  }
  else
    REPORT_F_INVALID_ARGS;
  return opening;
}  // opening_tag

void column_tags(const TStringArray& xmlItems,
  TStringArray& tags, const char* const cBeginning)
{
  const int cnt = xmlItems.Count();
  char opening[32];
  char closing[32];
  const char* const columnBegin =
    cBeginning && cBeginning[0] == HTML_TL && cBeginning[1] ?
    cBeginning : "<c";

  tags.Flush();
  for(int i = 0; i < cnt; i++)
  {
    const char* const current = xmlItems[i];

    if(strstr(current, columnBegin) == current)
    {
      int j = 0;

      ZERO_LEN(opening);
      while(current[j] != '\0' && current[j] != ' ')
      {
        strConcatCh(opening, current[j], sizeof(opening));
        j++;
      }
	tags.Add(strnew (opening));
      tags.Add(strnew (closing_tag(opening, closing,
        sizeof(closing))));
    }
  }
}  // column_tags
