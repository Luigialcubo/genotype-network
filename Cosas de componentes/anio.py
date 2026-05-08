from pathlib import Path
import xml.etree.ElementTree as ET

BASE_DIR = Path(__file__).resolve().parent
GRAPHML_FILE = BASE_DIR / "BWetal2022_FullNetworkData.graphml"

print("Leyendo:", GRAPHML_FILE)

# Primero miramos las claves de atributos declaradas en el GraphML
keys = {}

for event, elem in ET.iterparse(GRAPHML_FILE, events=("start",)):
    tag = elem.tag.split("}")[-1]

    if tag == "key":
        key_id = elem.attrib.get("id")
        attr_name = elem.attrib.get("attr.name")
        attr_type = elem.attrib.get("attr.type")
        target = elem.attrib.get("for")

        keys[key_id] = {
            "name": attr_name,
            "type": attr_type,
            "for": target
        }

print("\nAtributos declarados:")
for k, v in keys.items():
    print(k, "->", v)

print("\nPrimeros nodos con datos:")

contador = 0

for event, elem in ET.iterparse(GRAPHML_FILE, events=("end",)):
    tag = elem.tag.split("}")[-1]

    if tag == "node":
        node_id = elem.attrib.get("id")
        print("\nNodo:", node_id)

        for child in elem:
            child_tag = child.tag.split("}")[-1]
            if child_tag == "data":
                key = child.attrib.get("key")
                value = child.text
                print(" ", key, keys.get(key), "=", value)

        contador += 1
        elem.clear()

        if contador == 5:
            break