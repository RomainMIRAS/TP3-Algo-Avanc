/*
  Structures de type graphe
  Structures de donnees de type liste
  (Pas de contrainte sur le nombre de noeuds des  graphes)
*/

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#ifndef __GRAPHE_H__
#define __GRAPHE_H__
#include "graphe.h"
#endif

#include "file.h"
#include "pile.h"

psommet_t chercher_sommet(pgraphe_t g, int label)
{
  psommet_t s;

  s = g;

  while ((s != NULL) && (s->label != label))
  {
    s = s->sommet_suivant;
  }
  return s;
}

parc_t existence_arc(parc_t l, psommet_t s)
{
  parc_t p = l;

  while (p != NULL)
  {
    if (p->dest == s)
      return p;
    p = p->arc_suivant;
  }
  return p;
}

void ajouter_arc(psommet_t o, psommet_t d, int distance)
{
  parc_t parc;

  parc = (parc_t)malloc(sizeof(arc_t));

  if (existence_arc(o->liste_arcs, d) != NULL)
  {
    fprintf(stderr, "ajout d'un arc deja existant\n");
    exit(-1);
  }

  parc->poids = distance;
  parc->dest = d;
  parc->arc_suivant = o->liste_arcs;
  o->liste_arcs = parc;
  return;
}

// ===================================================================

int nombre_sommets(pgraphe_t g)
{
  psommet_t p = g;
  int nb = 0;

  while (p != NULL)
  {
    nb = nb + 1;
    p = p->sommet_suivant;
  }

  return nb;
}

int nombre_arcs(pgraphe_t g)
{

  psommet_t p = g;
  int nb_arcs = 0;

  while (p != NULL)
  {
    parc_t l = p->liste_arcs;

    while (l != NULL)
    {
      nb_arcs = nb_arcs + 1;
      l = l->arc_suivant;
    }

    p = p->sommet_suivant;
  }
  return nb_arcs;
}

void init_couleur_sommet(pgraphe_t g)
{
  psommet_t p = g;

  while (p != NULL)
  {
    p->couleur = 0;        // couleur indefinie
    p = p->sommet_suivant; // passer au sommet suivant dans le graphe
  }

  return;
}

int colorier_graphe(pgraphe_t g)
{
  /*
    coloriage du graphe g

    datasets
    graphe data/gr_planning
    graphe data/gr_sched1
    graphe data/gr_sched2
  */

  psommet_t p = g;
  parc_t a;
  int couleur;
  int max_couleur = INT_MIN; // -INFINI

  int change;

  init_couleur_sommet(g);

  while (p != NULL)
  {
    couleur = 1; // 1 est la premiere couleur

    // Pour chaque sommet, on essaie de lui affecter la plus petite couleur

    // Choix de la couleur pour le sommet p

    do
    {
      a = p->liste_arcs;
      change = 0;

      while (a != NULL)
      {
        if (a->dest->couleur == couleur)
        {
          couleur = couleur + 1;
          change = 1;
        }
        a = a->arc_suivant;
      }

    } while (change == 1);

    // couleur du sommet est differente des couleurs de tous les voisins

    p->couleur = couleur;
    if (couleur > max_couleur)
      max_couleur = couleur;

    p = p->sommet_suivant;
  }

  return max_couleur;
}

void afficher_graphe_largeur(pgraphe_t g, int r)
{

  init_couleur_sommet(g);

  pfile_t file = creer_file();
  psommet_t p = chercher_sommet(g, r);
  parc_t a = p->liste_arcs;

  int distance = 0;
  int nbDistanceAct = 1;
  int nbDistanceNext = 0;

  while (a != NULL)
  {
    nbDistanceNext++;
    a = a->arc_suivant;
  }

  enfiler(file, p);

  p->couleur = 1;

  printf("\n Distance 0 : ");

  while (!file_vide(file))
  {
    p = defiler(file);

    printf(" %d ", p->label);

    nbDistanceAct--;

    if (nbDistanceAct == 0) {
      distance++;
      nbDistanceAct = nbDistanceNext;
      nbDistanceNext = 0;

      printf("\n Distance %d : ", distance);
    }

    a = p->liste_arcs;
    while(a != NULL) {
      if (a->dest->couleur == 0) {
        nbDistanceNext++;
        a->dest->couleur = 1;
        enfiler(file, a->dest);
      }
      a = a->arc_suivant;
    }
  }
  printf("\n");
  return;
}

// void afficher_graphe_profondeur__rec_worker (psommet_t g, int r,int debut, int fin, int* temps)
// {

//   g->couleur = 1; // Gris - En cours de traitement
//   *temps++;

//   return ;
// }

void afficher_graphe_profondeur(pgraphe_t g, int r)
{
  // Pour tout les noeuds du graphe, on met la couleur a 0
  init_couleur_sommet(g);
  ppile_t p = creer_pile();
  psommet_t current = chercher_sommet(g, r);
  empiler(p, current);
  while (!pile_vide(p))
  {
    current = depiler(p);
    printf("Noeud %d\n", current->label);

    if (current->couleur != 2)
    {
      current->couleur = 2;
      parc_t a = current->liste_arcs;
      while (a != NULL)
      {
        psommet_t s = a->dest;
        if (s->couleur == 0)
        {
          s->couleur = 1;
          empiler(p, s);
        }
        a = a->arc_suivant;
      }
    }
  }
}

void relachement(psommet_t u, psommet_t v, int poids)
{
  /*
    relachement de l'arc (u,v) de poids poids
  */
  if (v->distance > u->distance + poids)
  {
    v->distance = u->distance + poids;
    v->pere = u;
  }
  return;
}

void init_sommet(pgraphe_t g)
{
  psommet_t p = g;

  while (p != NULL)
  {
    p->couleur = 0;        // couleur indefinie
    p->distance = INT_MAX; // INFINI
    p->pere = NULL;
    p = p->sommet_suivant; // passer au sommet suivant dans le graphe
  }

  return;
}

void algo_dijkstra(pgraphe_t g, int r)
{
  init_sommet(g);

  pfile_t file = creer_file();
  psommet_t p = chercher_sommet(g, r);
  parc_t a = p->liste_arcs;

  enfiler(file, p);

  p->couleur = 1;
  p->distance = 0;

  while (!file_vide(file))
  {
    p = defiler(file);
    a = p->liste_arcs;
    while (a != NULL)
    {
      if (a->dest->couleur == 0)
      {
        a->dest->couleur = 1;
        enfiler(file, a->dest);
      }
      relachement(p, a->dest, a->poids);
      a = a->arc_suivant;
    }
  }

  printf("Noeud\tDistance\tPère\n");
  p = g;
  while (p != NULL)
  {
    if (p->pere != NULL)
    printf("%d\t%d \t\t%d\n", p->label, p->distance, p->pere->label);
    p = p->sommet_suivant;
  }

  return;
}
void algo_dijkstra_remi(pgraphe_t g, int r)
{
  /*
    algorithme de dijkstra
    des variables ou des chanmps doivent etre ajoutees dans les structures.
  */

  return;
}

// ======================================================================

int degre_sortant_sommet(pgraphe_t g, psommet_t s)
{
  /*
    Cette fonction retourne le nombre d'arcs sortants
    du sommet n dans le graphe g
  */

  return 0;
}

int degre_entrant_sommet(pgraphe_t g, psommet_t s)
{
  /*
    Cette fonction retourne le nombre d'arcs entrants
    dans le noeud n dans le graphe g
  */

  return 0;
}

int degre_maximal_graphe(pgraphe_t g)
{
  /*
    Max des degres des sommets du graphe g
  */

  return 0;
}

int degre_minimal_graphe(pgraphe_t g)
{
  /*
    Min des degres des sommets du graphe g
  */

  return 0;
}

int independant(pgraphe_t g)
{
  /* Les aretes du graphe n'ont pas de sommet en commun */

  return 0;
}

int complet(pgraphe_t g)
{
  /* Toutes les paires de sommet du graphe sont jointes par un arc */

  return 0;
}

int regulier(pgraphe_t g)
{
  /*
     graphe regulier: tous les sommets ont le meme degre
     g est le ponteur vers le premier sommet du graphe
     renvoie 1 si le graphe est régulier, 0 sinon
  */

  return 0;
}

/*
  placer les fonctions de l'examen 2017 juste apres
*/


// ======================================================================

chemin_t creer_chemin(int label){
  chemin_t c = (chemin_t)malloc(sizeof(struct chemin));
  c->label = label;
  c->suivant = NULL;
  return c;
}

void afficher_chemin(chemin_t c){
  printf("Chemin : ");
  while(c != NULL){
    printf("%d ", c->label);
    c = c->suivant;
  }
  printf("\n");
}

void detruire_chemin(chemin_t c){
  chemin_t tmp;
  while(c != NULL){
    tmp = c;
    c = c->suivant;
    free(tmp);
  }
}

chemin_t ajouter_chemin(chemin_t c, int label){
  chemin_t tmp = c;
  while(tmp->suivant != NULL){
    tmp = tmp->suivant;
  }
  tmp->suivant = creer_chemin(label);
}

int est_dans_chemin(chemin_t c, int label){
  while(c != NULL){
    if(c->label == label){
      return 1;
    }
    c = c->suivant;
  }
  return 0;
}
/**
 * Un chemin élémentaire est un chemin ne passant pas deux fois par un même sommet,
 * c’est à dire un chemin dont tous les sommets sont distincts.
*/
int elementaire (pgraphe_t g, chemin_t c){
  chemin_t tmp = creer_chemin(c->label);
  while(c->suivant != NULL){
    if(est_dans_chemin(tmp, c->suivant->label)){
      return 0;
    }
    tmp = ajouter_chemin(tmp, c->suivant->label);
    c = c->suivant;
  }
}

int getNbArcs(pgraphe_t g){
  int nb_arcs = 0;
  psommet_t p = g;
  parc_t a;
  while(p != NULL){
    a = p->liste_arcs;
    while(a != NULL){
      nb_arcs++;
      a = a->arc_suivant;
    }
    p = p->sommet_suivant;
  }
  return nb_arcs;
}

/**
 * Un chemin est dit Eulérien si tous les arcs du graphe sont utilisés dans le chemin.
*/
int eulerien (pgraphe_t g, chemin_t c){
  int nb_arcs = 0;
  while(c->suivant != NULL){
    if(!est_dans_chemin(c->suivant, c->label)){
      return 0;
    }
    nb_arcs++;
    c = c->suivant;
  }
  if(nb_arcs == getNbArcs(g)){
    return 1;
  }
  return 0;
}

/**
 * Un graphe est dit Eulérien si il existe au moins un chemin qui soit Eulérien.
 * renvoie 1 si le graphe est eulérien, 0 sinon
*/
int graphe_eulerien (pgraphe_t g){
  psommet_t p = g;
  chemin_t c;
  while(p != NULL){
    c = creer_chemin(p->label);
    if(eulerien(g, c)){
      return 1;
    }
    p = p->sommet_suivant;
    ajouter_chemin(c, p->label);
  }
  return 0;
}

int distance (pgraphe_t g, int x, int y){
  psommet_t p = g;
  while(p != NULL){
    if(p->label == x){
      parc_t a = p->liste_arcs;
      while(a != NULL){
        if(a->dest->label == y){
          return a->poids;
        }
        a = a->arc_suivant;
      }
    }
    p = p->sommet_suivant;
  }
  return -1;
}


int excentricite (pgraphe_t g, int x){
  int max = 0;
  psommet_t p = g;
  while(p != NULL){
    if(p->label != x){
      int d = distance(g, x, p->label);
      if(d > max){
        max = d;
      }
    }
    p = p->sommet_suivant;
  }
  return max;
}

int diametre (pgraphe_t g){
  int max = 0;
  psommet_t p = g;
  while(p != NULL){
    int e = excentricite(g, p->label);
    if(e > max){
      max = e;
    }
    p = p->sommet_suivant;
  }
  return max;
}
