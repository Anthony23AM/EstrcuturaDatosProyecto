import sys
import pymongo
import os

# Uso: python eliminar_turno_mongo.py <idTurno>
if len(sys.argv) < 2:
    print("Uso: python eliminar_turno_mongo.py <idTurno>")
    sys.exit(1)

idTurno = sys.argv[1]

# URI de MongoDB Atlas
uri = os.environ.get(
    "MONGO_URI",
    "mongodb+srv://ramirezmeza2003:MEzaJose963.@cluster0.5a3otrc.mongodb.net/ModuloCitas?retryWrites=true&w=majority"
)

try:
    client = pymongo.MongoClient(uri)
    db = client["ModuloCitas"]
    coleccion = db["citas"]
    result = coleccion.delete_one({"idTurno": str(idTurno)})
    if result.deleted_count > 0:
        print(f"Turno {idTurno} eliminado de MongoDB.")
    else:
        print(f"No se encontró el turno {idTurno} en MongoDB.")
except Exception as e:
    print(f"Error al eliminar en MongoDB: {e}")
    sys.exit(1)
