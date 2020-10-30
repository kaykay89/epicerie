/*  INF3105 - Structures de données et algorithmes
    UQAM | Faculté des sciences | Département d'informatique
    Automne 2017 | Lab8 + TP2
    http://ericbeaudry.uqam.ca/INF3105/lab8/
    http://ericbeaudry.uqam.ca/INF3105/tp2/

    Tests unitaires.
*/
#include <iostream>
#include "arbremap.h"

void test1()
{
    std::cout << "ArbreMap / Test #1\n";
    ArbreMap<int, int> map1;
    map1[1] = 10;
    map1[2] = 20;
    std::cout << "map1[1]=" << map1[1];
    if(map1[1] == 10)
        std::cout << " : OK." << std::endl;
    else
        std::cout << " : Erreur." << std::endl;
      
    std::cout << "map[2]=" << map1[2];
    if(map1[2] == 20)
        std::cout << " : OK." << std::endl;
    else
        std::cout << " : Erreur." << std::endl;
    std::cout << "################################\n\n";

}

void test2()
{
    std::cout << "ArbreMap / Test #2\n";
  
    ArbreMap<std::string, int> map2;
    map2["Terre-Neuve"] = 405212;
    map2["Nouvelle-Ecosse"] = 55284;
    map2["Nouveau-Brunswick"] = 72908;
    map2["Ile-du-Prince-Edouard"] = 5660;
    map2["Quebec"] = 1542056;
    map2["Ontario"] = 1076395;
    map2["Manitoba"] = 647797;
    map2["Saskatchewan"] = 651036;
    map2["Alberta"] = 661848;
    map2["Colombie-Britanique"] = 944735;

    std::cout << "map2[\"Quebec\"]=" << map2["Quebec"];
    if(map2["Quebec"] == 1542056)
        std::cout << " : OK." << std::endl;
    else
        std::cout << " : Erreur." << std::endl;
  
    std::cout << "################################\n\n";
}
void test3()
{
    std::cout << "ArbreMap / Test #3\n";
  
    ArbreMap<int, std::string> map;
    map[0] = "zero";
    map[2] = "deux";
    map[4] = "quatres";
    map[6] = "six";
    map[1] = "un";
    map[3] = "trois";
    map[5] = "cinq";

    ArbreMap<int, std::string>::Iterateur iter = map.debut();
    int n=0;
    while(iter){
        std::cout << "map[" << iter.cle() << "]=" << "\"" << iter.valeur() << "\" : ";
        if(n==iter.cle())
            std::cout << " OK.\n";
        else
            std::cout << " Erreur.\n";
        ++iter; ++n;
    }
    if(n<7)
      std::cout << "Erreur : n<7\n";
  
    std::cout << "################################\n\n";
}

void test4()
{
    std::cout << "ArbreMap / Test #4\n";
  
    ArbreMap<int, std::string> map;
    map[0] = "zero";
    map[2] = "deux";
    map[4] = "quatres";
    map[6] = "six";
    map[1] = "un";
    map[3] = "trois";
    map[5] = "cinq";

    ArbreMap<int, std::string>::Iterateur iter = map.rechercher(4);
    int n=4;
    while(iter){
        std::cout << "map[" << iter.cle() << "]=" << "\"" << iter.valeur() << "\" : ";
        if(n==iter.cle())
            std::cout << " OK.\n";
        else
            std::cout << " Erreur.\n";
        ++iter; ++n;
    }
    if(n<7)
      std::cout << "Erreur : n<7\n";
  
    std::cout << "################################\n\n";
}
int main(int argc, const char** argv)
{
    test1();
    test2();
    test3();
    test4();
    return 0;
}
