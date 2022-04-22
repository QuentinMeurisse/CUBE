import argparse
from pathlib import Path
import tempfile
import sys

import networkx as nx
import json
from networkx.readwrite.nx_shp import read_shp
from networkx.readwrite import json_graph

from qgis.core import *

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

def add_color(nodes_path, school_type):
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
                if f["type"] == "school" and f["SchoolType"] == school_type:
                    f["color"] = "#00ffff"
                elif (f["type"] == "residential" or f["type"] == "apartments" or f["type"] == qgis.core.NULL) and f["nb"] == 1:
                    f["color"] = "#f00000"
                else:
                    f["color"] = "#acacac"
            else:
                f["color"] = "#acacac"
            nodesLayer.updateFeature(f)

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

def clean_edges(edges_path):
    edges_layer = QgsVectorLayer(edges_path)
    provider = edges_layer.dataProvider()
    request = "\"i\" is NULL"

    it= edges_layer.getFeatures(QgsFeatureRequest().setFilterExpression(request))
    provider.deleteFeatures([i.id() for i in it])

def clean_buildings(buildings_path, cleaned_buildings_path):
    buildings_layer = QgsVectorLayer(buildings_path)
    buildings_layer.selectAll()
    processing.run("native:saveselectedfeatures",
        {
            "INPUT": buildings_layer,
            "OUTPUT": cleaned_buildings_path
        })

    cleaned_layer = QgsVectorLayer(cleaned_buildings_path)
    provider = cleaned_layer.dataProvider()
    request = "not(\"type\" = 'school' or \"type\" = 'residential' or \"type\" = 'apartments' or \"type\" is NULL)"
    it= cleaned_layer.getFeatures(QgsFeatureRequest().setFilterExpression(request))
    provider.deleteFeatures([i.id() for i in it])

def add_length(edges_path):
    edgesLayer = QgsVectorLayer(edges_path)
    pv = edgesLayer.dataProvider()
    pv.addAttributes([QgsField("Length", QVariant.Double, "double", 10, 2)])
    edgesLayer.updateFields()

    expression = QgsExpression("$length")

    context = QgsExpressionContext()
    context.appendScopes(QgsExpressionContextUtils.globalProjectLayerScopes(edgesLayer))

    with edit(edgesLayer):
        for f in edgesLayer.getFeatures():
            context.setFeature(f)
            f["Length"] = expression.evaluate(context)
            edgesLayer.updateFeature(f)

def qgs_script(shape_dir, save_dir, tmp_dir, first_time, last_time, school_type):
    project = QgsProject.instance()
    project.setFileName("{}.qgz".format(save_dir))

    network_path = shape_dir + "/network.shp"
    buildings_path = shape_dir + "/buildings.shp"


    cleaned_buildings_name = "cleaned_buildings.shp"
    cleaned_buildings_path = "{}/{}".format(tmp_dir, cleaned_buildings_name)

    print("Run ~~ Clean buildings")
    clean_buildings(buildings_path, cleaned_buildings_path)
    
    joined_path = save_dir + "/joined.shp" 

    print("RUN ~~ Load time data")
    for i in range(first_time, last_time):
        data_path = shape_dir + "/April 2019 without holidays_{}_00-{}_00.dbf".format(i, i+1)
        first_input_path = network_path if i == first_time else tmp_dir + "/tmp_join{}.shp".format(i-1)
        output_path = joined_path if i == (last_time - 1) else tmp_dir + "/tmp_join{}.shp".format(i)
        avg_time = "BS_AvgTt" if i == 0 else "CS{}_AvgTt".format(i+1)
        avg_speed = "BS_AvgSp" if i == 0 else "CS{}_AvgSp".format(i+1)
        data_id = "BS_Id" if i == 0 else "CS{}_Id".format(i+1)

        parameters = {
        'DISCARD_NONMATCHING' : False, 
        'FIELD' : 'Id', 
        'FIELDS_TO_COPY' : [avg_time,avg_speed], 
        'FIELD_2' : data_id, 
        'INPUT' : first_input_path, 
        'INPUT_2' : data_path,
        'METHOD' : 0, 
        'OUTPUT' : output_path, 
        'PREFIX' : '' }
        processing.run('qgis:joinattributestable', parameters)

   
    print("RUN ~~ Build first graph")
    first_vertex_name = "first_vertex.shp"
    first_vertex_path = "{}/{}".format(tmp_dir, first_vertex_name)
    processing.run("Networks:build_graph", 
        {
            "RESEAU": joined_path, 
            "IDENT": 0, 
            "NOEUDS": first_vertex_path
        })

    print("RUN ~~ Point on Surface")
    centroids_name = "centroids.shp"
    centroids_path = "{}/{}".format(tmp_dir, centroids_name)
    processing.run("native:pointonsurface",
                   {
                       "INPUT": cleaned_buildings_path,
                       "OUTPUT": centroids_path,
                   })

    print("RUN ~~ Distance to nearest hub line to hub")
    hubdistance_name = "hubdistance.shp"
    hubdistance_path = "{}/{}".format(tmp_dir, hubdistance_name)
    processing.run("qgis:distancetonearesthublinetohub", 
        {
            "INPUT": centroids_path, 
            "HUBS": first_vertex_path, 
            "FIELD": "num", 
            "UNIT": 0, 
            "OUTPUT": hubdistance_path
        })

    print("RUN ~~ Compute length on Hub Distance")
    add_length(hubdistance_path)

    print("RUN ~~ Merge")
    edges_name = "edges.shp" 
    edges_path = "{}/{}".format(save_dir, edges_name)
    processing.run("native:mergevectorlayers", 
        {
            "LAYERS": [joined_path, hubdistance_path], 
            "CRS": None, 
            "OUTPUT": edges_path
        })

    print("RUN ~~ Build graph")
    vertex_name = "vertex.shp"
    vertex_path = "{}/{}".format(tmp_dir, vertex_name)
    processing.run("Networks:build_graph", 
        {
            "RESEAU": edges_path, 
            "IDENT": 0, 
            "NOEUDS": vertex_path
        })

    print("RUN ~~ Join attributes by locacion")
    nodes_name = "nodes.shp"
    nodes_path = "{}/{}".format(save_dir, nodes_name)
    processing.run("qgis:joinattributesbylocation", 
        { 
        "DISCARD_NONMATCHING" : False, 
        "INPUT" : vertex_path, 
        "JOIN" : cleaned_buildings_path, 
        "JOIN_FIELDS" : [], 
        "METHOD" : 0, 
        "OUTPUT" : nodes_path, 
        "PREDICATE" : [5], 
        "PREFIX" : "" })


    print("RUN ~~ Clean edges")
    clean_edges(edges_path)
    print("RUN ~~ Add color")
    add_color(nodes_path, school_type)
    load_layer(edges_path, "edges")
    loadAndCategorizeNodesLayer(nodes_path, "nodes")

    project.write()


shape_dir = "/home/quentin/Documents/GIS/mons-time without holidays/shape"
save_dir = "//home/quentin/Documents/GIS/mons-time without holidays/save"
Path(save_dir).mkdir(parents=True, exist_ok=True)
with tempfile.TemporaryDirectory() as tmp_dir:
    qgs_script(shape_dir, save_dir, tmp_dir, 7, 10, "Secondary")
print("done")