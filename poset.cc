#include "poset.h"

namespace jnpzad2{
  using Key = uint64_t;
  using Smaller = set<Key>;
  using Bigger = set<Key>;
  using Elem = std :: unordered_map<Key , std :: pair<Smaller, Bigger>>;
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

int posetTest()


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
  //value1 < value2
  std :: string name1 (value1);
  std :: string name2 (value2);

  auto answ1 = posets.find(id);
  if(answ1 == posets.end()) return false;
  auto& ElemMap = get<0>(answ1->second);
  auto& NameMap = get<1>(answ1->second);

  auto answ2 = NameMap.find(name1);
  if(answ2 == NameMap.end()) return false;
  Key key1 = answ2->second;

  auto answ3 = NameMap.find(name2);
  if(answ3 == NameMap.end()) return false;
  Key key2 = answ3->second;

  auto& elem1 = ElemMap[key1];
  auto answ4 = elem1.second.first.find(key2);
  if(answ4 != elem1.second.first.end()) return false;

  auto& elem2 = ElemMap[key2];
  auto answ5 = elem2.second.first.find(key1);
  if(answ5 != elem2.second.first.end()) return false;

  //iterate over set of smaller than elem1
  //adding all bigger than elem2 as bigger

  //iterate over set of bigger than elem2
  //adding all smaller than elem1 as smaller

  elem1.second.second.emplace(key2);
  elem2.second.first.emplace(key1);

  for(const auto& lowerElemKey: elem1.second.first){
    for(const auto& upperElemKey: elem2.second.second){
      ElemMap[lowerElemKey]->second.second.emplace(upperElemKey);
      ElemMap[upperElemKey]->second.first.emplace(lowerElemKey);
    }
  }

  return true;
}

bool poset_del(unsigned long id, char const *value1, char const *value2) {
  std :: string name1 (value1);
  std :: string name2 (value2);

  auto answ1 = posets.find(id);
  if(answ1 == posets.end()) return false;
  auto& ElemMap = get<0>(answ1->second);
  auto& NameMap = get<1>(answ1->second);


}

bool poset_test(unsigned long id, char const *value1, char const *value2) {
  //TODO

}

void poset_clear(unsigned long id) {
  //TODO
}
