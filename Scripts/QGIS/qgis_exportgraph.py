import qgis
from qgis.core import *
import networkx as nx 
import pydot
from networkx.drawing.nx_pydot import write_dot
import json
from networkx.readwrite.nx_shp import read_shp
from networkx.readwrite import json_graph
import os 



def export_weigthed_graph(graph_path, nodes_path, edges_path):
  nodesLayer = QgsVectorLayer(nodes_path)
  edgesLayer = QgsVectorLayer(edges_path)

  print("Creating Graph")
  table = []
  for f in edgesLayer.getFeatures():
    if f["i"] != f["j"]:
      i = int(f["i"])
      j = int(f["j"])
      table += [(i, j, f["Length"])]
      if f["oneway"] != "1":
        table += [(j, i, f["Length"])]

  
  g = nx.DiGraph()
  g.add_weighted_edges_from(table)

  for f in nodesLayer.getFeatures():
    num = int(f["num"])
    g.nodes[num]["color"] = f["color"]

  print("Number of nodes: ", g.number_of_nodes())
  print("Number of edges: ", g.number_of_edges())

  with open(graph_name, "w") as f:
      json.dump(json_graph.node_link_data(g), f, indent=4)
  print("Graph written in {}".format(graph_name))

def export_dynamic_graph(graph_path, nodes_path, edges_path, first_time, last_time):
  nodesLayer = QgsVectorLayer(nodes_path)
  edgesLayer = QgsVectorLayer(edges_path)

  print("Creating Graph")
  g = nx.DiGraph(time_interval=3600)
  for f in edgesLayer.getFeatures():
    i = int(f["i"])
    j = int(f["j"])
    length = float(f["Length"]) if f["Length"] != qgis.core.NULL else 1.
    times = []
    for k in range(first_time, last_time):
      avg_time = "BS_AvgTt" if k == 0 else "CS{}_AvgTt".format(k+1)
      t = float(f[avg_time]) if f[avg_time] != qgis.core.NULL else length/8. #30 km/h correspond about 8 m/s
      times.append(t)
    g.add_edge(i, j, length=length, times=times)
    if (f["id"] == qgis.core.NULL):
      g.add_edge(j, i, length=length, times=times)

  for f in nodesLayer.getFeatures():
    num = int(f["num"])
    g.nodes[num]["color"] = f["color"]

  print("Number of nodes: ", g.number_of_nodes())
  print("Number of edges: ", g.number_of_edges())

  with open(graph_name, "w") as f:
      json.dump(json_graph.node_link_data(g), f, indent=4)
  print("Graph written in {}".format(graph_name))

if __name__ == '__main__':
  
  QgsApplication.setPrefixPath('/usr', True)
  qgs = QgsApplication([], False)
  qgs.initQgis()


  #save_dir = "/home/quentin/Documents/GIS/Mons2"
  save_dir = "/home/quentin/Documents/GIS/mons-time without holidays"
  edges_path = os.path.join(save_dir, "save/edges_with_new_school.shp")
  nodes_path = os.path.join(save_dir, "save/nodes_with_new_school.shp")
  graph_name = os.path.join(save_dir, "mons_7h_10h_moved.json")

  export_dynamic_graph(graph_name, nodes_path, edges_path, 7, 10)
