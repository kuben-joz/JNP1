#include "poset.h"
#include <unordered_map>
#include <unordered_set>
#include <cassert>
#include <iostream>

#ifndef NDEBUG
const bool debug = true;
#else
const bool debug = false;
#endif

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
    assert(!poset.first.second.count(name));
    ElemMap &tempElems = poset.first.first;
    NameMap &tempNames = poset.first.second;

    auto tempNameIter = tempNames.find(name);

    if (tempNameIter == tempNames.end()) {
        PosetInternalCounter &counter = poset.second;
        while (tempElems.count(++counter));
        tempElems.emplace(counter, Elem{});
        //tempElems[counter].first.emplace(counter);
        //tempElems[counter].second.emplace(counter);
        tempNames.emplace(name, counter);
    }
}



unsigned long jnp1::poset_new(void) {
    if(debug) {
        std::cerr << "poset_new()\n";
    }
    while (posets.count(++posetCounter));
    posets.emplace(posetCounter, Poset{});
    assert(posets.count(posetCounter));
    if(debug) {
        std :: cerr << "poset_new: created poset with id " << posetCounter << "\n";
    }
    return posetCounter;
}

void jnp1::poset_delete(const PosetContainerIndexType id) {
    if(debug) {
        std::cerr << "poset_delete(" << id << ")\n";
    }
    auto toDelete = posets.find(id);
    if (toDelete != posets.end()) {
        posets.erase(toDelete);
        if(debug) {
            std::cerr << "poset_delete: poset " << id << " deleted\n";
        }
    }
    else if(debug){ //toDelete == posets.end()
        std :: cerr << "poset_delete: poset " << id << " not found\n";
    }
}

size_t jnp1::poset_size(const PosetContainerIndexType id) {
    if(debug) {
        std::cerr << "poset_size(" << id << ")\n";
    }
    auto answ1 = posets.find(id);
    if (answ1 == posets.end()) {
        if(debug) {
            std::cerr << "poset_size: poset " << id << " not found\n";
        }
        return 0;
    }
    if(debug) {
        std::cerr << "poset_size: found poset " << id << " of size " << (*answ1).second.first.second.size() << "\n";
    }
    return (*answ1).second.first.second.size();
}

bool jnp1::poset_insert(const PosetContainerIndexType id, char const *value) {
    if(value == NULL) {
        if(debug) {
            std::cerr << "poset_insert(" << id << ", NULL)\n";
            std::cerr << "poset_insert: Illegal value (NULL)\n";
        }
        return false;
    }
    std::string name(value);
    if(debug) {
        std::cerr << "poset_insert(" << id << ", " << name << ")\n";
    }
    auto answ1 = posets.find(id);
    if (answ1 == posets.end()) {
        if(debug) {
            std::cerr << "poset_insert: poset " << id << " not found\n";
        }
        return false;
    }
    Poset& poset = answ1->second;
    auto answ2 = poset.first.second.find(name);
    if (answ2 == poset.first.second.end()) {
        addElem(poset, name);
        if(debug) {
            std::cerr << "poset_insert: value " << name << " successfully added to poset\n";
        }
        return true;
    }
    if(debug) {
        std::cerr << "poset_insert: poset " << id <<
        " already contains value " << name << "\n";
    }
    return false;
}

bool jnp1::poset_remove(const unsigned long id, char const *value) {
    if(value == NULL) {
        if (debug) {
            std::cerr << "poset_remove(" << id << ", " << "NULL)\n";
            std::cerr << "poset_remove: invalid value (NULL)\n";
        }
        return false;

    }

    std::string valueString(value);
    if (debug) {
        std::cerr << "poset_remove(" << id << ", " << valueString <<")\n";
    }

    std::unordered_map<PosetContainerIndexType, Poset>::iterator posetsIt = posets.find(id);
    if (posetsIt == posets.end()) {
        if(debug) {
            std::cerr << "poset_remove: poset " << id << " not found\n";
        }
        return false;

    }
    PosetBody &currentPosetBody = (*posetsIt).second.first;
    NameMap::const_iterator nameMapIt = currentPosetBody.second.find(valueString);
    if (nameMapIt == currentPosetBody.second.end()) {
        if(debug) {
            std::cerr << "poset_remove: poset " << id << " does not contain value " << valueString << "\n";
        }
        return false;
    }

    const Key keyToRemove = (*nameMapIt).second;
    ElemMap::iterator elemMapIt = currentPosetBody.first.find(keyToRemove);
    // Asserts iff an element that was in NameMap is also present in ElemMap,
    // which is an invariant of PosetBody
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
    currentPosetBody.second.erase(valueString);

    if(debug) {
        std::cerr << "poset_remove: value " << valueString << " has been removed from poset " << id << "\n";
    }

    return true;
}

//// assertions finished here
bool loopCheck(ElemMap &elemMap, Elem &elem1, Elem &elem2) {
    for (const auto &lowerElemKey: elem1.first) {
        for (const auto &upperElemKey: elem2.second) {
            if (elemMap[lowerElemKey].first.find(upperElemKey) !=
                elemMap[lowerElemKey].first.end())
                if(debug){
                  std::cerr << "loopCheck : found loop";
                }
                return true;
        }
    }
    return false;
}

bool jnp1::poset_add(unsigned long id, char const *value1, char const *value2) {

    if(value1 == NULL || value2 == NULL) {
        return false;
    }
    //value1 < value2
    std::string name1(value1);
    std::string name2(value2);

    auto answ1 = posets.find(id);
    if (answ1 == posets.end()) return false;
    auto &elemMap = answ1->second.first.first; //dlaczego & żeby nie kopiować
    auto &nameMap = answ1->second.first.second; //dlaczego &  całego mapa

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

    elem1.second.emplace(key2);

    for (const auto &upperElemKey: elem2.second) {
      elem1.second.emplace(upperElemKey);
      elemMap[upperElemKey].first.emplace(key1);
    }

    elem2.first.emplace(key1);

    for (const auto &lowerElemKey: elem1.first) {
      elem2.first.emplace(lowerElemKey);
      elemMap[lowerElemKey].second.emplace(key1);
    }

    //element should be in smaller bigger set of itself
    //it solves a lot of problems

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
    if(value1 == NULL || value2 == NULL) {
        return false;
    }
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

    if (answ5 == elem1.first.end())
        return false;

    if (!isDetachable(elemMap, elem1, elem2, key1, key2))
        return false;

    elem1.second.erase(key2);
    for (const auto &upperElemKey: elem2.second) {
      elem1.second.erase(upperElemKey);
      elemMap[upperElemKey].first.emplace(key1);
    }

    elem2.first.erase(key1);
    for (const auto &upperElemKey: elem2.second) {
      elem1.second.erase(upperElemKey);
      elemMap[upperElemKey].first.emplace(key1);
    }

    for (const auto &lowerElemKey: elem1.first) {
        for (const auto &upperElemKey: elem2.second) {
            elemMap[lowerElemKey].second.erase(upperElemKey);
            elemMap[upperElemKey].first.erase(lowerElemKey);
        }
    }
    return true;
}

/*
 * TODO
 * make sure realtions are present, transitivty not working
 */

bool jnp1::poset_test(unsigned long id, char const *value1, char const *value2) {

    if(value1 == NULL || value2 == NULL) {
        return false;
    }
    //czy value1 < value2
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
    if(debug) {
        std::cerr << "poset_clear(" << id << ")\n";
    }
    auto posetsIt = posets.find(id);
    if (posetsIt == posets.end()) {
        if(debug) {
            std::cerr << "poset_clear: poset " << id << " not found\n";
        }
    }
    else {
        (*posetsIt).second.first.first.clear();
        (*posetsIt).second.first.second.clear();
        if(debug) {
            std::cerr << "poset_clear: poset " << id << " has been cleared\n";
        }
    }

}
