void sweep_invalid_link_winds(const TGArray<TSideWind>& sides,
  TGArray<TLinkWind>& links)
{
  int i ;
  const int items = links.Count () ;

  for (i = items - 1 ; i >= 0 ; i --)
  {  // reversed order, because I will detach any invalid elements
    TSideWind ordinary;
    TSideWind proxy;
    TSideWind redirect;
    TLinkWind& aL = *(links[i]);
    bool mustBeDeleted = false;

    aL.GetOrdinary(ordinary);
    aL.GetProxy(proxy);
    aL.GetRedirect(redirect);
    if((side_present(sides, ordinary) && side_present(sides, proxy)) == false)
      mustBeDeleted = true;
    else
      if(aL.Redirected() && (side_present(sides, redirect) == false))
        mustBeDeleted = true;
    if(mustBeDeleted)
      if(aL.Delete(true))
        links.Remove(i);
  }
}  // sweep_invalid_link_winds

void sweep_invalid_link_winds(TDatedContainer& dContainer)
{
  TGArray<TLinkWind> links;
  TLinkWind wL;

  if(!(dContainer.key.type && dContainer.key.year && dContainer.key.no &&
    dContainer.kind && dContainer.date.Empty() == false))
    return;
  wL << dContainer;
  link_wind_to_link_winds(wL, links);
  if(links.Count() == 0)
    return;

  TGArray<TSideWind> dSides;

  dated_container_to_side_winds(dContainer, dSides);
  sweep_invalid_link_winds(dSides, links);
}  // sweep_invalid_link_winds
