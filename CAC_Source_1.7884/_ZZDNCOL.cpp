#include "cac.h"

# ifdef WITH_STARS
# error WITH_STARS must not be defined here
# endif
# define WITH_STARS 1

// class BookZZDNEnum
// class BookZZDNColumns                                     

# if ! INSTANCE  // Regional court only
void side_to_string_invert_UCN(const TSideWind* const pSide, char* const dest,
  const size_t destSz, const bool includingUCN, const bool includingInvolvement)
{
  if (! (dest && destSz))
    return ;
  dest [0] = '\0' ;
  if (! pSide)
    return ;
  dest [destSz - 1] = '\0' ;

  ostrstream oss (dest, destSz - 1) ;

  char * tmp = new char [destSz] ;
  tmp [0] = '\0' ;

  if (includingInvolvement)
  {
    TInvolvement invl ;

    invl.involvement = pSide -> involvement ;
    if (invl.Get ())
      oss << invl.name << " ";
  }

  if (strchr (UCN_CITIZENS, pSide -> ucnType))
      citizenUCNAndUCNTypeToName (pSide -> ucn, pSide -> ucnType, tmp, destSz);
  else
      firmUCNAndUCNTypeToName (pSide -> ucn, pSide -> ucnType, tmp, destSz) ;
  oss << tmp;

  // 2013:343 LPR: simplified and removed useless code
  if (includingUCN && (pSide -> ucnType == UCN_CITIZEN_UCN || pSide -> ucnType == UCN_FIRM_TAX_NO))
  {
     oss << (pSide -> ucnType == UCN_CITIZEN_UCN ? " ЕГН " : " ") << pSide -> ucn ;
  }
  oss << ends ;
  delete[] tmp ;
}  // side_to_string_invert_UCN
# endif

# if ! INSTANCE  // Regional court only
BookZZDNEnum :: BookZZDNEnum ()
 {
 }  // BookZZDNEnum :: BookZZDNEnum

# endif

# if ! INSTANCE  // Regional court only
void BookZZDNColumns :: makeColumns ( TRequest & request,
						  const int serialNo
						 )
 {
  const size_t buffSz = MAKE_COLUMNS_BUFF_SIZE ;
  const size_t buffL = buffSz - 1 ;
  char * buff  = new char [buffSz] ;
  char * buff1 = new char [buffSz] ;
  char * buff2 = new char [buffSz] ;
  column * pColumn ;  // To make looking of the program text more easy

  TLawsuit lawsuit;
  clearGroupData (lawsuit) ;
  //const bool trueRequest = request.key.type == TYPE_REQUEST;
  bool reqOk =  TRCDKey_(request.key).FilledOK();
  bool lawOk = false;

  if(reqOk)
  // if(request.resolution == RESOLUTION_LAWED)
    lawOk = requestColumns::tryToLawsuit( request, lawsuit);

  if (! columns)
    return ;
  if (! buff)
    return ;
  buff  [buffSz - 1] = '\0' ;
  buff1 [buffSz - 1] = '\0' ;
  buff2 [buffSz - 1] = '\0' ;
  if ((pColumn = columns [iC_SerialNo]) != NULL)
     {  // -- iC_SerialNo --        Номер по ред
      ColumnXmlEncloser encloser(pColumn, iC_SerialNo + bOC, pItems);
      addLongToColumn (pColumn, serialNo) ;
     }  // -- iC_SerialNo --

  if ((pColumn = columns [iC_RequestNoAndDate]) != NULL)
     {  // -- iC_RequestNoAndDate --   Входящ документ
        ColumnXmlEncloser encloser(pColumn, iC_RequestNoAndDate + bOC, pItems);
        memset(buff, '\0', sizeof buff);
        memset(buff1, '\0', sizeof buff1);

        //kindToString (request.kind, buff, buffL) ;
        //pColumn -> addText (buff) ;

        CDateToString (request.date, buff1, buffL) ;
        ostrstream (buff, buffL) << "Вх.No " << request.key.no << "/" << buff1
                                 << "г." << ends;
        pColumn -> addText (buff) ;
     }  // -- iC_RequestNoAndDate --

  if ((lawOk)&&((pColumn = columns [iC_LawKey]) != NULL))
     {  // -- iC_LawKey --   Дело
        ColumnXmlEncloser encloser(pColumn, iC_LawKey + bOC, pItems);
        memset(buff, '\0', sizeof buff);
        memset(buff1, '\0', sizeof buff1);
        memset(buff2, '\0', sizeof buff1);

        type_to_alias (lawsuit.key.type, buff2, buffL ) ; // 20060318
	  CDateToString (lawsuit.date, buff1, buffL) ;
        ostrstream (buff, buffL) << buff2 << " No " << lawsuit.key.no << "/"
                                 << buff1  << "г. " << ends;
        pColumn -> addText (buff) ;

        addCompositionToColumn (lawsuit.composition, pColumn) ;

     }  // -- iC_LawKey --

  if ((pColumn = columns [iC_Sides]) != NULL)
  { // -- iC_Sides --
     ColumnXmlEncloser encloser(pColumn, iC_Sides + bOC, pItems);
     bool prUCN=PRINT_UCN;

     if (lawOk) // get sides from lawsuit
     {
       lawRCDKeyToSidePair pairs(lawsuit, true);
       if (pairs.getPairsCount ())
       {
         unsigned i;
         unsigned claims = pairs.getClaimantsCount();
         unsigned defs = pairs.getDefendantsCount();

         for(i = 0; i < claims; i ++)// "Молител: "
         {
           TSideWind clSide;
           TClaimant claimant;

           if(pairs.nextPair (&claimant, NULL, &clSide))
	     {  if(prUCN)                             //print ucn  // print involvement
                side_to_string_invert_UCN(&clSide, buff, buffL, true,          true);
              else side_to_string_invert_UCN(&clSide, buff, buffL, false, true);
              pColumn->addText(buff);
           }
         }  // for(i = 0; i < claims; i ++)

         pairs.readIndexesToZero();
         for(i = 0; i < defs; i ++)// "Ответник: "
         {
           TSideWind defS;
           TDefendant defendant;

           if(pairs.nextPair(NULL, & defendant, NULL, & defS))
           {  if(prUCN) side_to_string_invert_UCN(&defS, buff, buffL, true,  true);
              else      side_to_string_invert_UCN(&defS, buff, buffL, false, true);
              pColumn->addText(buff);
           }
         }  // for (i = 0 ; i < defs ; i ++)
       }  // if (pairs.getPairsCount ())
     } // if lawOk

     else // get sides from request
     {
       reqRCDKeyToSidePair pairs (request, true) ;

       if (pairs.getPairsCount ())
       {
         unsigned i;
         unsigned claims = pairs.getClaimantsCount();
         unsigned defs = pairs.getDefendantsCount();

         for(i = 0; i < claims; i ++)// "Молители: "
         {
           TSideWind clSide;
           TClaimant claimant;

           if(pairs.nextPair (&claimant, NULL, &clSide))
           {  if(prUCN)                             //print ucn  // print involvement
                 side_to_string_invert_UCN(&clSide, buff, buffL, true,          true);
              else side_to_string_invert_UCN(&clSide, buff, buffL, false, true);
              pColumn->addText(buff);
              // pColumn->addText(claimant.cName);
           }
         }

         pairs.readIndexesToZero();
         for(i = 0; i < defs; i ++)// "Ответник: "
         {
           TSideWind defS;
           TDefendant defendant;

           if(pairs.nextPair(NULL, & defendant, NULL, & defS))
           {
              if(prUCN) side_to_string_invert_UCN(&defS, buff, buffL,  true, true);
              else      side_to_string_invert_UCN(&defS, buff, buffL, false, true);
              pColumn->addText(buff);
              //pColumn->addText(defendant.dName);
           }
         }
       }  // if (pairs.getPairsCount ())
     }
  }  // -- iC_Sides --

 delete[] buff ;
 delete[] buff1 ;
 delete[] buff2 ;

}  // BookZZDNColumns :: makeColumns
# endif

# if ! INSTANCE  // Regional court only
BookZZDNColumns :: BookZZDNColumns (const char * formatFN,
                                    TWindow * parent,
                                    TRequest & request,
                                    size_t minRows,
                                    const int serialNo,
                                    TStringArray * itemsPtr
                                   ) :
                     BookZZDNEnum (),

                     columnsArr (formatFN, parent, i_columns,
                                 OFFSET_BOOK_ZZDN_CIPHER+1),

                     pItems (itemsPtr)
 {
  minRows=1;
  makeColumns (request, serialNo) ;
  defineMaxRows () ;
  if (maxRows < minRows)
    maxRows = minRows ;
 }  // BookZZDNColumns :: BookZZDNColumns
# endif




# undef WITH_STARS

// _ZZDNCol.Cpp -- End

