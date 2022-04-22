import bpy
from bpy import data as D, context as C
import bmesh
import json

def create_polyhedron(vertices, faces, color, name="Polyhedron"):
    # Create mesh from above data.
    mesh_data = D.meshes.new(name)
    mesh_data.from_pydata(vertices, [], faces)

    # Returns True if any invalid geometry was removed.
    corrections = mesh_data.validate(
      verbose=True,
      clean_customdata=True)

    # Load BMesh with mesh data.
    bm = bmesh.new()
    bm.from_mesh(mesh_data)

    bm.to_mesh(mesh_data)
    bm.free()
    mesh_obj = D.objects.new(mesh_data.name, mesh_data)
    C.collection.objects.link(mesh_obj)
    
    
    mat = bpy.data.materials.new(name="") #set new material to variable
    h = color.lstrip("#")
    r, g, b = tuple(int(h[i:i+2], 16) for i in (0, 2, 4))
    mat.diffuse_color = (r/255, g/255, b/255, 1)
    mesh_obj.active_material = mat
    

def load_urban_block(path):
    with open(path) as f:
        data = json.load(f)

    for k, v in data.items():
        vertices = []
        faces = []
        num_base_vertices = len(v["base"])
        for x, y in v["base"]:
            vertices.append((x, y, 0))
        faces = [tuple([x for x in range(num_base_vertices)])]
        if v["height"] != 0:
            for x, y in v["base"]:
                vertices.append((x, y, v["height"]))
            faces.append(tuple([x for x in range(num_base_vertices, 2 * num_base_vertices)]))
            for i in range(num_base_vertices):
                j = (i + 1) % num_base_vertices
                faces.append((i, j, j + num_base_vertices, i + num_base_vertices))
        name = "Building " + k
        create_polyhedron(vertices, faces, v["color"], name)




bpy.ops.object.select_all()
bpy.ops.object.delete()

"""ENTER THE PATH TO THE FILE CONTAINING THE 3D URBAN BLOCK HERE"""
file_name = "/home/quentin/Documents/CUBE/Scripts/Blender/3Dblock.json"

load_urban_block(file_name)

