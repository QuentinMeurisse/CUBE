## Scripts
Ce dossier contient plusieurs scripts utilisable par QGIS. Il est nécessaire d'installer le plugin Network et la librairie SAGA.

- Le script `qgis_buildgraph.py` permet de générer une graphe à partir des fichiers `buildings.shp` et `roads.shp` obtenus depuis OpenStreetMap par exemple. L'utilisateur doit spécifier le chemin d'accès à ces fichiers. Il doit également indiquer où sauvegarder les nouveau shapefiles créés par ce script.

- Le script `qgis_timed_graph.py` a la même fonction que `qgis_buildgraph.py`. En plus, il ajouter un temps de parcourt des différentes route à différentes heures.

- Le script `qgis_export_graph.py` contient des méthodes permettant d'exporter les graphes générés par les précédents scripts.
