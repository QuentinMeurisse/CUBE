# LSCUBE

## Prérequis 
- Une version récente de [Java](https://java.com/en/download/)

## Utilisation
- Exécuter le fichier LSCUBE.jar.


# CUBE

## Prérequis
- Un compilateur C++ récent (gcc 9 et g++ 9 sous Linux)
- [Boost](https://www.boost.org/)
- [OpenMesh](https://www.graphics.rwth-aachen.de/software/openmesh/download/)
- [Qt(5](https://wiki.qt.io/Install_Qt_5_on_Ubuntu)


## Installation d'OpenMesh
### (Sous Linux)
- Télécharger une version d'OpenMesh [ici](https://www.graphics.rwth-aachen.de/software/openmesh/download/).
- Extraire le dossier contenu dans l'archive téléchargée.
- Dans le dossier extrait, entrer les commandes suivantes : 
```bash
mkdir build && cd build
cmake ..
sudo make install
```

## Installation de CUBE
- Télécharger le ZIP ou entrer la commande suivante : 
 ```bash
git clone https://github.com/QuentinMeurisse/CUBE
```
- Cette commande installe également les librairies [fmt](https://github.com/fmtlib/fmt) et [nlohmann_json](https://github.com/ArthurSonzogni/nlohmann_json_cmake_fetchcontent) 

## Compilation 
### (Sous linux) 

```bash
cd CUBE/CUBE
mkdir build && cd build
cmake ..
make
```
## Utilisation
- Exécuter le fichier CUBE générer dans le dossier build.

## Exemples
- Le dossier Exemples contient plusieurs fichiers pouvant être utilisé dans CUBE.
- Ces fichiers sont regroupés en fonction du module dans lequel ils peuvent être utilisés. 