/*  AUTEUR(S):
    1) Charles-Étienne Couture COUC13049004
 * 2) Kayla Louis LOUK13628902
*/
#include <iostream>
#include "epicerie.h"
#include <limits>
#include <math.h> 

Epicerie::Epicerie()
{
}

Epicerie::Epicerie(const PointST& point, const ArbreMap<string,ArbreMap<Date,int> > inventaire_)
    :location(point),inventaire(inventaire_)
{
}

Epicerie::Epicerie(const Epicerie& e )
    :nom(e.nom), location(e.location)
{
    
     this->inventaire = e.inventaire;
}

Epicerie::Epicerie(const PointST& point, string nom_)
    :nom(nom_), location(point)
{
}



void Epicerie::copieInventaire(ArbreMap<string,ArbreMap<Date,int> >& inventaire_){
    for(ArbreMap<string,ArbreMap<Date,int> >::Iterateur iter = inventaire_.debut();iter;++iter)
    {
        inventaire[iter.cle()]=iter.valeur();
    }
}

void Epicerie::setInventaire(const string nomProduit,const Date expiration, const int nbItems)
{
    ArbreMap<Date,int> expirationEtQuantite = inventaire[nomProduit];
    if(expirationEtQuantite.contient(expiration)){
        inventaire[nomProduit][expiration]+=nbItems;
    }else{
        inventaire[nomProduit][expiration]=nbItems;
    }
        
    
}

void Epicerie::setInventaire(Date nouvelleDate)
{

    ArbreMap<string, ArbreMap<Date,int> >::Iterateur iterI= inventaire.debut();
    while(iterI){
        ArbreMap<Date,int> d = iterI.valeur();
        ArbreMap<Date,int>::Iterateur iterD = d.debut();
        while(iterD){
            if(iterD.cle() <= nouvelleDate){
                setInventaire(iterI.cle(), iterD.cle(), -iterD.valeur());           
            }
            ++iterD;
        }
        ++iterI;
    }
}

int Epicerie::quantityAvailable(const string productName){
    
    if(!(inventaire.contient(productName))){
        return 0;
    }
    
    return productQuantity(inventaire[productName]);
}

int Epicerie::productQuantity( ArbreMap<Date,int> productInventory){
   
    ArbreMap<Date,int>::Iterateur iter = productInventory.debut();
    int quantity =0;
    while(iter ){
        
        quantity += iter.valeur();   
        ++iter;
    }
    
    return quantity;
}

void Epicerie::calculerDistanceInterEpicerie(Epicerie& autreEpicerie){
    PointST autreLocation = autreEpicerie.getLocation();
    distanceInterEpeceries[autreEpicerie.nom]=location.distance(autreLocation);
} 

 double Epicerie::getDistance(Epicerie& epicerie){
     return distanceInterEpeceries[epicerie.nom];
 }

string Epicerie::inventaireToString()
{
    string availableFood = "";
    ArbreMap<string,ArbreMap<Date,int> >::Iterateur iter = inventaire.debut();
    while(iter){
        ArbreMap<Date,int> temp = iter.valeur();
        ArbreMap<Date,int>::Iterateur iterDate = temp.debut();
        
        int compteur = 0;
        while(iterDate){
            compteur +=iterDate.valeur();
            ++iterDate;
        }
        if(compteur > 0){    
            availableFood.append(iter.cle()+" "+SSTR(compteur)+"\t");
        }    
        ++iter;
    }
    availableFood.append(";");
    return availableFood;
}

ArbreMap<string,ArbreMap<Date,int> >  Epicerie::getInventaire(){
    return inventaire;
}

PointST Epicerie::getLocation()
{
    return location;
}

Commande::Commande()
{
}

void Commande::initialiser(const PointST position_, const int maxEpiceries_, const double maxDistance_,
                         ArbreMap<string,Epicerie>& epiceries_,const Date maintenant_)
{
    position = position_;
    maxEpiceries=maxEpiceries_;
    maxDistance=maxDistance_;
    distanceTotal =  std::numeric_limits<double>::max();
    epiceries=epiceries_;
    maintenant = maintenant_;
    
    
 
    
}

void Commande::initialiser(ArbreMap<string,Epicerie>& epiceries_,const Date maintenant_)
{
    epiceries = epiceries_;
    maintenant = maintenant_;
}

ArbreMap<string,Epicerie> Commande::ramasser(Epicerie epicerie)
{
    for(ArbreMap<string, int>::Iterateur iterCommande = liste.debut();iterCommande;++iterCommande){
        int quantite = iterCommande.valeur();
        string nomItem = iterCommande.cle();
       
        if(epicerie.getInventaire().contient(nomItem)){ 
            ArbreMap<Date,int> p = epicerie.getInventaire().rechercher(nomItem).valeur();
            ArbreMap<Date,int>::Iterateur iterProduit = p.debut();
            ArbreMap<Date, int>::Iterateur iterProduit2 = p.debut();
            ++iterProduit2;
            while(iterProduit2){
                ++iterProduit;
                ++iterProduit2;
            }
            while(iterProduit && quantite>0){
                if(quantite < iterProduit.valeur()){
                    liste[nomItem] =0;
                    epiceries[epicerie.nom].setInventaire(nomItem, iterProduit.cle(), -quantite);
                    quantite = 0;
                }else if(iterProduit.valeur() < quantite){
                    epiceries[epicerie.nom].setInventaire(nomItem, iterProduit.cle(), -iterProduit.valeur());
                    quantite = quantite - iterProduit.valeur();
                    liste[nomItem] = quantite;
                }else if(quantite == iterProduit.valeur()){
                    quantite = 0; 
                    liste[nomItem] = 0;
                    epiceries[epicerie.nom].setInventaire(nomItem, iterProduit.cle(), -iterProduit.valeur());
                }
               --iterProduit;
            }
        }
    }
    return epiceries;   

}

void Commande::afficherRamasser(){
    int compt = 0;
    for(ArbreMap<string,int>::Iterateur iter = liste.debut(); iter; ++iter){
        if(iter.valeur() > 0)
            compt++;
    }
    if(compt > 0){
        cout << "MANQUE" << " ";
        for(ArbreMap<string,int>::Iterateur i = liste.debut(); i; ++i){
            if(0<i.valeur()){
                cout << i.cle() << " " << i.valeur() << "\t";
            }
        }
        cout << ";";
    }else{
        cout << "COMPLET";
    }
}

void Commande::recommander(){
    
  
    
    ArbreAVL<Epicerie> pool = poolEpiceries(epiceries);
        
       
        ArbreMap<int, ArbreAVL<Epicerie> > ensembleDesParcours = EnsembleDesParcours(pool); 
         
        
        ArbreMap<int, ArbreAVL<Epicerie> > ensembleDesParcoursValide = 
                                                getSousEnsemblesValides(ensembleDesParcours);
       
       if(ensembleDesParcoursValide.taille() !=0){ 
         ArbreMap<double,string> distances = CalculeDistances(ensembleDesParcoursValide);
      
        }else{
        cout<<"IMPOSSIBLE";               
        } 
     
}

ArbreMap<double,string> Commande::CalculeDistances(ArbreMap<int, ArbreAVL<Epicerie> >& ensembleDesParcours)
{
    ArbreMap<double,string> distances;
    double minDistance = std::numeric_limits<double>::max();;
    string message;
    ArbreMap<int, ArbreAVL<Epicerie> >::Iterateur iter= ensembleDesParcours.debut();
    while(iter){
  
        ArbreAVL<Epicerie> sousSouensmble = iter.valeur();
       
 
 
        distances = calculer(sousSouensmble,position);
        ArbreMap<double,string>::Iterateur distance = distances.debut();
        if( minDistance>distance.cle()){
            minDistance = round(distance.cle());
            message ="m"+distance.valeur();
        }
       ++ iter;
    }
    
    if(minDistance<=maxDistance){
        cout<<minDistance<<message;
    }else{
         cout<<"IMPOSSIBLE";
    }
    return distances;
}



ArbreMap<double,string> Commande::calculer(ArbreAVL<Epicerie>& sousEnsemble,PointST& point){
    
    ArbreMap<double,string> distances;
    ArbreAVL<Epicerie>::Iterateur unElement = sousEnsemble.debut();  
    
    if(sousEnsemble.taille() ==1){
 
        Epicerie epi = (*unElement);
        PointST pointTemp = epi.getLocation();
        double  distance =  point.distance(pointTemp);
        distance = distance + pointTemp.distance(position);
        distances[distance]=" " + epi.nom;
        return distances;
    }
    
    
    while(unElement){
        Epicerie temp = *unElement;
   
        ArbreAVL<Epicerie> tousSaufEl = tousSaufElement(sousEnsemble,temp);
        PointST tempPoint = temp.getLocation();
        ArbreMap<double,string> distancesSansEl = calculer(tousSaufEl,tempPoint);
        ArbreMap<double,string>::Iterateur j = distancesSansEl.debut();
        while(j){
            
            double nouvelleDistance = point.distance(temp.getLocation());
            double anciennceDistance = j.cle();
            double total = nouvelleDistance + anciennceDistance;
            
            distances[total] = j.valeur()+" "+ temp.nom;
            ++j;
        }
        
        ++unElement;
    }
    
    return distances;
}
 
ArbreAVL<Epicerie>Commande::tousSaufElement(ArbreAVL<Epicerie>& sousEnsemble, Epicerie& epicerie){
    ArbreAVL<Epicerie> tousSaufE;
    ArbreAVL<Epicerie>::Iterateur iter = sousEnsemble.debut();
    while(iter){
        Epicerie temp = *iter;
        if(epicerie.nom.compare(temp.nom)!=0){
            tousSaufE.inserer(temp);
        }
        ++iter;
    }
    return tousSaufE;
}

ArbreMap<int, ArbreAVL<Epicerie> > Commande::getSousEnsemblesValides
                    (ArbreMap<int, ArbreAVL<Epicerie> > ensembleDesParcours){
   
    ArbreMap<int, ArbreAVL<Epicerie> > ensembleDesParcoursValide;
            int compteur = 0; 
            ArbreMap<int, ArbreAVL<Epicerie> >::Iterateur iter = ensembleDesParcours.debut();
            ++iter;
            
            while(iter){
                
                ArbreAVL<Epicerie> temp = iter.valeur();
                
                if(valideSousEnsemble(temp)){ 
                    ensembleDesParcoursValide[compteur]=iter.valeur();
                }
                
                compteur++;
                ++iter;
            }
            
    return ensembleDesParcoursValide;
}

bool Commande::valideSousEnsemble(ArbreAVL<Epicerie>& sousEnsemble ){
    
    if(sousEnsemble.taille()>maxEpiceries)
        return false;
   
    if(!valideQuantiteItem(sousEnsemble)){
        return false;
    }
    
     return true;  
    
}

/*void Commande::debugPoolEpicerie (ArbreMap<int, ArbreAVL<Epicerie> > ensembleDesParcours){
    
    for(ArbreMap<int, ArbreAVL<Epicerie> >::Iterateur iter = ensembleDesParcours.debut();iter;++iter){
        cout<<"sous-ensemble: "<<iter.cle()<< " ";
        ArbreAVL<Epicerie> temp = iter.valeur();
        if(temp.vide())
            cout<<"un arbre vide";
        for(ArbreAVL<Epicerie>::Iterateur j = iter.valeur().debut(); j; ++j ){
            cout<<"{ ";
            Epicerie epicerie = *(j);
            cout<<epicerie.nom<<" ";
           
        }
        cout<<"}";
    }
}
*/


ArbreAVL<Epicerie> Commande::poolEpiceries(ArbreMap<string, Epicerie>& e)
{
    ArbreAVL<Epicerie> pool;
    ArbreMap<string, Epicerie>::Iterateur iterE = e.debut();
    PointST tempPoint;
    while(iterE){
         tempPoint= e[iterE.cle()].getLocation();

        if(valideEpicerie(e[iterE.cle()]) && (round(position.distance(tempPoint)*2) <= maxDistance)){          
            pool.inserer(iterE.valeur());
        }
        ++iterE;
    }
       ArbreAVL<Epicerie>::Iterateur i = pool.debut();
  
       
    return pool;
}



ArbreMap<int, ArbreAVL<Epicerie> > Commande::EnsembleDesParcours(ArbreAVL<Epicerie>& poolEpicerie) 
//Algo trouvé sur https://stackoverflow.com/questions/728972/finding-all-the-subsets-of-a-set

{
    
    ArbreMap<int, ArbreAVL<Epicerie> > ensembleDesParcours;
    ArbreAVL<Epicerie> vide;
    ensembleDesParcours[0] = vide;
    int compteur = 1;

    
  
    for(ArbreAVL<Epicerie>::Iterateur i = poolEpicerie.debut(); i; ++i){
        ArbreMap<int, ArbreAVL<Epicerie> > ensembleDesParcoursTemp = ensembleDesParcours;
   
        for(int j = 0; j < ensembleDesParcoursTemp.taille(); j++){
                ensembleDesParcoursTemp[j].inserer(*i);
        }
        
        for(int j = 0; j< ensembleDesParcoursTemp.taille(); j++){
                if(ensembleDesParcoursTemp[j].taille()<=maxEpiceries){
                    ensembleDesParcours[compteur] = ensembleDesParcoursTemp[j];
                    compteur++;
                }
        }
        
    }
    
    return ensembleDesParcours;
}

bool Commande::valideQuantiteItem(ArbreAVL<Epicerie>& sousEnsemble){
    
    int nbItemVoulu= nbItemsStillNeeded,quantiteDispo;
    ArbreAVL<Epicerie> parcouru;

    ArbreMap<string,int>::Iterateur itemDansListe =liste.debut();
    ArbreAVL<Epicerie>::Iterateur iterEpiceries=sousEnsemble.debut();
    
    
    while(itemDansListe){ 


        string itemName = itemDansListe.cle();
        int quantity = itemDansListe.valeur();
        while( iterEpiceries && quantity!=0){
            
            Epicerie epicerie = *iterEpiceries;    
            quantiteDispo=epicerie.quantityAvailable(itemName);
            
           
            if( quantiteDispo > 0){ 
    
                if(quantity > quantiteDispo){                    
                    quantity = quantity - quantiteDispo;
                }else{
                        quantity = 0 ;
                        nbItemVoulu--;
                }
                parcouru.inserer(*iterEpiceries);
                
            }
              
            iterEpiceries++;            
        }
       
         iterEpiceries = sousEnsemble.debut();
        ++itemDansListe;
    }

 return sousEnsemble.taille() == parcouru.taille() && nbItemVoulu==0;
}

int Commande::quantiteDisponible(Epicerie& epicerie,ArbreMap<string,int>::Iterateur itemDansListe){
    ArbreMap<string,ArbreMap<Date,int> > inventaire = epicerie.getInventaire();
    ArbreMap<Date,int> dateEtQuantite = inventaire[itemDansListe.cle()];
    int quantiteDisponible = 0;
    ArbreMap<Date,int>::Iterateur iter = dateEtQuantite.debut();
    
    while(iter){      
        quantiteDisponible += iter.valeur();
        ++iter;
    }
    return quantiteDisponible;   
}


bool Commande::valideEpicerie(Epicerie& epicerie)
{
    bool estVisite = false;
    ArbreMap<string,int>::Iterateur item = liste.debut();
    while(item && !estVisite){
        //si item dans liste existe dans inventaire
        if(item.valeur()>0){
            estVisite=epicerie.quantityAvailable(item.cle())>0;
        }
        ++item;
    }
    return estVisite;
}

istream& operator>>(istream& is, Commande& c){
    int nbItems=0;
    string nomproduit;
    is >> nomproduit;
    while(is && nomproduit!=";"){
	int quantite;
        is >> quantite;
        //compléter: 
        c.liste[nomproduit] = quantite;
        c.nbItemsStillNeeded= ++nbItems;
        is >> nomproduit;
    }
    return is;
}
