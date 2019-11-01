#include "poset.h"

int main() {
    long id = poset_new();
    poset_delete(id);
    unsigned long p1;
    p1 = poset_new();
    poset_insert(p1, "A");
    poset_insert(p1, "B");
    poset_insert(p1, "C");
    poset_delete(id);
}
