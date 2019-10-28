#include "poset.h"

namespace jnpzad2{
  using Key = uint64_t;
  using Elem = std :: unordered_map<Key , std :: unordered_map<Key, std :: bool>>;
  using ElemMap = std :: unordered_map<Key, Elem>;
  using NameMap = std :: unordered_map<std :: string, Key>;
  using Poset = std :: tuple<ElemMap, NameMap, unsigned int>;
  using PosetContainerIndexType = unsigned long;
}

using namespace jnpzad2;

static PosetContainerIndexType posetCounter = 0;
//może trzymać referencje do posetów &poset
//to zapasowy pomysł jeżeli kompilator będzie miał problem

static std ::unordered_map<PosetContainerIndexType, Poset&> posets;


Key hashString(const Poset& poset, const std :: string& name){
    ElemMap tempElems = std :: get<0>(poset);
    NameMap tempNames = std :: get<1>(poset);
    auto tempNameIter = tempNames.find(name);
    if(tempNameIter == tempNames.end()) {
        unsigned int counter = std :: get<2>(poset);
        while((std :: get<0>(poset)).count(++counter));
        return counter;
        //add to ElemMap and NameMap here isntead of after exit
    }
    else return (*tempNameIter).second;
}

void addElem(Poset& poset, std :: string& name){
    Key key = hashString(poset, name);
    poset.first[name] = key;
    poset.second[key] = ; //empty set
}


unsigned long poset_new(void) {
    //Could add protection for all long IDs being taken, by iterating once through long and then throwing exception
    while (posets.count(++posetCounter));
    posets.emplace(std :: make_pair(posetCounter, Poset{ElemMap{make_pair(Key{}, Elem{})}, NameMap{}, 0}));
    return posetCounter;
}
void poset_delete(unsigned long id) {
  //TODO
}

size_t poset_size(unsigned long id) {
  auto answ1 = posets.find(id);
  if(answ1 == posets.end()) return 0;
  auto answ2 = get<0>(answ1);
  return (size_t) answ2.size();
}

bool poset_insert(unsigned long id, char const *value) {
  std :: string name (value);
  auto answ1 = posets.find(id);
  if(answ1 == posets.end()) return false;
  auto& poset = answ1->second;
  auto answ2 = poset->second.find(name);
  if(answ2 == posets.end()) {
    addElem(poset, name);
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
