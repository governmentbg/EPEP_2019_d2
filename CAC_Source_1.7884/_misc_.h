class stringToSQLCharSet
 {
  private :
    char * rSBuff ;
  public :
    stringToSQLCharSet (const char * str) ;
    ~stringToSQLCharSet () ;
    const char * getTheSet () const ;
 } ;  // class stringToSQLCharSet

class SQLCharSet : public stringToSQLCharSet
 {
  public :
    SQLCharSet (const char * str) ;
 } ;  // class SQLCharSet

ostream & operator << (ostream & os, const SQLCharSet & cS) ;

struct doubleStrikedString
 {
  const char * const s ;
  const bool upCCyr ;
  doubleStrikedString (const char * const str = NULL, // NULL == ""
			     const bool uCC = false) ;
 } ;  // struct doubleStrikedString

typedef TGSArray <sessLstData> TIS_sessLstDataArray ;
typedef TGSArray <sessLstTimeSortData> TIS_sessLstTimeSortDataArray ;
typedef TStringArray stringArray ;

ostream & operator << (ostream & strm, const CDate d) ;
ostream & operator << (ostream & strm, const doubleStrikedString s) ;
ostream & operator << (ostream & strm, const CTime t) ;  // 06.apr.2001

class CharPtr
 {
  protected :
    size_t size ;
    char * s ;
    void NewData (const size_t size_) ;
    void EmptyData () ;
  public :
    CharPtr (const size_t size_) ;
    CharPtr (const CharPtr & source) ;
    CharPtr (const char * const str) ;
    ~CharPtr () { EmptyData () ; }
    const CharPtr & operator = (const CharPtr & source) ;
    bool operator == (const CharPtr & other) const ;
    size_t Size () const ;
    char * const Str () const ;
    operator char * const () ;
    void End () ;  // 18.feb.2003
 } ;  // class CharPtr

class JustifiedString : public CharPtr
 {
  protected :
    void Justify () ;
  public :
    JustifiedString (const char * str, size_t width) ;
 } ;  // class JustifiedString

ostream & operator << (ostream & os, const JustifiedString & jS) ;

typedef TIArray <CharPtr> CharPtrArray ;
typedef TGArray <TCitizen> CitizenArray ;  // 18.feb.2003

bool cannotOpenFmtFile (const char *fN);
  //  returns false

int scSprintf (size_t buffSz, char * buff, const char * format, ...) ;

bool referenceToFmtFile(char *fmtFN, size_t fmtFNSz);

void elapsed_24h_time_nessage (const CTime start, const CTime end) ;

int judges_count_expecting (const char lawKind, const char sessKind,
                            const bool ret0ForClosed) ;
int judges_in_session (const BriefSessionsToCheckGroup & sess) ;

const char * male_noun_article (const char * const noun, const bool full);
const char * noun_article (const char * const noun, const bool full) ;
char * add_noun_article (char * const noun, const size_t nounSz,
                         const bool full) ;
// noun must contain any substantive. Returns noun.                         

const char * male_adjective_article (const char * const adjective,
                                     const bool full) ;
const char * adjective_article (const char * const adjective,
                                const bool full) ;
char * add_adjective_article (char * const adjective,
                              const size_t adjectiveSz,
                              const bool full) ;
// adjective must not be empty.                              
char * add_article (char * const str, const size_t strSz, const bool full);
// str have to contain any phrase. resturns str

const char * male_passive_article (const char * const linkedNoun,
					     const bool full, const bool the) ;
const char * passive_article (const char * const linkedNoun,
					const bool full, const bool the) ;

bool cannot_load_format_info (const char * fmtS) ;
bool cannot_load_fmt_file_name_data (const char * const docName) ;

int cmp_str(const char* const a, const char* const b);  // 04.mar.2003
void court_logo_image_file(ostream& os);

char *gettext(ostrstream &oss);
char *getcrlf(const char *text);
