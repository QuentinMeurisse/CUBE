# LSCUBE

## Prérequis 
- Une version récente de Java https://java.com/en/download/

## Utilisation
- Exécuter le fichier LSCUBE.jar.


# CUBE

## Prérequis
- Un compilateur C++ récent.
- Boost
- Openmesh https://www.graphics.rwth-aachen.de/software/openmesh/
- Qt5 

## Installation
 Ce projet fait appel à des sous-modules. Ainsi un simple git clone ne suffit pas pour installer toutes les dépendances. La commande suivante permet un téléchargement de toutes les dépendances : 

```bash
git clone --recurse-submodules https://github.com/QuentinMeurisse/CUBE
```

## Compilation

```bash
cd CUBE/CUBE
mkdir build && cd build
cmake ..
cmake --build .
```
## Utilisation
- Exécuter le fichier CUBE générer dans le dossier build.
- Le dossiers Exemples contient plusieurs fichiers pouvant être utilisé dans CUBE. 