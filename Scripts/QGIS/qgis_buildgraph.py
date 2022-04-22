#!/usr/bin/python
# If processing is missing add the location of the module in your PYTHONPATH
# Example: PYTHONPATH=$PYTHONPATH:/usr/share/qgis/python/plugins python main.py
import argparse
from pathlib import Path
import tempfile
import sys
import os 

import networkx as nx
import json
from networkx.readwrite.nx_shp import read_shp
from networkx.readwrite import json_graph

from qgis.core import (
    QgsProject,
    QgsApplication,
    QgsVectorLayer,
    QgsField,
    QgsFeatureRequest,
    edit,
)

from qgis.analysis import QgsNativeAlgorithms
from PyQt5.QtCore import QVariant


def load_layer(path, name):
    layer = QgsVectorLayer(path, name, "ogr")
    if not layer.isValid():
        raise Exception(
            "Layer {} failed to load (path: {})".format(path, name))
        print("Layer failed to load!")
    else:
        QgsProject.instance().addMapLayer(layer)
    return layer


def print_algorithms():
    for alg in QgsApplication.processingRegistry().algorithms():
        print(alg.id(), "->", alg.displayName())


def print_green(str):
    print("{}{}{}".format("\033[92m", str, "\033[0m"))


def clean_roads(roads_path, clean_roads_path):
    road_layer= QgsVectorLayer(roads_path)
    road_layer.selectAll()
    processing.run("native:saveselectedfeatures",
        {
            "INPUT": road_layer,
            "OUTPUT": clean_roads_path
        })

    clean_layer = QgsVectorLayer(clean_roads_path)
    provider = clean_layer.dataProvider()
    request = "\"type\" is  'construction' or \"type\" is 'corridor' or  \"type\" is  'cycleway' or  \"type\" is 'footway' or  \"type\" is 'living_street' or  \"type\" is 'motorway' or  \"type\" is 'motorway_link' or  \"type\" is 'path' or  \"type\" is 'pedestrian' or  \"type\" is 'platform' or  \"type\" is 'service' or  \"type\" is 'steps' or  \"type\" is 'track'"

    it= clean_layer.getFeatures(QgsFeatureRequest().setFilterExpression(request))
    provider.deleteFeatures([i.id() for i in it])


def add_color(nodes_path):
    nodesLayer = QgsVectorLayer(nodes_path)
    pv = nodesLayer.dataProvider()
    pv.addAttributes([QgsField("color", QVariant.String)])
    nodesLayer.updateFields()
    context = QgsExpressionContext()
    context.appendScopes(QgsExpressionContextUtils.globalProjectLayerScopes(nodesLayer))

    with edit(nodesLayer):
        for f in nodesLayer.getFeatures():
            context.setFeature(f)
            if f["osm_id"]:
                if f["type"] == "school":
                    f["color"] = "#00ffff"
                else:
                    f["color"] = "#f00000"
            else:
                f["color"] = "#acacac"
            nodesLayer.updateFeature(f)


def add_length(edges_path):
    edgesLayer = QgsVectorLayer(edges_path)
    pv = edgesLayer.dataProvider()
    pv.addAttributes([QgsField("Length", QVariant.Double)])
    edgesLayer.updateFields()

    expression = QgsExpression("$length")

    context = QgsExpressionContext()
    context.appendScopes(QgsExpressionContextUtils.globalProjectLayerScopes(edgesLayer))

    with edit(edgesLayer):
        for f in edgesLayer.getFeatures():
            context.setFeature(f)
            f["Length"] = expression.evaluate(context)
            edgesLayer.updateFeature(f)



def loadAndCategorizeNodesLayer(nodes_path, nodes_name):
    nodes_layer = QgsVectorLayer(nodes_path, nodes_name)
    if not nodes_layer.isValid():
        raise Exception(
            "Layer {} failed to load (path: {})".format(path, name))
        print("Layer failed to load!")
    else:
        QgsProject.instance().addMapLayer(nodes_layer)

        colors = ["#00ffff", "#acacac", "#f00000"]
        categories = []
        for color in colors:
            symbol = QgsSymbol.defaultSymbol(nodes_layer.geometryType())
            symbol.setColor(QColor(color))
            category = QgsRendererCategory(color, symbol, color)
            categories.append(category)

        renderer = QgsCategorizedSymbolRenderer("color", categories)
        nodes_layer.setRenderer(renderer)
        nodes_layer.triggerRepaint()


def qgs_script(shape_dir, save_dir, tmp_dir):
    project = QgsProject.instance()
    project.setFileName("{}.qgz".format(save_dir))

    buildings_path = path.os.join(shape_dir, "buildings.shp")
    roads_path = path.os.join(shape_dir, "roads.shp")

    print("RUN ~~ Fixing Geometries")
    fixed_buildings_name = "buildings_fixed.shp"
    fixed_buildings_path = os.path.join(save_dir, fixed_buildings_name)
    processing.run("native:fixgeometries",
                   {
                       "INPUT": buildings_path,
                       "OUTPUT": fixed_buildings_path,
                   })

    print("RUN ~~ Point on Surface")
    centroids_name = "centroids.shp"
    centroids_path = os.path.join(tmp_dir, centroids_name)
    processing.run("native:pointonsurface",
                   {
                       "INPUT": fixed_buildings_path,
                       "OUTPUT": centroids_path,
                   })

    print("RUN ~~ Clean Roads")
    clean_roads_name = "clean_roads.shp"
    clean_roads_path = os.path.join(tmp_dir, clean_roads_name)
    clean_roads(roads_path, clean_roads_path)

    print("RUN ~~ Convert Lines to Points")
    points_name = "points.shp"
    points_path = os.path.join(tmp_dir, points_name)
    processing.run("saga:convertlinestopoints", 
        {
            "LINES": clean_roads_path, 
            "ADD": True, 
            "DIST": 1, 
            "POINTS": points_path
        })

    print("RUN ~~ Distance to nearest hub line to hub")
    hubdistance_name = "hubdistance.shp"
    hubdistance_path = os.path.join(tmp_dir, hubdistance_name)
    processing.run("qgis:distancetonearesthublinetohub", 
        {
            "INPUT": centroids_path, 
            "HUBS": points_path, 
            "FIELD": "osm_id", 
            "UNIT": 0, 
            "OUTPUT": hubdistance_path
        })

    print("RUN ~~ Merge")
    merged_name = "merged.shp" 
    merged_path = os.path.join(tmp_dir, merged_name)
    processing.run("native:mergevectorlayers", 
        {
            "LAYERS": [clean_roads_path, hubdistance_path], 
            "CRS": None, 
            "OUTPUT": merged_path
        })

    print("RUN ~~ Split with lines")
    edges_name = "edges.shp"
    edges_path = os.path.join(save_dir, edges_name)
    processing.run("native:splitwithlines", 
        {
            "INPUT": merged_path, 
            "LINES": merged_path, 
            "OUTPUT": edges_path
        })

    print("RUN ~~ Build graph")
    vertex_name = "vertex.shp"
    vertex_path = os.path.join(tmp_dir, vertex_name)
    processing.run("Networks:build_graph", 
        {
            "RESEAU": edges_path, 
            "IDENT": 0, 
            "NOEUDS": vertex_path
        })

    print("RUN ~~ Join attributes by locacion")
    nodes_name = "nodes.shp"
    nodes_path = os.path.join(save_dir, nodes_name)
    processing.run("qgis:joinattributesbylocation", 
        { 
        "DISCARD_NONMATCHING" : False, 
        "INPUT" : vertex_path, 
        "JOIN" : fixed_buildings_path, 
        "JOIN_FIELDS" : [], 
        "METHOD" : 0, 
        "OUTPUT" : nodes_path, 
        "PREDICATE" : [5], 
        "PREFIX" : "" })    

    print("RUN ~~ Add Length")
    add_length(edges_path)

    print("RUN ~~ Add color")
    add_color(nodes_path)
    load_layer(fixed_buildings_path, "fixed buildings")
    load_layer(edges_path, "edges")
    loadAndCategorizeNodesLayer(nodes_path, "nodes")

    project.write()
        


def network_script(shape_dir, graph_name):
    print("Reading graph shapefiles into Networkx")
    edges = read_shp(os.path.join(shape_dir, "edges.shp"), False, False, False)
    nodes = read_shp(os.path.join(shape_dir, "nodes.shp"), False, False, False)
    G = nx.compose(nodes, edges)
    print("Number of nodes: ", G.number_of_nodes())
    print("Number of edges: ", G.number_of_edges())

    with open(graph_name, "w") as f:
        json.dump(json_graph.node_link_data(G), f, indent=4)
    print("Graph written in {}".format(graph_name))


"""Enter the absolute path to the directiory containing the shape files"""
shapedire = "/home/quentin/Documents/GIS/Test/shape"
"""Enter the absolute path to the directiory that will contain the new layers"""
save_dir = "/home/quentin/Documents/GIS/Test/save"
"""Enter the absolute path to the directiory that will contain the graph"""
graphname = "/home/quentin/Documents/GIS/Mons2/mons_graph.json"


Path(save_dir).mkdir(parents=True, exist_ok=True)
with tempfile.TemporaryDirectory() as tmp_dir:
    qgs_script(shapedire, save_dir, tmp_dir) #Create the graph

