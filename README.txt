Projet Dataframe en C

Membres du projet :
Samy AIT ALLA
Sajin Sivasaranam

Lien github:
https://github.com/Samy2700/CDataframe-aitalla-sivasaranam-A

Description du Projet
Ce projet implémente une structure de données de type dataframe en langage C, similaire à celle utilisée dans des bibliothèques comme pandas en Python.
Le dataframe permet de stocker des données tabulaires avec différentes colonnes de types variés (int, float, double, char, string, structure personnalisée, etc.). Le projet permet de manipuler ces données via diverses opérations telles que l'ajout, la suppression, le tri, l'analyse et le chargement/sauvegarde depuis/vers des fichiers CSV.

Structure des Fichiers
main.c : Contient la fonction principale du programme et gère l'interaction avec l'utilisateur via un menu. Permet de choisir entre différentes options pour manipuler le dataframe.
cdataframe.h : Déclare les structures de données et les prototypes de fonctions pour gérer le dataframe.
cdataframe.c : Implémente les fonctions pour créer, manipuler et afficher le dataframe, ainsi que pour charger et sauvegarder les données depuis/vers des fichiers CSV.
column.h : Déclare les structures de données et les prototypes de fonctions pour gérer les colonnes du dataframe.
column.c : Implémente les fonctions pour créer, manipuler et afficher les colonnes du dataframe.
sort.h : Déclare les prototypes des fonctions de tri pour les colonnes.
sort.c : Implémente les fonctions de tri pour les colonnes, utilisant l'algorithme de tri rapide.

Fonctionnalités Principales
Création d'un dataframe : Permet de créer un dataframe vide ou pré-rempli avec des données prédéfinies.

Manipulation de données :
Ajout et suppression de lignes et de colonnes.
Modification de valeurs spécifiques dans le dataframe.
Renommage de colonnes.

Affichage :
Affichage complet du dataframe.
Affichage d'un nombre spécifié de lignes ou de colonnes.

Analyse :
Comptage du nombre de lignes et de colonnes.
Vérification de l'existence d'une valeur spécifique.
Comptage des cellules égales, supérieures ou inférieures à une valeur donnée.

Tri :
Tri des colonnes en ordre ascendant ou descendant.
Affichage des colonnes triées.
Vérification et mise à jour des index de tri.

Chargement et sauvegarde de fichiers CSV :
Permet de charger des données à partir d'un fichier CSV et de sauvegarder le dataframe dans un fichier CSV.

Utilisation
Compilation :
Clonez le projet depuis le dépôt Git ou téléchargez les fichiers.
Utiliser un environnement de développement comme CLion ou GCC pour compiler le projet.
S'assurer que tous les fichiers sources (.c) et les fichiers d'en-tête (.h) sont inclus dans le projet.

Exécution :
Suivre les instructions à l'écran pour interagir avec le programme via le menu.

Chargement de fichiers CSV :
Placer le fichier CSV dans le répertoire cmake-build-debug du projet.
Lors du choix de l'option pour charger un fichier CSV, entrer le nom du fichier avec son extension, par exemple "Exemple.csv".

Dépendances
Ce projet utilise uniquement les bibliothèques standard de C :
stdio.h
stdlib.h
string.h

Bugs et Solutions
Bug : Échec de l'ouverture du fichier CSV
Lors de la tentative de chargement d'un fichier CSV, le message "Échec de l'ouverture du fichier CSV" est affiché.

Solution : S'assurer que le chemin du fichier est correct et que le fichier existe dans le répertoire spécifié. Utiliser le chemin complet du fichier lors de l'entrée du nom du fichier.

Bug : Mauvais format de structure personnalisée
Les données de la structure personnalisée ne sont pas correctement chargées depuis le fichier CSV.

Solution : S'assurer que les données de la structure dans le fichier CSV respectent le format attendu. Adaptez le code de chargement pour correspondre à la structure réelle des données.

Le projet a été fait sur CLion et Git.