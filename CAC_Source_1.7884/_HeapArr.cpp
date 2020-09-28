HeapingData& HeapingData::operator = (const HeapingData& other)
{
	Copy(other);
	return (*this);
}  //HeapingData::operator =

void HeapingData::Clear()
{
}  // HeapingData::Clear

bool HeapingDataArray::StoreUp(const HeapingData& piece)
{
	const int ind = Find(&piece);
	const int items = Count();
	bool rT = true;

	if(ind >= 0 && ind < items)
  		((*this)[ind])->Accumulate(piece);
	else
	{
  	HeapingData* pNewItem = NewItem();

    pNewItem->Copy(piece);
		Add(pNewItem);
		if(Count() <= items)
			rT = error("%s::StoreUp\n""Unsuccessful Add operation.",
      	typeid(*this).name());
	}
	return rT;
}  // HeapingDataArray::StoreUp

//
