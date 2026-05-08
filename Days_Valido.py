from pathlib import Path
import xml.etree.ElementTree as ET

BASE_DIR = Path(__file__).resolve().parent
GRAPHML_FILE = BASE_DIR / "BWetal2022_FullNetworkData.graphml"

days = []

for event, elem in ET.iterparse(GRAPHML_FILE, events=("end",)):
    tag = elem.tag.split("}")[-1]

    if tag == "node":
        for child in elem:
            child_tag = child.tag.split("}")[-1]

            if child_tag == "data":
                key = child.attrib.get("key")
                value = child.text

                # En tu salida, la key temporal parece llamarse directamente "days"
                if key == "days" and value is not None:
                    days.append(int(value))

        elem.clear()

validos = [d for d in days if d >= 0]
invalidos = [d for d in days if d < 0]

print("Total nodos con atributo days:", len(days))
print("Days válidos:", len(validos))
print("Days inválidos (-1):", len(invalidos))

if validos:
    print("Mínimo day válido:", min(validos))
    print("Máximo day válido:", max(validos))
    print("Primeros 20 days válidos:", sorted(validos)[:20])
else:
    print("No hay days válidos.")