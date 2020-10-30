/*  INF3105 - Structures de données et algorithmes
    UQAM | Faculté des sciences | Département d'informatique
    Automne 2017 | Lab8 + TP2
    http://ericbeaudry.uqam.ca/INF3105/lab8/
    http://ericbeaudry.uqam.ca/INF3105/tp2/

    AUTEUR(S):
     (1) Charles-Etienne Couture COUC13049004
     (2) Kayla Louis
*/
#if !defined(_ARBREMAP___H_)
#define _ARBREMAP___H_

#include "arbreavl.h"

template <class K, class V>
class ArbreMap
{
  private:
    class Entree {
    public:
        Entree(const K& c):cle(c), valeur(){}
	    K cle;
	    V valeur;
	    bool operator < (const Entree& e) const { return cle < e.cle; }
    };
    ArbreAVL<Entree> entrees;

  public:
    bool contient(const K&) const;
    int taille(); 
    void enlever(const K&);
    void vider();
    bool vide() const;
    
    const V& operator[] (const K&) const;
    V& operator[] (const K&);
    
    class Iterateur{
        public:
            Iterateur(ArbreMap& a):iter(a.entrees.debut()){}
            Iterateur(typename ArbreAVL<Entree>::Iterateur i):iter(i){}
            operator bool()const {return iter.operator bool();};
            Iterateur& operator++(){++iter;return *this;}
            Iterateur& operator--(){--iter; return *this;}
            const K& cle() const{return (*iter).cle;}
            const V& valeur() {return(V&)(*iter).valeur;}
    private:
            typename ArbreAVL<Entree>::Iterateur iter;
    };
    Iterateur debut() {return Iterateur(*this);}
    Iterateur fin() {return Iterateur(entrees.fin());}
    Iterateur rechercher(const K& cle){return Iterateur(entrees.rechercher(cle));}
    Iterateur rechercherEgalOuSuivant(const K& cle) {return
    Iterateur(entrees.rechercherEgalOuSuivant(cle));}
    Iterateur rechercherEgalOuPrecedent(const K& cle) {return
    Iterateur(entrees.rechercherEgalOuPrecedent(cle));}
   
};



template <class K, class V>
void ArbreMap<K,V>::vider(){
    entrees.vider();
}

template <class K, class V>
bool ArbreMap<K,V>::vide() const{
    return entrees.vide();
}

template <class K, class V>
void ArbreMap<K,V>::enlever(const K& c)
{
	entrees.enlever(c);
        //taille--;
}

template <class K, class V>
bool ArbreMap<K,V>::contient(const K& c) const
{
    
    return entrees.contient(c);
}

template <class K, class V>
int ArbreMap<K,V>::taille() 
{
    return entrees.taille();
}

template <class K, class V>
const V& ArbreMap<K,V>::operator[] (const K& c)  const
{
    typename ArbreAVL<Entree>::Iterateur iter=entrees.rechercher(c);
    return entrees[iter].valeur; 
}

template <class K, class V>
V& ArbreMap<K,V>::operator[] (const K& c) 
{
    typename ArbreAVL<Entree>::Iterateur iter = entrees.rechercher(Entree(c));
	if(!iter){
		entrees.inserer(Entree(c));
		iter = entrees.rechercher(c);
	}
    return entrees[iter].valeur;
}



#endif

