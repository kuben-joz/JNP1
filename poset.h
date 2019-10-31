#ifndef POSET_H
#define POSET_H

//dont need to do this but maybe better we do
#ifndef __cplusplus
#include "stdbool.h"
#endif

#include "stddef.h"

#ifdef __cplusplus
namespace jnp1 {
#endif

#ifdef __cplusplus
extern "C" {
#endif



/**
 * Tworzy nowy poset i zwraca jego identyfikator.
 * @return
 */
unsigned long poset_new(void);

/**
 * Jeżeli istnieje poset o identyfikatorze id, usuwa go, a w przeciwnym
 * przypadku nic nie robi.
 * @param id
 */
void poset_delete(unsigned long id);

/**
 * Jeżeli istnieje poset o identyfikatorze id, to wynikiem jest liczba jego
 * elementów, a w przeciwnym przypadku 0.
 * @param id
 * @return
 */
size_t poset_size(unsigned long id);

/**
 * Jeżeli istnieje poset o identyfikatorze id i element value nie należy do
 * tego zbioru, to dodaje element do zbioru, a w przeciwnym przypadku nic nie
 * robi. Nowy element nie jest w relacji z żadnym elementem. Wynikiem jest
 * true, gdy element został dodany, a false w przeciwnym przypadku.
 * @param id
 * @param value
 * @return
 */
bool poset_insert(unsigned long id, char const *value);

/**
 * Jeżeli istnieje poset o identyfikatorze id i element value należy do tego
 * zbioru, to usuwa element ze zbioru oraz usuwa wszystkie relacje tego
 * elementu, a w przeciwnym przypadku nic nie robi. Wynikiem jest true, gdy
 * element został usunięty, a false w przeciwnym przypadku.
 * @param id
 * @param value
 * @return
 */
bool poset_remove(unsigned long id, char const *value);

/**
 * Jeżeli istnieje poset o identyfikatorze id oraz elementy value1 i value2
 * należą do tego zbioru i nie są w relacji, to rozszerza relację w taki
 * sposób, aby element value1 poprzedzał element value2 (domyka relację
 * przechodnio), a w przeciwnym przypadku nic nie robi. Wynikiem jest true,
 * gdy relacja została rozszerzona, a false w przeciwnym przypadku.
 * @param id
 * @param value1
 * @param value2
 * @return
 */
bool poset_add(unsigned long id, char const *value1, char const *value2);

/**
 * Jeżeli istnieje poset o identyfikatorze id, elementy value1 i value2
 * należą do tego zbioru, element value1 poprzedza element value2 oraz
 * usunięcie relacji między elementami value1 i value2 nie zaburzy warunków
 * bycia częściowym porządkiem, to usuwa relację między tymi elementami,
 * a w przeciwnym przypadku nic nie robi. Wynikiem jest true, gdy relacja
 * została zmieniona, a false w przeciwnym przypadku.
 * @param id
 * @param value1
 * @param value2
 * @return
 */
bool poset_del(unsigned long id, char const *value1, char const *value2);

/**
 * Jeżeli istnieje poset o identyfikatorze id, elementy value1 i value2
 * należą do tego zbioru oraz element value1 poprzedza element value2, to
 * wynikiem jest true, a w przeciwnym przypadku false.
 * @param id
 * @param value1
 * @param value2
 * @return
 */
bool poset_test(unsigned long id, char const *value1, char const *value2);

/**
 * Jeżeli istnieje poset o identyfikatorze id, usuwa wszystkie jego elementy
 * oraz relacje między nimi, a w przeciwnym przypadku nic nie robi.
 * @param id
 */
void poset_clear(unsigned long id);

#ifdef __cplusplus
} // extern "C"
#endif

#ifdef __cplusplus
} // namespace jnp1
#endif

#endif //POSET_H