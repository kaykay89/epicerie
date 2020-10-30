/*  AUTEUR(S):
    1) Charles-Étienne Couture COUC13049004
    2) Kayla Louis
*/
#if !defined(_EPICERIE__H_)
#define _EPICERIE__H_

#include <string>
#include "arbremap.h"
#include "pointst.h"
#include "date.h"
#include <sstream>
#define SSTR( x ) static_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()
// from https://stackoverflow.com/questions/5590381/easiest-way-to-convert-int-to-string-in-c

using namespace std;



class Epicerie{
  public:

      string nom;

      Epicerie();
      Epicerie(const PointST& point,const ArbreMap<string,ArbreMap<Date,int> > inventaire_);
      Epicerie(const PointST& point,string nom_);
      Epicerie(const Epicerie& e);
      
      
      void copieInventaire(ArbreMap<string,ArbreMap<Date,int> >& inventaire_);
      void setInventaire(Date);
      void setInventaire(const string nomProduit,const Date expiration, const int nbItems);
      int quantityAvailable(const string productName);
      void calculerDistanceInterEpicerie(Epicerie& autreEpicerie);
      string inventaireToString();
      ArbreMap<string,ArbreMap<Date,int> >  getInventaire();
      PointST getLocation();
      double getDistance(Epicerie&);
      ArbreMap<string,double> distanceInterEpeceries;
      
      bool operator < (const Epicerie& e) const{return nom < e.nom;}

  private:
      PointST location;
      ArbreMap<string,ArbreMap<Date,int> > inventaire;//<nomProduit,<ExpirationDate,NumberInInventory>>
     int productQuantity(ArbreMap<Date,int> productInventory);
};

class Commande{
  public:
      Commande();
     void initialiser(const PointST position_, const int maxEpiceries_, const double maxDistance_,
                         ArbreMap<string,Epicerie>& epiceries_,const Date maintenant_);
     void initialiser(ArbreMap<string,Epicerie>&, const Date);
     void recommander();
     ArbreMap<string,Epicerie> ramasser(Epicerie);
    void afficherRamasser();  
              
  private:
    // À compléter
      PointST position;
      int maxEpiceries,nbItemsStillNeeded;
      double maxDistance,distanceTotal;
      ArbreMap<string,int> liste;//nomProduit, quantité souhaité
      ArbreMap<string,Epicerie> epiceries;
      Date maintenant;
      
      //fonctions privees
      void setListe(string, int);
      string plusProcheEpicerie(ArbreMap<string,Epicerie>& parcours,ArbreMap<string,Epicerie>& parcouru, PointST point);
      bool valideDistance(ArbreMap<string,Epicerie>& parcours);
      bool valideQuantiteItem(ArbreAVL<Epicerie>&);
      void debugPoolEpicerie (ArbreMap<int, ArbreAVL<Epicerie> >& ensembleDesParcours);
      ArbreMap<int, ArbreAVL<Epicerie> > getSousEnsemblesValides
                    (ArbreMap<int, ArbreAVL<Epicerie> >);
      ArbreMap<double,string> calculer(ArbreAVL<Epicerie>&,PointST&);
      ArbreMap<double,string> calculeDistances();
      ArbreAVL<Epicerie>tousSaufElement(ArbreAVL<Epicerie>&, Epicerie&);

      double parcoursEpiceriesPlusProchent(ArbreMap<double,string>& distances);
      bool valideEpicerie( Epicerie& epicerie);
      bool chercherDansInventaire(const ArbreMap<string,ArbreMap<Date,int> > inventaire, 
        const ArbreMap<string,int>::Iterateur item);
      double trouveChemin(ArbreMap<string,Epicerie>& epicerieVisite,
            ArbreMap<double,string>& distances);
      bool valideSousEnsemble(ArbreAVL<Epicerie>&);
      int quantiteDisponible(Epicerie& epicerie,ArbreMap<string,int>::Iterateur itemDansListe);
      ArbreMap<double, string> CalculeDistances(ArbreMap<int, ArbreAVL<Epicerie> >&);
      ArbreMap<int, ArbreAVL<Epicerie> > EnsembleDesParcours(ArbreAVL<Epicerie>&);
      ArbreAVL<Epicerie> poolEpiceries(ArbreMap<string, Epicerie>& e);
      friend istream& operator>>(istream& is, Commande& c);
};
#endif
