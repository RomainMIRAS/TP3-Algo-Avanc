#include <stdlib.h>

#include "file.h"

pfile_t creer_file ()
{
  pfile_t f = (pfile_t) malloc (sizeof (file_t));
  if (f == NULL)
    return NULL;

  //Set tab à nulle
  for (int i = 0; i < MAX_FILE_SIZE; i++)
    f->Tab[i] = NULL;
  f->tete = 0;
  f->queue = 0;
  return f;

}

int detruire_file (pfile_t f)
{  
  if (f == NULL)
    return -1;
  free (f);
  return 0;
}




int file_vide (pfile_t f)
{
  return (f->tete == f->queue);
}

int file_pleine (pfile_t f)
  {
  return (f->tete == (f->queue + 1) % (MAX_FILE_SIZE+1));
  }

psommet_t defiler (pfile_t f)
  {
  if (file_vide (f))
    return NULL;
  f->tete = (f->tete + 1) % (MAX_FILE_SIZE+1);
  return f->Tab[f->tete];
  }

int enfiler (pfile_t f, psommet_t p)
{
  if (file_pleine (f))
    return -1;
  f->queue = (f->queue + 1) % (MAX_FILE_SIZE+1);
  f->Tab[f->queue] = p;
  return 0;
}
