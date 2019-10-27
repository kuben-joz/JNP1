#include "poset.h"

namespace jnpzad2{
  using Key = std :: uint_64;
  using Elem = std :: unordered_map<Key , std :: unordered_set<Key>>;
  using ElemSet = unordered_set<Elem>;
  using NameMap = std :: unordered_map<std :: string, Key>;
  using Poset = std :: pair<ElemSet, NameMap>;

  using PosetContainerIndexType = std :: unsigned long;
  std ::unordered_map<PosetContainerIndexType, Poset> Posets;


}
