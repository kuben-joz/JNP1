#include <iostream>
#include "poset.h"

int main(){
    unsigned long id = jnp1::poset_new();
    jnp1::poset_delete(id);
}
