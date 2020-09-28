// "Less and less effort is spent on fixing original design flaws; more and
// more is spent on fixing flaws introduced by earlier fixes".
//
// "As time passes, the system becomes less and less well-ordered. Sooner or
// later the fixing ceases to gain any ground. Each forward step is matched
// by a backward one. Although in principle usable forever, the system has
// worn out as a base for progress."
//
// Frederick P. Brooks
// University of North Carolina professor and former OS/360 project manager

// The Law of Continuing Growth:
// "The functional capability of E-type systems must be continually
// increased to maintain user satisfaction over the system lifetime."
//
// The Law of Declining Quality:
// "The quality of E-type systems will appear to be declining unless they
// are rigorously adapted, as required, to take into account changes in the
// operational environment."
//
// The Principle of Software Uncertainty:
// "The real world outcome of any E-type software execution is inherently
// uncertain with the precise area of uncertainty also unknowable."
//
// Meir Lehman, a former researcher at IBM

// Алиса тъкмо започна да се отегчава, когато доприпка Белият заек, сграбчил
// пластмасово шише в лапките си. Капачката му беше пробита, той пръскаше
// вода наоколо и си мърмореше под мустачките.
// - Извинявай, господин заеко, за какво ти е тази вода? - попита Алиса.
// - Тази вода ли? - сепна се заекът. - Това е административна вода. Пръскам
// с нея против бъгове.
//
// (в-к "Капитал")

// Basically, companies don't want to change the way their fundamental
// "business processes" work even when these "processes" don't make any
// sense. So if you take the same old inefficient way of doing things, and
// make software to facilitate it, you're still doing it inefficiently.
// Especially when requirements for "visionary" systems get bogged down with
// specification by committee - everybody wants to make sure that their
// department or group level jobs are represented and that nobody designs
// them out of the picture.
//
// ("IT Doesn't Matter", Nicholas Carr)

// The source code itself is the specification. The level of detail required
// to interoperate successfully is simply not documentable - it would
// produce a stack of paper so high you might as well publish the source
// code.
//
// (The developers of Samba)

// To create quality software, the ability to say "no" is usually far more
// important than the ability to say "yes."
//
// ("The Rise and Fall of CORBA" - Michi Henning, member of the Object
// Management Group's architecture board)

// Pride
//
// This is all too common in programmers. Instead of asking whether a
// particular function exists, or for the best way to retrieve data from the
// system, a proud programmer is likely to write their own. Especially when
// faced with a large, or unfamiliar, code base.
//
// Lust
//
// Programmers crave pleasure; they love to 'scratch their own itches'. If
// unchecked, some developers will write fantastic, innovative, original...
// and completely unnecessary, code.
//
// Anger
//
// Do not code out of anger. Do not ignore good ideas, regardless from where
// they come. [...] The engine programmer should not be allowed to implement
// his own solution just because "He's the engine programmer".
//
// ("The seven sins of programmers" - Steven Goodwin)

// NT v3.51 shows a quirk whereby a popup/disabled tooltip takes
// activation away from the main window unless it's made visible?
// (I know! don't ask).
//
// (Borland C++ 5.01 Object Windows Library, TOOLTIP.C:356)

// There’s a software engineering philosophy called "Worse Is Better" that’s
// been around for a good 18 years. In that time, it’s come to mean that an
// inferiorly designed system or piece of software may be more successful
// than its better-designed competitor, based on subjective or unexpected
// market criteria. [...] "Worse Is Better" happens because systems are
// multi-layered. Users interact with the topmost layers of a system, and
// this superficial interaction characterizes their perception of the
// system’s value.
//
// (Internet Evolution - Alex Payne - The Internet's on Shaky Ground)

// One of the biggest problems the tech industry has is the ability to
// differentiate between "can we do this?" and "should we do this?" Itanium
// was a textbook case of engineering optimism outweighing business sense.
//
// Q: How many Pentium designers does it take to screw in a light bulb?
// A: 0.99904274017, but that's close enough for non-technical people.
//
// The problem is that the market does not appreciate statistical
// probability. If you were to tell people that there was a 1 in 10,000
// chance of something happening to their computer, they will worry about
// it even though they know the odds of a failure are extremely low.
//
// (Shaun Nichols, Iain Thomson - Top 10 technology mistakes)

// Finally, we will access, disclose and preserve personal data, including
// your content (such as the content of your emails, other private
// communications or files in private folders), when we have a good faith
// belief that doing so is necessary.
//
// (Microsoft Privacy Statement, July 2015)

// В приказката за трите прасенца-програмисти, къщата е направена от кал и
// пръчки, но прасенцата я ремонтират по-бързо, отколкото вълкът я събаря.

// ----- VARIANTH ------------------------------------------------------------
#define INSTANCE 1
#define APPEAL 0
#define ADMIN 0
#define MILITARY 0
#define SPECIAL 0

#if (APPEAL || ADMIN || MILITARY) && !INSTANCE
#error APPEAL, ADMIN and MILITARY require INSTANCE
#endif

#if (APPEAL + ADMIN + MILITARY) > 1
#error ADMIN, APPEAL and MILITARY mutually exclusive
#endif

#if SPECIAL && !MILITARY
#error SPECIAL requires MILITARY
#endif

// ----- VERSION -------------------------------------------------------------
#define BASEVER "7884"
#define VERSION "1." BASEVER
#define EXTRAVER "47"

// ----- RESOURCE ------------------------------------------------------------
#define PRODUCTVERSION PRODUCTVERSION 1, 78, 8, 4
#define FILEVERSION FILEVERSION 1, 79, 3, 1

// ----- RELEASE -------------------------------------------------------------
#define CUTCITILAWS MILITARY	// or SPECIAL only
#define SPEEDTESTING 0
#define SPLITREPSUBJ 0		// 20170262

// ----- COURT ---------------------------------------------------------------
#define COURT_REGIONAL	7
#define COURT_MILITARY	6
#define COURT_AREAL	5
#define COURT_APPEAL	3
#define COURT_ADMIN	2

#define COURT_LETTER_APPEAL	'p'
#define COURT_LETTER_ADMIN	'd'
#define COURT_LETTER_AREAL	'a'
#define COURT_LETTER_MILITARY	'm'
#define COURT_LETTER_REGIONAL	'r'

// run-time CourtType only, compilation = COURT_MILITARY
#define COURT_MILITARY_APPEAL 9

#if APPEAL
#define COURT_TYPE	COURT_APPEAL
#define COURT_LETTER	COURT_LETTER_APPEAL
#endif  // APPEAL

#if ADMIN
#define COURT_TYPE	COURT_ADMIN
#define COURT_LETTER	COURT_LETTER_ADMIN
#endif  // ADMIN

#if MILITARY
#define COURT_TYPE	COURT_MILITARY
#define COURT_LETTER	COURT_LETTER_MILITARY
#endif  // MILITARY

#ifndef COURT_TYPE
#if INSTANCE  // AREAL
#define COURT_TYPE	COURT_AREAL
#define COURT_LETTER	COURT_LETTER_AREAL
#else  // REGIONAL
#define COURT_TYPE	COURT_REGIONAL
#define COURT_LETTER	COURT_LETTER_REGIONAL
#endif
#endif

#if COURT_TYPE != COURT_APPEAL && COURT_TYPE != COURT_ADMIN && COURT_TYPE != COURT_AREAL && COURT_TYPE != COURT_MILITARY && COURT_TYPE != COURT_REGIONAL
#error invalid COURT_TYPE
#endif

#if CUTCITILAWS && !MILITARY
#error CUTCITILAWS requires MILITARY
#endif

// 2018-09-21
#define EPORTAL_NAME "ЕПЕП"
