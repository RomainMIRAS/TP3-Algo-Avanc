#include <stdlib.h>
#include "pile.h"

ppile_t creer_pile ()
{
  ppile_t p = (ppile_t) malloc (sizeof (pile_t));
  if (p == NULL)
    return NULL;

  //Set tab à nulle
  for (int i = 0; i < MAX_PILE_SIZE; i++)
    p->Tab[i] = NULL;
  p->sommet = 0;
  return p;
}

int detruire_pile (ppile_t p)
{
  // Free tab
  for (int i = 0; i < MAX_PILE_SIZE; i++)
    free (p->Tab[i]);
  // Free pile

  free (p);
  return 0;
}  

int pile_vide (ppile_t p)
{
  return p->sommet == 0;
}

int pile_pleine (ppile_t p)
 {
  return p->sommet == MAX_PILE_SIZE;
  } 

psommet_t depiler (ppile_t p)
{
  if (pile_vide (p))
    return NULL;
  p->sommet--;
  return p->Tab[p->sommet];
}

int empiler (ppile_t p, psommet_t pn)
  {
  if (pile_pleine (p))
    return -1;
  p->Tab[p->sommet] = pn;
  p->sommet++;
  return 0;
  }
