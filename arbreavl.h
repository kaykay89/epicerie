/* UQAM / Département d'informatique
   INF3105 - Structures de données et algorithmes
   Squelette pour classe générique ArbreAVL<T> pour le Lab6 et le TP2.

   AUTEUR(S):
    1) Charles-Étienne Couture
    2) Kayla Louis
*/

#if !defined(_ARBREAVL___H_)
#define _ARBREAVL___H_
#include <cassert>
#include "pile.h"
#include <iostream>
template <class T>
class ArbreAVL {
  public:
    ArbreAVL();
    ArbreAVL(const ArbreAVL&);
    ~ArbreAVL();

    void inserer(const T&);
    bool contient(const T&) const;
    bool vide() const;
    void vider();
    void enlever(const T&); // non requis pour le TP2.
    int  hauteur();
    int taille() const;
    // Annonce l'existance d'une classe Iterateur.
    class Iterateur;
    
    // Fonctions pour obtenir un itérateur (position dans l'arbre)    
    Iterateur debut() const;
    Iterateur fin() const;
    Iterateur rechercher(const T&) const;
    Iterateur rechercherEgalOuSuivant(const T&) const;
    Iterateur rechercherEgalOuPrecedent(const T&) const;

    // Accès aux éléments de l'arbre via un intérateur.
    const T& operator[](const Iterateur&) const;
    T& operator[](const Iterateur&);
    
    // Copie d'un arbre AVL.
    ArbreAVL& operator = (const ArbreAVL&);

  private:
    struct Noeud{
        Noeud(const T&);
        T contenu;
        int equilibre;
        Noeud *gauche;
        Noeud *droite;
    };
    Noeud* racine;
    int nbNoeuds;

    // Fonctions internes.
    bool inserer(Noeud*&, const T&);
    void rotationGaucheDroite(Noeud*&);
    void rotationDroiteGauche(Noeud*&);
    void vider(Noeud*&);
    void copier(const Noeud*, Noeud*&) const;
    const T& max(Noeud*) const;
    bool enlever(Noeud*&, const T& e);
    int trouverhauteur(Noeud*& n);
  public:
   
    class Iterateur{
      public:
        Iterateur(const ArbreAVL& a);
        Iterateur(const Iterateur& a);
        Iterateur(const ArbreAVL& a, Noeud* c);

        operator bool() const;
        bool operator!() const;
        bool operator==(const Iterateur&) const;
        bool operator!=(const Iterateur&) const;
                
        const T& operator*() const;

        Iterateur& operator++();
        Iterateur operator++(int);
        Iterateur& operator = (const Iterateur&);
        Iterateur& operator--();

      private:
        const ArbreAVL& arbre_associe;
        Noeud* courant;
        Pile<Noeud*> chemin;
        
      friend class ArbreAVL;
    };
};

//-----------------------------------------------------------------------------

template <class T>
ArbreAVL<T>::Noeud::Noeud(const T& c):
contenu(c), equilibre(0), gauche(NULL), droite(NULL)
{
}

template <class T>
ArbreAVL<T>::ArbreAVL() : racine(NULL)
{
    nbNoeuds =0;
}

template <class T>
ArbreAVL<T>::ArbreAVL(const ArbreAVL<T>& autre) : racine(NULL)
{
    copier(autre.racine, racine);
    nbNoeuds = autre.taille();
}

template <class T>
ArbreAVL<T>::~ArbreAVL()
{
    vider(racine);
}

template <class T>
bool ArbreAVL<T>::contient(const T& element) const
{
   
    return ArbreAVL::rechercher(element).courant!=NULL;


}

template <class T>
void ArbreAVL<T>::inserer(const T& element)
{
    if(!contient(element)){
        inserer(racine, element);
           
    }
}

template <class T>
bool ArbreAVL<T>::inserer(Noeud*& noeud, const T& element)
{

    if(noeud==NULL)
    {
        noeud = new Noeud(element);
        nbNoeuds++;
        return true;
    }
    if(element < noeud->contenu){
        if(inserer(noeud->gauche, element))
        {
            noeud->equilibre++;
            if(noeud->equilibre == 0) return false;
            if(noeud->equilibre == 1) return true;
            assert(noeud->equilibre==2);
            if(noeud->gauche->equilibre == -1)
                rotationDroiteGauche(noeud->gauche);
            rotationGaucheDroite(noeud);
        }
        return false;
    }
    else if(noeud->contenu < element){

      
        if(inserer(noeud->droite,element))
        {
            noeud->equilibre--;
            
       
            if(noeud->equilibre == 0) return false;
            if(noeud->equilibre == -1) return true;
            
              assert(noeud->equilibre==-2);
            if(noeud->droite->equilibre == 1){
                rotationGaucheDroite(noeud->droite);
            }    
            rotationDroiteGauche(noeud);
            
        }
             
        return false;
    }

    noeud->contenu = element;  // Mise à jour
    return false;
}

template <class T>
void ArbreAVL<T>::rotationGaucheDroite(Noeud*& racinesousarbre)
{
    
    Noeud *temp = racinesousarbre->gauche;
    int  ea = temp->equilibre;
    int  eb = racinesousarbre->equilibre;
    int  neb = -(ea>0 ? ea : 0) - 1 + eb;
    int  nea = ea + (neb < 0 ? neb : 0) - 1;

    temp->equilibre = nea;
    racinesousarbre->equilibre = neb;
    racinesousarbre->gauche = temp->droite;
    temp->droite = racinesousarbre;
    racinesousarbre = temp;
}

template <class T>
void ArbreAVL<T>::rotationDroiteGauche(Noeud*& racinesousarbre)
{
    
   
    Noeud *temp = racinesousarbre->droite;
    int eb = temp ->equilibre;
    int ea = racinesousarbre->equilibre;
    int nea = -(eb<0 ? eb :0) + 1 + ea;
    int neb = (nea>0 ? nea: 0) + 1 +eb;
    
    temp->equilibre = neb;
    racinesousarbre->equilibre = nea;
    racinesousarbre->droite = temp->gauche;
    temp->gauche = racinesousarbre;
    racinesousarbre = temp;

}

template <class T>
bool ArbreAVL<T>::vide() const
{
    return racine ==NULL ;
}

template <class T>
void ArbreAVL<T>::vider(){
  vider(racine);
      nbNoeuds=0;
}

template <class T>
void ArbreAVL<T>::vider(Noeud*& noeud)
{
    if(noeud!=NULL){
    
        if(noeud->droite!=NULL){
             vider(noeud->droite);
        }    
        if(noeud->gauche != NULL)
             vider(noeud->gauche);    
  
        delete noeud;
        noeud = NULL;
    }else{
        return;
    }
}

template <class T>
void ArbreAVL<T>::copier(const Noeud* source, Noeud*& noeud) const
{
 
   
    if(source == NULL) return; 
    noeud = new Noeud(source->contenu);
    if(source->droite!=NULL){
        copier(source->droite,noeud->droite);
    }
    
    if(source->gauche!=NULL){
        copier(source->gauche,noeud->gauche);
    }
      
}

template <class T>
int  ArbreAVL<T>::hauteur(){
    return trouverhauteur(racine);
}

template <class T>
int ArbreAVL<T>::taille() const
{
    return nbNoeuds;
}

template <class T>
int  ArbreAVL<T>::trouverhauteur(Noeud*& n) 
{
    if(n == NULL)
        return 0;
    
    int hGauche = trouverhauteur(n->gauche);
    int hDroit = trouverhauteur(n->droite);
    if(hGauche>hDroit) return hGauche +1;
    
    return hDroit+1;
    
    
}

template <class T>
const T& ArbreAVL<T>::max(Noeud* n) const
{
    assert(n!=NULL);
    if(n->droite == NULL)
        return n->contenu;
    else
        return max(n->droite);
    
}

// L'enlèvement est optionnel (non requise pour le TP2)
template <class T>
void ArbreAVL<T>::enlever(const T& element)
{
    enlever(racine, element);
}

template <class T>
bool ArbreAVL<T>::enlever(Noeud*& noeud, const T& element)
{
    if(element < noeud->contenu)
    {
        if(enlever(noeud->gauche, element))
        {
            // ...
        }
    }
    else if(element > noeud->contenu)
    {
        // ...
    }
    else if(element == noeud->contenu)
    {
        if (noeud->gauche==NULL && noeud->droite==NULL)
        {
            delete noeud;
            noeud = NULL;
            return true;
        }
        else
        {
            // ...
            return true;
        }
    }
    
}
//----------- L'enlèvement est optionnel (non requise pour le TP2) ----------//

template <class T>
typename ArbreAVL<T>::Iterateur ArbreAVL<T>::debut() const
{
    Iterateur iter(*this);
    iter.courant = racine; 
    if(iter.courant != NULL)
        while(iter.courant->gauche !=NULL){
            iter.chemin.empiler(iter.courant);
            iter.courant=iter.courant->gauche;
        }
    return iter;
}

template <class T>
typename ArbreAVL<T>::Iterateur ArbreAVL<T>::fin() const
{
    return Iterateur(*this);
}

template <class T>
typename ArbreAVL<T>::Iterateur ArbreAVL<T>::rechercher(const T& e) const
{
    Iterateur iter(*this);
    Noeud* n = racine;
    
    while(n){
        if(e < n->contenu){
	    iter.chemin.empiler(n);	
            n = n->gauche;
        }else if(n->contenu <e){
            n = n->droite;
        } else{
	  iter.courant = n;
	  return iter;	
	}
    }
    iter.chemin.vider(); 	
    return iter;
}

template <class T>
typename ArbreAVL<T>::Iterateur ArbreAVL<T>::rechercherEgalOuSuivant(const T& e) const
{
    Iterateur iter(*this);
    Noeud* n = racine;
    
    while(n){
        if(e < n->contenu){
	    iter.chemin.empiler(n);	
            n = n->gauche;
        }else if(n->contenu <e){
            n = n->droite;
        } else{
	  iter.courant = n;
	  return iter;	
	}
    }
    if(!iter.chemin.vide()){
        iter.courant = iter.chemin.depiler();
        while(iter.courant->contenu<e)
            iter++;
    }
        
    iter.chemin.vider(); 	
    return iter;
}

template <class T>
typename ArbreAVL<T>::Iterateur ArbreAVL<T>::rechercherEgalOuPrecedent(const T& e) const
{
    Noeud* n = racine, *dernier = NULL; 
    while(n){
        if(e < n->contenu){
            n = n->gauche;
        }
        else if(n->contenu<e){
            dernier = n; 
            n = n->droite;
        }else{
            return rechercher(e);
        }
    }
    if(dernier!=NULL)
        return rechercher(dernier->contenu);
    return Iterateur(*this);
}

template <class T>
const T& ArbreAVL<T>::operator[](const Iterateur& iterateur) const
{
    assert(&iterateur.arbre_associe == this);
    assert(iterateur.courant);
    return iterateur.courant->contenu;
}

template <class T>
T& ArbreAVL<T>::operator[](const Iterateur& iterateur)
{
    assert(&iterateur.arbre_associe == this);
    assert(iterateur.courant);
    return iterateur.courant->contenu;
}

template <class T>
ArbreAVL<T>& ArbreAVL<T>::operator=(const ArbreAVL& autre) {
  
    if(this==&autre) return *this;
    
    vider();

    copier(autre.racine, racine);
    nbNoeuds = autre.taille();
    return *this;
}

//-----------------------
template <class T>
ArbreAVL<T>::Iterateur::Iterateur(const ArbreAVL& a)
 : arbre_associe(a), courant(NULL)
{
}

template <class T>
ArbreAVL<T>::Iterateur::Iterateur(const ArbreAVL<T>::Iterateur& a)
: arbre_associe(a.arbre_associe)
{
    courant = a.courant;
    chemin = a.chemin;
}

// Pré-incrément
template <class T>
typename ArbreAVL<T>::Iterateur& ArbreAVL<T>::Iterateur::operator++()
{
    assert(courant);
    Noeud* suivant = courant->droite;
    while(suivant){
        chemin.empiler(suivant);
        suivant = suivant->gauche;
    }
    if(!chemin.vide())
        courant = chemin.depiler();
    else
        courant = NULL;
    return *this;
}

// Post-incrément
template <class T>
typename ArbreAVL<T>::Iterateur ArbreAVL<T>::Iterateur::operator++(int)
{
    Iterateur copie(*this);
    operator++();
    return copie;
}

template<class T>
typename ArbreAVL<T>::Iterateur& ArbreAVL<T>::Iterateur::operator--()
{
    assert(courant);
    Noeud* nLimite = courant;
    *this = arbre_associe.debut();
    Noeud* noeud = courant;
    Noeud* precedent;

    while(noeud != nLimite){
        operator++();
        precedent = noeud;
        noeud = courant;        
    }

    if(precedent == NULL)
        courant = NULL;
    else
        courant = precedent;
    return *this;
}

template <class T>
ArbreAVL<T>::Iterateur::operator bool() const
{
    return courant!=NULL;
}

template <class T>
bool ArbreAVL<T>::Iterateur::operator!() const{
    return courant==NULL;
}

template <class T>
bool ArbreAVL<T>::Iterateur::operator==(const Iterateur& o) const{
    assert(&arbre_associe==&o.arbre_associe);
    return courant==o.courant;
}

template <class T>
bool ArbreAVL<T>::Iterateur::operator!=(const Iterateur& o) const{
    assert(&arbre_associe==&o.arbre_associe);
    return courant!=o.courant;
}

template <class T>
const T& ArbreAVL<T>::Iterateur::operator *() const
{
    assert(courant!=NULL);
    return courant->contenu;
}

template <class T>
typename ArbreAVL<T>::Iterateur& ArbreAVL<T>::Iterateur::operator = (const Iterateur& autre){
    assert(&arbre_associe==&autre.arbre_associe);
    courant = autre.courant;
    chemin = autre.chemin;
    return *this;
}

#endif
