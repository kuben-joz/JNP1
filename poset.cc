#include "poset.h"

namespace jnp2{
  using Key = std :: uint_64;
  using Elem = std :: unordered_map<Key , std :: unordered_set<Key>>;
  using ElemSet = unordered_set<Elem>;
  using NameMap = std :: unordered_map<std :: string, Key>;
  using Poset = std :: pair<ElemSet, NameMap>;

  using PosetContainerIndexType = std :: unsigned long;
  //może trzymać referencje do posetów &poset
  //to zapasowy pomysł jeżeli kompilator będzie miał problem
  std ::unordered_map<PosetContainerIndexType, &Poset> posets;


  Key hashString(std :: string &string){
    //TODO
  }

  void addElem(&poset, &name){
    Key key = hashString(name);
    poset.first[name] = key;
    poset.second[key] = ;//empty set
  }




}


unsigned long poset_new(void) {
  //TODO
}

void poset_delete(unsigned long id) {
  //TODO
}

size_t poset_size(unsigned long id) {
  //TODO
}

bool poset_insert(unsigned long id, char const *value) {
  std :: string name (value);
  auto answ1 = jnp2 :: posets.find(id);
  if(answ1 == posets.end()) return false;
  auto& poset = answ1->second;
  auto answ2 = poset->second.find(name);
  if(answ2 == posets.end()) {
    jnp2 :: addElem(poset, name);
    return true;
  }
  else return false;
}

bool poset_remove(unsigned long id, char const *value) {
  //TODO
}

bool poset_add(unsigned long id, char const *value1, char const *value2) {
  //TODO
}

bool poset_del(unsigned long id, char const *value1, char const *value2) {
  //TODO
}

bool poset_test(unsigned long id, char const *value1, char const *value2) {
  //TODO
}

void poset_clear(unsigned long id) {
  //TODO
}
