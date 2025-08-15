
import sys
import pymongo
import os


# Parámetros esperados: todos los datos del turno como argumentos
# Uso: python insertar_turno_mongo.py <idTurno> <nombre> <apellido> <cedula> <correo> <telefono> <nombreEsp> <apellidoEsp> <especialidad> <fecha> <hora> <hospital>
                                                                                         
def descargar_turnos():
    """
    Descarga todos los turnos de MongoDB y los guarda en moduloCitas.txt en formato compatible.
    """
    db_uri = os.environ.get(
        "MONGO_URI",
        "mongodb+srv://ramirezmeza2003:MEzaJose963.@cluster0.5a3otrc.mongodb.net/turnos_medicos?retryWrites=true&w=majority"
    )
    try:
        client = pymongo.MongoClient(db_uri)
        db = client["ModuloCitas"]
        coleccion = db["citas"]
    except Exception as e:
        print(f"Error de conexión a MongoDB: {e}")
        print(f"URI usado: {db_uri}")
        sys.exit(1)
    turnos = list(coleccion.find())
    ruta_base = sys.argv[2] if len(sys.argv) > 2 else ""
    if ruta_base and not ruta_base.endswith("/") and not ruta_base.endswith("\\"):
        ruta_base += "/"
    archivo_txt = ruta_base + "moduloCitas.txt"
    with open(archivo_txt, "w", encoding="utf-8") as f:
        for t in turnos:
            linea = f"{t.get('idTurno','')}|{t.get('nombre','')}|{t.get('apellido','')}|{t.get('cedula','')}|{t.get('nombreEsp','')}|{t.get('apellidoEsp','')}|{t.get('especialidad','')}|{t.get('fecha','')}|{t.get('hora','')}|{t.get('hospital','')}\n"
            f.write(linea)
    print(f"{len(turnos)} turnos descargados de MongoDB y guardados en {archivo_txt}")

if __name__ == "__main__":
    if (len(sys.argv) == 2 and sys.argv[1] == "descargar") or (len(sys.argv) == 3 and sys.argv[1] == "descargar"):
        descargar_turnos()
        sys.exit(0)
    if len(sys.argv) < 13:
        print("Uso: python insertar_turno_mongo.py <idTurno> <nombre> <apellido> <cedula> <correo> <telefono> <nombreEsp> <apellidoEsp> <especialidad> <fecha> <hora> <hospital>")
        print("O: python insertar_turno_mongo.py descargar")
        sys.exit(1)
    idTurno = sys.argv[1]
    nombre = sys.argv[2]
    apellido = sys.argv[3]
    cedula = sys.argv[4]
    correo = sys.argv[5]
    telefono = sys.argv[6]
    nombreEsp = sys.argv[7]
    apellidoEsp = sys.argv[8]
    especialidad = sys.argv[9]
    fecha = sys.argv[10]  # formato: DD/MM/AAAA
    hora = sys.argv[11]   # formato: HH:MM
    hospital = sys.argv[12]
    db_uri = os.environ.get(
        "MONGO_URI",
        "mongodb+srv://ramirezmeza2003:MEzaJose963.@cluster0.5a3otrc.mongodb.net/turnos_medicos?retryWrites=true&w=majority"
    )
    try:
        client = pymongo.MongoClient(db_uri)
        db = client["ModuloCitas"]
        coleccion = db["citas"]
    except Exception as e:
        print(f"Error de conexión a MongoDB: {e}")
        print(f"URI usado: {db_uri}")
        sys.exit(1)
    documento = {
        "idTurno": idTurno,
        "nombre": nombre,
        "apellido": apellido,
        "cedula": cedula,
        "correo": correo,
        "telefono": telefono,
        "nombreEsp": nombreEsp,
        "apellidoEsp": apellidoEsp,
        "especialidad": especialidad,
        "fecha": fecha,
        "hora": hora,
        "hospital": hospital
    }
    try:
        coleccion.insert_one(documento)
        print("Turno insertado en MongoDB")
    except Exception as e:
        print(f"Error al insertar en MongoDB: {e}")
