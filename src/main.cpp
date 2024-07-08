// Charlot Noémie
using namespace std;
#include <iostream>
#include <string>
// -------------------------------
const char *textual(bool b) { return b ? "true" : "false"; }

using t_cls_rarete = struct {
  string classe_rarete;
  double facteur;
};
using t_artefact = struct {
  string nom;
  int pouvoir;
  int poids;
  int prix_u;
  int nb_exemplaire;
  t_cls_rarete *rarete;
};
using t_sac = struct {
  t_artefact **contenu; // Une liste d'emplacement ; avec un pointeur vers un
                        // artefact dedans
  int capacite;
  int occupation;
};
// -------------------------------------
// Fonctions et procédure a propos du sac d'artefact

// role : crée un nouveau sac avec une capacité de 4.
// entrée : Aucune
// sortie : sac : t_sac
t_sac initSac() {
  t_sac sac;
  sac.contenu = new t_artefact *[4];
  sac.capacite = 4;
  sac.occupation = 0;
  return sac;
}

//------------------------------------------------------------------
// role : double la capacité du tableau contenant les classe si l'occupation
// atteint 2/3 de sa capacité
//  parametre:(m)t_cls_rarete **tab_classe
void augmenterTabClasse(t_cls_rarete **tab_classe) {
  t_cls_rarete **tmp;
  int ind = 0;
  int null;
  while (tab_classe[ind] != nullptr) {
    ind++;
  }
  null = ind;
  while (tab_classe[null] == nullptr) {
    null++;
  }
  if (ind > 2 * ((ind + null) / 3)) {
    tmp = new t_cls_rarete *[(ind + null) * 2];
    for (int i = 0; i < ind; i++) {
      tmp[i] = tab_classe[i];
    }
    delete[] tab_classe;
    tab_classe = tmp;
  }
}

//------------------------------------------------------------------
// role : double la capacité du sac si l'occupation  atteint 2/3 de sa capacité
//  parametre:(m)t_sac *sac
void augmenterCapaSac(t_sac *sac) {
  t_sac tmp;
  if (sac->occupation > 2 * (sac->capacite / 3)) {
    tmp.contenu = new t_artefact *[(sac->capacite) * 2];
    tmp.capacite = sac->capacite * 2;
    for (int i = 0; i < sac->occupation; i++) {
      tmp.contenu[i] = sac->contenu[i];
    }
    delete[] sac->contenu;
    sac->contenu = tmp.contenu;
    sac->capacite = tmp.capacite;
  }
}

//------------------------------------------------------------------
// role : divise  la capacité du sac par 2 si l'occupation  est inferieur a 1/6
// de sa capacité
//  parametre:(m)t_sac *sac
void reduireCapaSac(t_sac *sac) {
  t_sac tmp;
  if ((sac->occupation <= (sac->capacite) / 6) && (sac->capacite > 4)) {
    if ((sac->capacite) / 2 < 4) {
      tmp.contenu = new t_artefact *[4];
      tmp.capacite = 4;
    } else {
      tmp.contenu = new t_artefact *[(sac->capacite) / 2];
      tmp.capacite = sac->capacite / 2;
    }
    for (int i = 0; i < sac->occupation; i++) {
      tmp.contenu[i] = sac->contenu[i];
    }
    delete[] sac->contenu;
    sac->contenu = tmp.contenu;
    sac->capacite = tmp.capacite;
  }
}

//-----------------------------------------------------------------------------------
// role:retourne vrai et la position si un exemplaire de l'artefacte est déja
// présent dans le sac  ,sinon retourne faux parametre:(e)t_sac sac
// ,(e)t_artefact art,(s)bool est_present , (s)int position
void present(t_sac *sac, t_artefact art, bool &est_present, int &position) {
  est_present = false;
  while (!(est_present) && position < sac->occupation) {
    if ((art.nom == sac->contenu[position]->nom) &&
        (sac->contenu[position] != nullptr)) {
      est_present = true;
    } else {
      position++;
    }
  }
}
int insert_position(t_sac sac, t_artefact art) {
  // variables
  int i;
  // debut
  i = 0;
  while ((i < sac.occupation) && (art.pouvoir < sac.contenu[i]->pouvoir)) {
    i++;
  }
  return i;
}

//------------------------------------------------------------------
// role :retourne la position d'un artefacte dans le sac.
//  parametre:(e)t_sac sac , (e)string art_name ,(s) int
int rechercheNbArtefact(t_sac sac, string art_name) {
  for (int i = 0; i < sac.occupation; i++) {
    if (sac.contenu[i]->nom == art_name) {
      return sac.contenu[i]->nb_exemplaire;
    }
  }
  return 0;
}

//------------------------------------------------------------------
// role :ajoute un nouvel artefacte dans le sac.
//  parametre:(m)t_sac sac , (e)t_artefact art
void ajouterArtefact(t_sac *sac, t_artefact *art) {
  bool est_present;
  int position = 0;
  if (sac->occupation == 0) {
    sac->contenu[0] = art;
    sac->occupation++;
  } else {
    present(sac, *art, est_present, position);
    if (est_present) {
      sac->contenu[position]->nb_exemplaire =
          sac->contenu[position]->nb_exemplaire + art->nb_exemplaire;
    } else {
      position = insert_position(*sac, *art);
      for (int i = position; i < sac->occupation; i++) {
        sac->contenu[sac->occupation + position - i] =
            sac->contenu[sac->occupation - 1 + position - i];
      }
      sac->contenu[position] = art;
      sac->occupation += 1;
    }
    augmenterCapaSac(sac);
  }
}

//------------------------------------------------------------------
// role :supprime un  artefacte deja present dans le sac.
//  parametre:(m)t_sac sac , (e)t_artefact art
bool supprimerArtefact(t_sac *sac, string art_name) {
  bool supprimer = false;
  int i = 0;
  while (i < sac->occupation && !(supprimer)) {
    if ((sac->contenu[i])->nom == art_name) {
      if (sac->contenu[i]->nb_exemplaire > 1) {
        sac->contenu[i]->nb_exemplaire--;
      } else {
        sac->contenu[i] = nullptr;
        while (sac->contenu[i + 1] != nullptr) {
          sac->contenu[i] = sac->contenu[i + 1];
          i++;
        }
        sac->occupation--;
        reduireCapaSac(sac);
      }
      supprimer = true;
    } else {
      i++;
    }
  }
  return supprimer;
}

//------------------------------------------------------------------
// role :calcul le prix total de tous les artefacts du sac
//  parametre:(e)t_sac sac ;(s)int val_totale
int valeurTotaleVente(t_sac sac) {
  int val_totale = 0;
  for (int i = 0; i < sac.occupation; i++) {
    val_totale =
        (int)(((sac.contenu[i]->prix_u) * ((sac.contenu[i]->rarete)->facteur)) *
              sac.contenu[i]->nb_exemplaire) +
        val_totale;
  }
  return val_totale;
}

//------------------------------------------------------------------
// role :retourne l'artefact ayant la plus grande puissance magique.
//  parametre:(e)t_sac sac ;(s) t_artefact *max_art
t_artefact maxForceMagique(t_sac sac) {
  t_artefact *max_art;
  max_art = sac.contenu[0];
  for (int i = 0; i < sac.occupation; i++) {
    if (sac.contenu[i]->pouvoir > max_art->pouvoir) {
      max_art = sac.contenu[i];
    }
  }
  return *(max_art);
}

//------------------------------------------------------------------
// role :retourne  un tableau d'artefact ayant la plus grande puissance magique.
//  parametre:(e)t_sac sac ;(s) t_artefact **max_art
t_artefact **egaForceMagique(t_sac sac) {
  t_artefact max;
  t_artefact **tab_max;
  int cmpt = 0;
  tab_max = new t_artefact *[sac.occupation];
  max = maxForceMagique(sac);
  tab_max[0] = &max;
  for (int i = 0; i < sac.occupation; i++) {
    if (sac.contenu[i] != nullptr) {
      if (sac.contenu[i]->pouvoir == max.pouvoir) {
        tab_max[cmpt] = sac.contenu[i];
        cmpt++;
      }
    }
  }
  return tab_max;
}

//------------------------------------------------------------------
// role : affiche les informations d'un artefact.
//  parametre:(e)t_artefact art
void afficherArtefact(t_artefact art) {
  if (art.nb_exemplaire > 1) {
    cout << art.nom << ", " << art.pouvoir << " magie, " << art.poids
         << " kg, type <<" << (art.rarete)->classe_rarete << ">>, "
         << art.prix_u << " rupee par pièce, " << art.nb_exemplaire
         << " pièces ;" << endl;
  } else {
    cout << art.nom << ", " << art.pouvoir << " magie, " << art.poids
         << " kg, type <<" << (art.rarete)->classe_rarete << ">>, "
         << art.prix_u << " rupee par pièce, " << art.nb_exemplaire
         << " pièce ;" << endl;
  }
}
//------------------------------------------------------------------
// role : affiche les informations d'un sac.
//  parametre:(e)t_sac sac
void afficherSac(t_sac sac) {
  for (int i = 0; i < sac.occupation; i++) {
    cout << "Emplacement " << i + 1 << ": ";
    afficherArtefact(*sac.contenu[i]);
  }
  for (int i = sac.occupation; i < sac.capacite; i++) {
    cout << "Emplacement " << i + 1 << ": vide " << endl;
  }
  cout << endl;
}

//------------------------------------------------------------------
// role : affiche les informations d'un tableau d'artefact.
//  parametre:(e)t_artefact **l_max,(e)t_sac sac
void afficherTab(t_artefact **l_max, t_sac sac) {
  cout << "Artefact a la magie equivalante à " << l_max[0]->pouvoir << " : "
       << endl;
  for (int i = 0; i < sac.occupation; i++) {
    if (l_max[i] != NULL) {
      cout << "- ";
      afficherArtefact(*(l_max[i]));
    }
  }
  cout << endl;
}

//------------------------------------------------------------------
// role : affiche les instructions permettant de supprimer un artefacte et
// appelle la fonction supprimerArtefact.
//  parametre:(m)t_sac sac
void affichageSupprimer(t_sac *sac) {
  string nom_supp;
  int pos;
  for (int i = 0; i < sac->occupation; i++) {
    cout << (i + 1) << ") " << sac->contenu[i]->nom << ";" << endl;
  }
  cout << "Entrez le numero de l'artefact que vous souhaitez supprimer : ";
  cin >> pos;
  if (pos <= 0 || pos > sac->occupation) {
    cout << "La position est invalide, veuillez recommencer" << endl;
    affichageSupprimer(sac);
  }
  nom_supp = sac->contenu[pos - 1]->nom;
  if (supprimerArtefact(sac, nom_supp)) {
    cout << "L'artefact a bien été supprimé" << endl;
  } else {
    cout << "L'artefact n'existe pas" << endl << endl;
  }
}

//------------------------------------------------------------------
// role : affiche le nombre d'exemplaire d'un artefact dans le sac.
//  parametre:(m)t_sac sac
void afficherNbExemplaire(t_sac sac) {
  string nom_art;
  cout << "Donner le nom de l'artefact dont vous voulez la quantité : ";
  getline(cin, nom_art);
  if (rechercheNbArtefact(sac, nom_art) == 0) {
    cout << "Vous n'avez pas cet artefact" << endl;
  } else {
    if (rechercheNbArtefact(sac, nom_art) == 1) {
      cout << "Vous avez " << rechercheNbArtefact(sac, nom_art)
           << " exemplaire de " << nom_art << endl;
    } else {
      cout << "Vous avez " << rechercheNbArtefact(sac, nom_art)
           << " exemplaires de " << nom_art << endl;
    }
  }
}
//------------------------------------------------------------------
// role :verifie si la classe entrez par l'utisateur existe réelement.
//  parametre:(e)t_cls_rarete **tab_classe , (e)string nom_classe
int rechercheClasse(t_cls_rarete **tab_classe, string nom_classe) {
  int pos = 0;
  while (tab_classe[pos]->classe_rarete != nom_classe &&
         tab_classe[pos + 1] != nullptr) {
    pos++;
  }
  if (tab_classe[pos]->classe_rarete == nom_classe) {
    return pos;
  } else {
    cout << "Cette classe n'existe pas" << endl;
    return -1;
  }
}
//------------------------------------------------------------------
// role :verifie si l'artefact entrez par l'utisateur existe réelement.
//  parametre:(e)t_sac sac , (e)string nom_art
void rechercheArtefact(t_sac sac, string nom_art) {
  int pos = 0;
  while (sac.contenu[pos]->nom != nom_art && pos + 1 < sac.occupation) {
    pos++;
  }
  if (sac.contenu[pos]->nom == nom_art) {
    afficherArtefact(*(sac.contenu[pos]));
  } else {
    cout << "Vous n'avez pas cet artefact" << endl;
  }
}
//------------------------------------------------------------------
// role : creer une nouvelle classe.
//  parametre:(m)t_cls_rarete **tab_classe
void creerClasse(t_cls_rarete **tab_classe) {
  t_cls_rarete *classe;
  classe = new t_cls_rarete;
  int i = 0;
  cout << "Donner le nom de la classe : ";
  getline(cin, classe->classe_rarete);
  cout << "Donner son facteur : ";
  cin >> classe->facteur;
  if (classe->facteur > 10 || classe->facteur < 0) {
    cout << "Votre facteur est trop ou pas assez élevé, veillez ressayer "
         << endl;
    cout << "Donner son facteur : ";
    cin >> classe->facteur;
    cout << endl;
  }
  while (tab_classe[i] != nullptr) {
    i++;
  }
  tab_classe[i] = classe;
  augmenterTabClasse(tab_classe);
}

//------------------------------------------------------------------
// role : creer un nouvelle artefact et l'ajoute dans une classe.
//  parametre:(m)t_sac *sac,t_cls_rarete **t_classe
void creerArtefact(t_sac *sac, t_cls_rarete **t_classe) {
  t_artefact *art;
  string nom_classe;
  if (t_classe[0] == nullptr) {
    cout << "Vous n'avez pas encore creer de classe" << endl;
  } else {
    int pos;
    art = new t_artefact;
    cout << "Donner le nom de l'artefact : ";
    getline(cin, art->nom);
    cout << "Donner son pouvoir : ";
    cin >> art->pouvoir;
    if (art->pouvoir > 100 || art->pouvoir < 0) {
      cout << "Votre artefact est trop ou pas assez puissant, veuillez ressayer"
           << endl;
      cout << "Donner son pouvoir : ";
      cin >> art->pouvoir;
    }
    cout << "Donner son poids : ";
    cin >> art->poids;
    cout << "Donner son prix : ";
    cin >> art->prix_u;
    cout << "Donner la quantité obtenue de celui : ";
    cin >> art->nb_exemplaire;
    cin.ignore(256, '\n');
    cout << "Donner sa classe : ";
    getline(cin, nom_classe);
    pos = rechercheClasse(t_classe, nom_classe);
    while (pos == -1) {
      cout << "voici les classes présentes : " << endl;
      int i = 0;
      while (t_classe[i+1]!=nullptr){
        cout << " . " << t_classe[i]->classe_rarete << endl;
      }
      cout << "Donner sa classe : ";
      getline(cin, nom_classe);
      pos = rechercheClasse(t_classe, nom_classe);
    }
    art->rarete = t_classe[pos];
    ajouterArtefact(sac, art);
  }
}

void choixUti(t_sac *sac, t_cls_rarete **tab_classe) {
  string rep1, rep2, rep3;
  cin.ignore(256, '\n');
  cout << "Nous vous laissons le choix de vos actions, dans la limite des "
          "commandes acceptées : "
       << endl
       << "creer ; supprimer ; afficher ; denombrer ; "
          "plus puissants ; prix vente ; stop"
       << endl
       << endl;
  cout << "Que voulez vous faire ? ";
  getline(cin, rep1);
  cout << endl;
  if (rep1 == "creer") {
    cout << "classe ou artefact ? ";
    getline(cin, rep2);
    if (rep2 == "classe") {
      creerClasse(tab_classe);
      choixUti(sac, tab_classe);
    } else if (rep2 == "artefact") {
      creerArtefact(sac, tab_classe);
      cout << endl;
      choixUti(sac, tab_classe);
    } else {
      cout << "Votre choix ne correspond pas à ceux proposés" << endl;
      choixUti(sac, tab_classe);
    }
  } else if (rep1 == "supprimer") {
    affichageSupprimer(sac);
    cout << endl;
    choixUti(sac, tab_classe);
  } else if (rep1 == "afficher") {
    cout << "sac ou artefact ? ";
    getline(cin, rep2);
    if (rep2 == "sac") {
      afficherSac(*sac);
      cout << endl;
      choixUti(sac, tab_classe);
    } else if (rep2 == "artefact") {
      if (sac->occupation == 0) {
        cout << "Vous n'avez pas encore d'artefact" << endl;
        choixUti(sac, tab_classe);
      } else {
        cout << "Donnez le nom de l'artefact : ";
        getline(cin, rep3);
        rechercheArtefact(*sac, rep3);
        choixUti(sac, tab_classe);
      }
    } else {
      cout << "Votre choix ne correspond pas à ceux proposés" << endl;
      choixUti(sac, tab_classe);
    }
  } else if (rep1 == "denombrer") {
    afficherNbExemplaire(*sac);
    cout << endl;
    choixUti(sac, tab_classe);
  } else if (rep1 == "plus puissants") {
    if (sac->contenu[0] == nullptr) {
      cout << "Vous n'avez pas encore d'artefact" << endl;
      choixUti(sac, tab_classe);
    } else {
      t_artefact **l_max;
      l_max = egaForceMagique(*sac);
      afficherTab(l_max, *sac);
      choixUti(sac, tab_classe);
    }
  } else if (rep1 == "prix vente") {
    cout << "La valeur totale de vente des artefacts est de "
         << valeurTotaleVente(*sac) << " rupee" << endl
         << endl;
    choixUti(sac, tab_classe);
  } else if (rep1 == "stop") {
  } else {
    cout << "Votre choix ne correspond pas à ceux proposés";
    cout << endl;
    choixUti(sac, tab_classe);
  }
}

int main() {
  cout << "Bienvenu dans votre inventaire, Link !" << endl;
  t_sac sac;
  string aff;
  int i;
  t_cls_rarete **tab_classe;
  tab_classe = new t_cls_rarete *[4];
  sac = initSac();
  choixUti(&sac, tab_classe);
  cout << "En esperant que cela vous a distrait" << endl;
  // desallocation des tableaux
  for (i = 0; i < sac.occupation; i++) {
    delete[] sac.contenu[i];
  }
  delete[] sac.contenu;
  i = 0;
  delete[] tab_classe[i];
  while (tab_classe[i] != nullptr && tab_classe[i + 1] != nullptr) {
    delete[] tab_classe[i];
    i++;
  }
  delete[] tab_classe[i];
  delete[] tab_classe;
}