/*  INF3105 - Structures de données et algorithmes
    UQAM | Faculté des sciences | Département d'informatique
    Automne 2017 | TP2
    http://ericbeaudry.uqam.ca/INF3105/tp2/   

    AUTEUR(S):
     1) Charles-Étienne Couture 
     2) Kayla Louis
*/
#include <fstream>
#include <iostream>
#include <string>
#include <cassert>
#include <cmath>
#include <limits>
#include "date.h"
#include "epicerie.h"
#include "pointst.h"

using namespace std;

int tp2(istream& entree){
    Date maintenant;
    ArbreMap<string,Epicerie> epiceries; //nomEpicerie->Epicerie(Pointst,liste<nomItem, ArbreAvl<Date>>)  
    while(entree){
        string typecommande;
        entree >> typecommande;
        if(!entree) break; // détection fin ==> sortie
        
        if(typecommande=="RECOMMANDER"){
     
            PointST p;
            int nbMaxEpiceries=0;
            double maxdistance=0;
            Commande c;
            char deuxpoints=0;
            entree >> p >> nbMaxEpiceries >> maxdistance >> deuxpoints >> c;
            assert(deuxpoints==':');
            if(!(epiceries.vide())){
                c.initialiser(p,nbMaxEpiceries,maxdistance,epiceries,maintenant);
                c.recommander();
            }else{
                cout << "IMPOSSIBLE";
            }    
            
            
        }else if(typecommande=="RAMASSER"){
            Commande c;
            entree >> c;
            string nomepicerie;
            entree >> nomepicerie;
            c.initialiser(epiceries,maintenant);
            while(entree && nomepicerie!=";"){
                // À compléter
                if(epiceries.contient(nomepicerie)){
                    ArbreMap<string,Epicerie>::Iterateur i = epiceries.rechercher(nomepicerie);
                    Epicerie epi = i.valeur();
                    epiceries = c.ramasser(epi);    
                }    
                entree >> nomepicerie;
            
            }
            c.afficherRamasser();
            // À compléter
        }else if(typecommande=="APPROV"){
            string nomepicerie;
            char deuxpoints=0;
            entree >> nomepicerie >> deuxpoints;
            assert(deuxpoints==':');
            string nomproduit;
            entree >> nomproduit;
         
            while(entree && nomproduit!=";"){
                int quantite;
                Date dateexpiration;
                entree >> quantite >> dateexpiration;
                epiceries[nomepicerie].setInventaire(nomproduit,dateexpiration,quantite);
                // À compléter
                entree >> nomproduit;
               
            }
            // À compléter
            cout << "OK";
        }else if(typecommande=="PLACER"){
            PointST position;
            string nom;
            char pointvirgule=0;
            entree >> nom >> position >> pointvirgule;
            assert(pointvirgule==';');
            // À compléter
            Epicerie epicerie(position,nom);
            /*if(!(epiceries.vide())){
                for(ArbreMap<string,Epicerie>::Iterateur iter = epiceries.debut();iter;++iter){
                    Epicerie tempEpicerie = iter.valeur();
                    epicerie.calculerDistanceInterEpicerie(tempEpicerie); 
                }
                
            }*/
            epiceries[nom]=epicerie;
            cout << "OK";
        }else if(typecommande=="INVENTAIRE"){
            string nomepicerie;
            char pointvirgule=0;
            entree >> nomepicerie>> pointvirgule;
            assert(pointvirgule==';');
            // À compléter
            cout << epiceries[nomepicerie].inventaireToString();
        }else if(typecommande=="DATE"){
            char pointvirgule=0;
            entree >> maintenant >> pointvirgule;
            assert(pointvirgule==';');
            //compléter
            for(ArbreMap<string, Epicerie>::Iterateur i = epiceries.debut();i;++i){
                
                epiceries[i.cle()].setInventaire(maintenant);
            }
            cout << "OK";
        }else{
            cout << "Commande '" << typecommande << "' invalide!" << endl;
            return 2;
        }
        cout << endl;
        entree >> ws; // force la lecture d'un espace blanc.
    }
    return 0;
}
// syntaxe d'appel : ./tp2 [nomfichier.txt]
int main(int argc, const char** argv){
    // Gestion de l'entrée :
    //  - lecture depuis un fichier si un argument est spécifié;
    //  - sinon, lecture depuis std::cin.
  
    
    
    if(argc>1){
         std::ifstream entree_fichier(argv[1]);
         if(entree_fichier.fail()){
             std::cerr << "Erreur d'ouverture du fichier '" << argv[1] << "'" << std::endl;
             return 1;
         }
         return tp2(entree_fichier);
    }else
         return tp2(std::cin);        

    return 0;
}

