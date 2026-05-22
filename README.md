# CavitY-Taskforce

Pour compiler : make
Pour lancer le jeu : make run
Pour supprimer les .o et l'exécutable : -make clean
                                        -make mrproper (supprime aussi sauvegarde.dat et scores.txt)

Structure du projet : 

utils.h/c : Fonctions utilitaires et affichage 
partie.h/c : Init, chargement fichiers, patients, sauvegarde, scores 
actions.h/c : Actions du dentiste (prendre, déposer, soigner…) 
jeu.h/c : Boucle principale et menu 
main.c : point d'entrée 
pathologies.txt : Données des pathologies (instruments  prix) 
couts.txt : Coût de chaque instrument (numéro de l'instrument prix)

CavitY-Taskforce est un jeu vidéo qui consiste à guérir des patients ayant des pathologies.
Vous incarnez le dentiste.Vos patients ont des pathologies qui necéssitent un ou deux outils maximum pour être traitées. Vous devez mettre des gants puis déposer sur le plateau du patient les outils nécessaires. Ensuite, vous le soignerez et vous viderez son plateau dans la poubelle biologique avant de le redéposer. Assurez vous également d'avoir jeté vos gants dans la poubelle biologique et d'en porter des propres avant de soigner votre prochain patient. Vous ne pouvez pas jeter vos gants si vous avez un plateau dans les mains. La jauge de patience des patients est représentée à côté des pathologies des patients comme ceci : [########..] 21. 

Commandes : 

Pour vous déplacer :

A gauche : Appuyer sur 1 puis appuyer sur entrée.

A droite : Appuyer sur 3 puis appuyer sur entrée.

En haut : Appuyer sur 5 puis appuyer sur entrée.

En bas : Appuyer sur 2 puis appuyer sur entrée.

Vous pouvez saisir plusieurs commandes à la suite puis entrer mais le nombre de tours joués sera le même qu'en saisissant les commandes une à une.

Pour faire une action, se positionner à côté de la case concernée par l'action puis appuyer sur e :

-prendre des gants  (case G)
-jeter quelque chose (cases R ou B)
-soigner (case T)
-prendre un instrument (case I puis le numéro de l'instrument)


Pour sauvegarder :

Appuyer sur s puis appuyer sur entrée.

Pour quitter :

Appuyer sur x puis appuyer sur entrée, puis o ou n pour sauvegarder ou non. 



