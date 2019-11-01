#include <iostream>
#include <assert.h>
#include "poset.h"



int main(){
    unsigned long id = jnp1::poset_new();
    jnp1::poset_delete(id);
    unsigned long p1;
    p1 = jnp1:: poset_new();
    jnp1::poset_insert(p1, "A");
    jnp1::poset_insert(p1, "B");
    jnp1::poset_insert(p1, "C");
    assert(jnp1::poset_add(p1, "A", "B"));
    assert(jnp1::poset_add(p1, "B", "C"));
    //assert(!jnp1::poset_add(p1, "C", "A"));
    assert(jnp1::poset_test(p1, "A", "C"));
}
