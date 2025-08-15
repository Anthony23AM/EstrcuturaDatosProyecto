import pymongo
import os
import sys

# Configura tu URI de MongoDB Atlas aquí (ajusta si es necesario)
db_uri = os.environ.get(
    "MONGO_URI",
    "mongodb+srv://ramirezmeza2003:MEzaJose963.@cluster0.5a3otrc.mongodb.net/ModuloCitas?retryWrites=true&w=majority"
)

try:
    client = pymongo.MongoClient(db_uri)
    db = client["ModuloCitas"]
    coleccion = db["citas"]
except Exception as e:
    print(f"Error de conexión a MongoDB: {e}")
    sys.exit(1)

# Recibe la ruta base como argumento
import sys
if len(sys.argv) < 2:
    print("Uso: python descargar_turnos_mongo.py <rutaBase>")
    sys.exit(1)

ruta_base = sys.argv[1]
if not ruta_base.endswith("/") and not ruta_base.endswith("\\"):
    ruta_base += "/"

# Crear la ruta base si no existe
import os
os.makedirs(ruta_base, exist_ok=True)

archivo_txt = ruta_base + "moduloCitas.txt"


docs = list(coleccion.find())
with open(archivo_txt, "w", encoding="utf-8") as f:
    for doc in docs:
        # Solo escribir si el documento tiene los campos mínimos requeridos
        idTurno = str(doc.get("idTurno", ""))
        nombre = doc.get("nombre", "")
        apellido = doc.get("apellido", "")
        cedula = doc.get("cedula", "")
        correo = doc.get("correo", "")
        telefono = doc.get("telefono", "")
        nombreEsp = doc.get("nombreEsp", "")
        apellidoEsp = doc.get("apellidoEsp", "")
        especialidad = doc.get("especialidad", "")
        fecha = doc.get("fecha", "")
        hora = doc.get("hora", "")
        hospital = doc.get("hospital", "")
        if not idTurno or not nombre or not apellido or not cedula or not nombreEsp or not apellidoEsp or not especialidad or not fecha or not hora or not hospital:
            continue  # Omitir documentos incompletos
        fechaNac = "01/01/2000"
        cedulaEsp = ""
        fechaAtencion = fecha
        horaAtencion = hora
        direccionHospital = hospital
        linea = f"{idTurno}|{nombre}|{apellido}|{cedula}|{correo}|{telefono}|{fechaNac}|{nombreEsp}|{apellidoEsp}|{especialidad}|{cedulaEsp}|{fechaAtencion}|{horaAtencion}|{direccionHospital}\n"
        f.write(linea)

print(f"Datos descargados de MongoDB y guardados en {archivo_txt}")
