#include "cac.h"  // 18.may.2001
//# include "all.h"

int bit_value (const int bitNo)
 {
  int bitVal = 1 ;

  for (int j = 1 ; j <= bitNo ; j ++)
    bitVal *= 2 ;
  return bitVal ;  
 }  // bit_value

int which_bit_is_set (const int flagValue, const int maxFlags)
// returns -1 at event of failure
 {
  // Sorry, but it looks, the TBitSet class can not help me here.
  int whichBit = -1 ;

  for (int i = 0 ; whichBit == -1 && i < maxFlags ; i ++)
    if (bit_value (i) == flagValue)
      whichBit = i ;
  if (whichBit == -1)
    error ("which_bit_is_set: incorrect flagValue (%d).", flagValue) ;
  return whichBit ;
 }  //  which_bit_is_set

void add_to_set_bit_array (arrayOfLong & arr, const long int val,
                           const bool unconditionally = false)
 {
  if (unconditionally)
    arr.Add (val) ;
  else
   {
    bool found = false ;
    const int items = arr.Count () ;

    for (int i = 0 ; found == false && i < items ; i ++)
      found = (arr [i] == val) ;
    if (! found)
      arr.Add (val) ;
   }
 }  // add_to_set_bit_array

void recourse_bits (arrayOfLong & arr, int & currValue,
                    const int flagValue, const int bitToProcess)
// !!! This function works only if the first call to it is with
// zero value, referenced by currValue !!!
 {
  if (bitToProcess < 0)
    return ;

  const int diff = bit_value (bitToProcess) ;

  if ((diff & currValue) == 0)
   {  // The bit is not set.
    currValue += diff ;  // Set the bitToProcess.
    if ((flagValue & currValue) == flagValue)
      add_to_set_bit_array (arr, currValue) ;
    recourse_bits (arr, currValue, flagValue,
                   bitToProcess - 1) ;  // Process the next younger bit.
                                        // It is not set now.
    recourse_bits (arr, currValue, flagValue,
                   bitToProcess) ;  // Prosess the bitToProcess.
                                    // It is set now.
   }
  else
   {
    currValue -= diff ;
    if ((flagValue & currValue) == flagValue)
      add_to_set_bit_array (arr, currValue) ;
    recourse_bits (arr, currValue, flagValue,
                   bitToProcess - 1) ;  // Process the next younger bit.
                                        // It is not set now.
   }
 }  // recourse_bits

void add_set_bit_values (arrayOfLong & arr, const int flagValue,
                         int maxFlags)
 {
  if (maxFlags < 1)
    return ;
  if (flagValue == 0)
    add_to_set_bit_array (arr, flagValue) ;
  else
    if (maxFlags > 0)
     {  // -- 0 --
      const int flagsLimit = 8 ;

      if (maxFlags > flagsLimit)
       {
        error ("add_set_bit_values: the value of maxFlags is too "
               "big (%d) and will be set to %d", maxFlags, flagsLimit) ;
        maxFlags = flagsLimit ;
       }

      const int whichBit = which_bit_is_set (flagValue, maxFlags) ;

      if (whichBit != -1)
       {
        int currValue = 0 ;

        recourse_bits (arr, currValue, flagValue,
                       maxFlags - 1  // start with the major bit.
                      ) ;
       }
     }  // -- 0 --
 }  // add_set_bit_values

//
