#include "poset.h"
#include <unordered_map>
#include <unordered_set>
#include <cassert>


using Key = uint64_t;
  using Smaller = std :: unordered_set<Key>;
  using Bigger = std :: unordered_set<Key>;
  using Elem = std :: pair<Smaller, Bigger>;
  using ElemMap = std :: unordered_map<Key, Elem>;
  using NameMap = std :: unordered_map<std :: string, Key>;
  using PosetBody = std :: pair <ElemMap, NameMap>;
  using PosetInternalCounter = Key;
  using Poset = std :: pair <PosetBody, PosetInternalCounter>;
  using PosetContainerIndexType = unsigned long;

/**
 * TODO
 * check all passes are by reference - done
 * write tests for reference
 * check
 */

////Could consider anonymous workspace instead
static PosetContainerIndexType posetCounter = 0;
//może trzymać referencje do posetów &poset
//to zapasowy pomysł jeżeli kompilator będzie miał problem

static std ::unordered_map<PosetContainerIndexType, Poset> posets;

void addElem(Poset &poset, std::string &name) {
    ElemMap &tempElems = poset.first.first;
    NameMap &tempNames = poset.first.second;

    auto tempNameIter = tempNames.find(name);

    if (tempNameIter == tempNames.end()) {
        PosetInternalCounter &counter = poset.second; //Should counter be without &???
        while (tempElems.count(++counter));
        tempElems.emplace(make_pair(counter, Elem{})); //am I passing counter by reference? if so that pretty bad
        tempNames.emplace(name, counter); //again is this by value or reference
    }
}



unsigned long jnp1::poset_new(void) {
    while (posets.count(++posetCounter));
    posets.emplace(posetCounter, Poset{});
    assert(posets.count(posetCounter));
    return posetCounter;
}

//unsigned long here makes the thing unsightly
void jnp1::poset_delete(const PosetContainerIndexType id) {
    auto toDelete = posets.find(id);
    if (toDelete != posets.end()) {
        posets.erase(toDelete);
    }
}

size_t jnp1::poset_size(const PosetContainerIndexType id) {
    auto answ1 = posets.find(id);
    if (answ1 == posets.end()) return 0;
    return (*answ1).second.first.second.size(); //I dont think we have to cast
    //return (size_t) poset.first.size();
}

bool jnp1::poset_insert(const PosetContainerIndexType id, char const *value) {
    std::string name(value);
    auto answ1 = posets.find(id);
    if (answ1 == posets.end()) return false;
    auto &poset = answ1->second;
    auto answ2 = poset.first.second.find(name);
    if (answ2 == poset.first.second.end()) {
        addElem(poset, name);
        return true;
    }
    return false;
}

bool jnp1::poset_remove(const unsigned long id, char const *value) {
    std::unordered_map<PosetContainerIndexType, Poset>::iterator posetsIt = posets.find(id);
    if (posetsIt == posets.end()) {
        return false;
    }
    PosetBody &currentPosetBody = (*posetsIt).second.first;
    NameMap::const_iterator nameMapIt = currentPosetBody.second.find(value);
    if (nameMapIt == currentPosetBody.second.end()) {
        return false;
    }

    const Key keyToRemove = (*nameMapIt).second;
    ElemMap::iterator elemMapIt = currentPosetBody.first.find(keyToRemove);
    // Asserts iff an element that was in NameMap is also present in ElemMap,
    // an invariant of PosetBody
    assert(elemMapIt != currentPosetBody.first.end());
    Elem &relations = (*elemMapIt).second;

    Smaller::const_iterator smallerIt = relations.first.begin();
    while (smallerIt != relations.first.end()) {
        Key tempKey = *smallerIt++;
        Elem &tempElem = (*currentPosetBody.first.find(tempKey)).second;
        // Asserts that a < b -> b > a
        assert(tempElem.second.count(tempKey));
        tempElem.second.erase(keyToRemove);
    }

    Bigger::const_iterator biggerIt = relations.second.begin();
    while (biggerIt != relations.second.end()) {
        Key tempKey = *biggerIt++;
        Elem &tempElem = (*currentPosetBody.first.find(tempKey)).second;
        // Asserts that b > a -> a < b
        assert(tempElem.second.count(tempKey));
        tempElem.first.erase(keyToRemove);
    }

    currentPosetBody.first.erase(keyToRemove);
    currentPosetBody.second.erase(value);

    return true;
}

//// assertions finished here
bool loopCheck(ElemMap &elemMap, Elem &elem1, Elem &elem2) {
    for (const auto &lowerElemKey: elem1.first) {
        for (const auto &upperElemKey: elem2.second) {
            if (elemMap[lowerElemKey].first.find(upperElemKey) !=
                elemMap[lowerElemKey].first.end())
                return true;
        }
    }
    return false;
}

bool jnp1::poset_add(unsigned long id, char const *value1, char const *value2) {
    //value1 < value2
    std::string name1(value1);
    std::string name2(value2);

    auto answ1 = posets.find(id);
    if (answ1 == posets.end()) return false;
    auto &elemMap = answ1->second.first.first; //dlaczego &
    auto &nameMap = answ1->second.first.second; //dlaczego &

    auto answ2 = nameMap.find(name1);
    if (answ2 == nameMap.end()) return false;
    Key key1 = answ2->second;

    auto answ3 = nameMap.find(name2);
    if (answ3 == nameMap.end()) return false;
    Key key2 = answ3->second;

    auto &elem1 = elemMap[key1];
    auto answ4 = elem1.first.find(key2);
    if (answ4 != elem1.first.end()) return false;

    auto &elem2 = elemMap[key2];
    auto answ5 = elem2.first.find(key1);
    if (answ5 != elem2.first.end()) return false;

    //check if adding an edge would result in a loop

    if (loopCheck(elemMap, elem1, elem2) == true)
        return false;

    // for(const auto& lowerElemKey: elem1.first){
    //   for(const auto& upperElemKey: elem2.second){
    //     if(ElemMap[lowerElemKey]->first.find(upperElemKey)!=
    //        ElemMap[lowerElemKey]->first.end())
    //        return false;
    //   }
    // }

    //iterate over set of smaller than elem1
    //adding all bigger than elem2 as bigger

    //iterate over set of bigger than elem2
    //adding all smaller than elem1 as smaller

    elem1.second.emplace(key2);
    elem2.first.emplace(key1);

    for (const auto &lowerElemKey: elem1.first) {
        for (const auto &upperElemKey: elem2.second) {
            elemMap[lowerElemKey].second.emplace(upperElemKey);
            elemMap[upperElemKey].first.emplace(lowerElemKey);
        }
    }

    return true;
}


bool isDetachable(ElemMap &elemMap, Elem &elem1, Elem &elem2, Key key1, Key key2) {
    //only call for elem1 < elem2
    //a < b
    //w większych od każdego większego od a nie ma b
    //w mniejszych od każdego mniejszego od b nie ma a

    //trzeba dodać elemMap ...
    for (const auto &kbigr1 : elem1.second) {
        auto &bigr1 = elemMap[kbigr1];
        if (bigr1.second.find(key2) !=
            bigr1.second.end())
            return false;
    }

    for (const auto &ksmalr2 : elem2.first) {
        auto &smalr2 = elemMap[ksmalr2];
        if (smalr2.first.find(key1) != smalr2.first.end())
            return false;
    }

    return true;
}

bool jnp1::poset_del(unsigned long id, char const *value1, char const *value2) {
    std::string name1(value1);
    std::string name2(value2);

    auto answ1 = posets.find(id);
    if (answ1 == posets.end()) return false;
    auto &elemMap = (*answ1).second.first.first;
    auto &nameMap = (*answ1).second.first.second;

    auto answ2 = nameMap.find(name1);
    if (answ2 == nameMap.end()) return false;
    Key key1 = answ2->second;

    auto answ3 = nameMap.find(name2);
    if (answ3 == nameMap.end()) return false;
    Key key2 = answ3->second;

    auto &elem1 = elemMap[key1];
    auto answ4 = elem1.first.find(key2);

    auto &elem2 = elemMap[key2];
    auto answ5 = elem2.first.find(key1);
    if (answ4 == elem1.first.end() && answ5 == elem2.first.end())
        return false;

    if (answ4 == elem1.first.end()) {//nie jest elem1 > elem2 => elem1 < elem2
        if (!isDetachable(elemMap, elem1, elem2, key1, key2))
            return false;

        elem1.second.erase(key2);
        elem2.first.erase(key1);

        for (const auto &lowerElemKey: elem1.first) {
            for (const auto &upperElemKey: elem2.second) {
                elemMap[lowerElemKey].second.erase(upperElemKey);
                elemMap[upperElemKey].first.erase(lowerElemKey);
            }
        }
    } else { //jest elem1 > elem2
        if (!isDetachable(elemMap, elem2, elem1, key2, key1))
            return false;

        elem2.second.erase(key1);
        elem1.first.erase(key2);

        for (const auto &lowerElemKey: elem2.first) {
            for (const auto &upperElemKey: elem1.second) {
                elemMap[lowerElemKey].second.erase(upperElemKey);
                elemMap[upperElemKey].first.erase(lowerElemKey);
            }
        }
    }
    return true;
}

bool jnp1::poset_test(unsigned long id, char const *value1, char const *value2) {
    //czy value1 < value2Successful compilation and linking
    std::string name1(value1);
    std::string name2(value2);

    auto answ1 = posets.find(id);
    if (answ1 == posets.end()) return false;
    auto &elemMap = (*answ1).second.first.first;
    auto &nameMap = (*answ1).second.first.second;

    auto answ2 = nameMap.find(name1);
    if (answ2 == nameMap.end()) return false;
    Key key1 = answ2->second;

    auto answ3 = nameMap.find(name2);
    if (answ3 == nameMap.end()) return false;
    Key key2 = answ3->second;

    auto &elem2 = elemMap[key2];
    auto answ5 = elem2.first.find(key1);
    if (answ5 != elem2.first.end()) return true;
    else return false;
}


void jnp1::poset_clear(unsigned long id) {
    auto posetsIt = posets.find(id);
    if (posetsIt == posets.end()) return;
    else {
        (*posetsIt).second.first.first.clear();
        (*posetsIt).second.first.second.clear();
    }

}

