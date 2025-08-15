import sys
import os
from pymongo import MongoClient

def parsear_turno(linea):
    campos = linea.strip().split('|')
    if len(campos) < 16:
        return None
    # Parsear fechas y horas
    try:
        fnac_dia, fnac_mes, fnac_anio = map(int, campos[7].split('/'))
        fat_dia, fat_mes, fat_anio = map(int, campos[13].split('/'))
        hora, minuto = map(int, campos[14].split(':'))
    except Exception as e:
        print("Error parseando fechas/horas:", e)
        return None
    return {
        "id": int(campos[0]),
        "paciente": {
            "id": int(campos[1]),
            "nombre": campos[2],
            "apellido": campos[3],
            "cedula": campos[4],
            "correo": campos[5],
            "telefono": campos[6],
            "fecha_nacimiento": {
                "dia": fnac_dia,
                "mes": fnac_mes,
                "anio": fnac_anio
            }
        },
        "especialista": {
            "id": int(campos[8]),
            "nombre": campos[9],
            "apellido": campos[10],
            "especialidad": campos[11],
            "cedula": campos[12]
        },
        "fecha_atencion": {
            "dia": fat_dia,
            "mes": fat_mes,
            "anio": fat_anio
        },
        "hora_atencion": {
            "hora": hora,
            "minuto": minuto
        },
        "direccion_hospital": campos[15]
    }

def sincronizar_txt_a_mongo(ruta_archivo, mongo_uri="mongodb+srv://ramirezmeza2003:MEzaJose963.@cluster0.5a3otrc.mongodb.net/", db_name="ModuloCitas", coleccion="citas"):
    if not os.path.exists(ruta_archivo):
        print("Archivo no encontrado:", ruta_archivo)
        return

    client = MongoClient(mongo_uri)
    db = client[db_name]
    col = db[coleccion]

    with open(ruta_archivo, "r", encoding="utf-8") as f:
        for linea in f:
            doc = parsear_turno(linea)
            if doc:
                col.replace_one({"id": doc["id"]}, doc, upsert=True)
    print("Sincronización completada.")

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Uso: python sincronizar_turnos.py <ruta_archivo>")
        sys.exit(1)
    ruta_archivo = sys.argv[1]
    sincronizar_txt_a_mongo(ruta_archivo)