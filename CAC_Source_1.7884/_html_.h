// ========================== pair tags - begin =========================

const char HTML_TL = '<' ;
const char HTML_TR = '>' ;
const char HTML_TE = '/' ;

class HTML_
 {
  protected :
    ostream & os ;
    const bool nL ;
    const char * const tag ;
    HTML_ (ostream & oS, const bool simple, const char * const tagStr,
           const bool newLine) ;
    ~HTML_ () ;
    void SimpleBegin () ;
 } ;  // class HTML_

class HTML_html : public HTML_
 {
  public :
    HTML_html (ostream & oS, const bool newLine = true) ;
 } ;  // class HTML_html

class HTML_head : public HTML_
 {
  public :
    HTML_head (ostream & oS, const bool newLine = true) ;
 } ;  // class HTML_head

class HTML_title : public HTML_
 {
  public :
    HTML_title (ostream & oS, const bool newLine = true) ;
 } ;  // class HTML_title

class HTML_center : public HTML_
 {
  public :
    HTML_center (ostream & oS, const bool newLine = true) ;
 } ;  // class HTML_center

class HTML_pre : public HTML_
 {
  public :
    HTML_pre (ostream & oS, const bool newLine = true) ;
 } ;  // class HTML_pre

class HTML_b : public HTML_
 {
  public :
    HTML_b (ostream & oS, const bool newLine = true) ;
 } ;  // class HTML_b

class HTML_body : public HTML_
 {
  public :
    HTML_body (ostream & oS, const char * const color = NULL,
               const bool newLine = true) ;
 } ;  // class HTML_body

class HTML_font : public HTML_
 {
  public :
    HTML_font (ostream & oS, const char * const face,
               const char * const color = NULL,
               const char * const size = NULL,
               const bool newLine = true) ;
 } ;  // class HTML_font

class HTML_table : public HTML_
 {
  public :
    HTML_table (ostream & oS, const bool border,
                const char * bgcolor = NULL,
                const int cellspacing = 0, const int cellpadding = 1,
                const char * const width = NULL,
                const bool newLine = true) ;
 } ;  // class HTML_table

class HTML_tr : public HTML_  // 14.aug.2002 - begin
{
	public:
		HTML_tr(ostream & oS, const bool newLine = true);
};  // class HTML_tr

class HTML_th_td : public HTML_
{
	protected:
		HTML_th_td(ostream & oS, const char* const tag_,
      const char* const valign, const char* const align,
      const char* const bgcolor, const size_t colspan = 0,
      const size_t rowspan = 0, const bool newLine = true);
};  // class HTML_th_td

class HTML_th : public HTML_th_td
{
	public:
		HTML_th(ostream & oS, const char* const valign,
      const char* const align, const char* const bgcolor,
      const size_t colspan = 0, const size_t rowspan = 0,
      const bool newLine = true);
};  // class HTML_th

class HTML_td : public HTML_th_td
{
	public:
		HTML_td(ostream & oS, const char* const valign,
			const char* const align, const char* const bgcolor,
      const size_t colspan = 0, const size_t rowspan = 0,
      const bool newLine = true);
};  // class HTML_td  // 14.aug.2002 - end

class HTML_select : public HTML_
{  // 02.nov.2002
	public:
		HTML_select(ostream& oS, const bool newLine) ;
};  // class HTML_select

class HTMLSelectWriter
{  // 20.nov.2002
	public:  // Construction / destruction
		HTMLSelectWriter(const StringObjectRoll& lst, const bool guardHTMLCode,
			ostream* pOs = NULL, const bool newLine = false,
			const int selectedInd = 0);
      // Attention! All derivatives, that override any virtual member
      // functions must pass NULL for pOs (because it won't work).
	public:  // Methods
    void Write(ostream& os) const;
	protected:  // Methods
    virtual void WriteOption(ostream& os, const int ind) const;
	protected:  // Data
		const StringObjectRoll& roll;
		const bool switchHTMLCode;
		const int selectedIndex;
		const bool useNewLines;
};  // class HTMLSelectWriter

ostream& operator<<(ostream& os, const HTMLSelectWriter& writer);

class CharMatchHTMLSelectWriter: public HTMLSelectWriter
{
	public:
		CharMatchHTMLSelectWriter(const CharMatchStringObjectRoll& lst,
      const char test_, const bool guardHTMLCode,
      const bool newLine = false, const int selectedInd = 0);
	protected:  // Methods
    virtual void WriteOption(ostream& os, const int ind) const;
	protected:  // Data
    const char test;
};  // class CharMatchHTMLSelectWriter

// =========================== pair tags - end ==========================

// ========================= single tags - begin ========================
void html_meta (ostream & os, const char * const text = NULL,
                const bool nLBefore = false, const bool nLAfter = true) ;
void html_comment (ostream & os, const char * const text,
                   const bool nLBefore = false,
                   const bool nLAfter = true) ;
void html_br (ostream & os, const bool nLBefore = false,
                const bool nLAfter = true) ;
void html_option(ostream& os, const char* const text = NULL,
	const bool selected = false, const bool nLBefore = true,
	const bool nLAfter = false);
// ========================== single tags - end =========================

void string_to_html_ostream_swap_CRLF (ostream & os,
					 const char * const str) ;
inline void string_to_html_ostream_swap_CRLF (ostream & os, mstr &m) { string_to_html_ostream_swap_CRLF(os, str(m)); }
void string_to_html_ostream_allowCode(ostream& os, const char* const str);
inline void string_to_html_ostream_allowCode(ostream& os, mstr &m) { string_to_html_ostream_allowCode(os, str(m)); }
const char * html_CER_blank () ;

// ========================== XML_ - begin =========================

class XML_
{
public:  // Construction/destruction
  XML_(ostream& oS, const char* const tagStr,
    SimpleStringsArray* pAttribs = NULL, const bool empty_ = false,
    const bool newLineBefore = false, const bool newLineAfter = true,
    const bool singleTag = false);
  ~XML_();
protected:  // Data
  ostream& os;
  const char* const tag;
  const bool empty;
  const bool nlAfter;
};  // class XML_

// ========================== XML_ - end =========================

class XmlText
{
public:  // Construction/destruction
  XmlText(const char* const txt);
public:  // Data
  const char* const text;
};

ostream& operator<<(ostream& os, const XmlText& text);

size_t xml_text_len(const char* const text);
size_t xml_text_len(const char* const text, const size_t symbols);

const char* closing_tag(const char* const opening, char* const closing,
  const size_t closingSz);
const char* opening_tag(const char* const closing, char* const opening,
  const size_t openingSz);
void column_tags(const TStringArray& xmlItems,
  TStringArray& tags, const char* const cBeginning);
