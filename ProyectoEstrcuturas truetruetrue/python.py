import sys
import unicodedata
if sys.platform == "win32":
    import os
    os.system("chcp 65001 > nul")

def quitar_tildes(texto):
    return ''.join(c for c in unicodedata.normalize('NFD', texto)
                  if unicodedata.category(c) != 'Mn')

# Lista de hospitales (cada elemento es una tupla: (nombre, canton, lat, lon))
hospitales_lista = [
    ("Hospital Carlos Andrade Marín, Av. Universitaria, Quito, Pichincha, Ecuador", "quito", "-0.20513", "-78.50446"),
    ("Hospital San Francisco de Quito, Junín, Quito, Pichincha, Ecuador", "quito", "-0.21672", "-78.50089"),
    ("Hospital del Día Eloy Alfaro, Ayapamba, Quito, Pichincha, Ecuador", "quito", "-0.28630", "-78.51250"),
    ("Hospital del Día El Batán, Av. Eloy Alfaro N29‑235 y Rusia, Quito, Pichincha, Ecuador", "quito", "-0.17870", "-78.47515"),
    ("Hospital del Día El Inca, Av. De Los Shyris S/N y El Inca, Quito, Pichincha, Ecuador", "quito", "-0.16550", "-78.46815"),
    ("Dispensario Médico IESS Quito Norte, Eloy Alfaro y José Queri, Quito, Pichincha, Ecuador", "quito", "-0.15080", "-78.47890"),
    ("Centro Especialidades IESS Villaflora, Av. Rodrigo de Chávez S/N y Pedro de Alfaro, Quito, Pichincha, Ecuador", "quito", "-0.25270", "-78.53410"),
    ("Hospital Eugenio Espejo, Av. Gran Colombia, Quito, Pichincha, Ecuador", "quito", "-0.22007", "-78.50161"),
    ("Hospital Dermatológico Gonzalo González, Pablo Guevara y Antonio Sierra, Quito, Pichincha, Ecuador", "quito", "-0.20443", "-78.50125"),
    ("Hospital Gineco‑Obstétrico Isidro Ayora, Av. Gran Colombia y Sodiro, Quito, Pichincha, Ecuador", "quito", "-0.22157", "-78.50021"),
    ("Hospital Psiquiátrico San Lázaro, Ambato y Rafael Barahona, Quito, Pichincha, Ecuador", "quito", "-0.21680", "-78.52714"),
    ("Hospital Pablo Arturo Suárez, Angel Ludeña OE5-261, Quito, Pichincha, Ecuador", "quito", "-0.17776", "-78.52498"),
    ("Hospital Básico de Sangolquí, Luis Cordero y Quito, Sangolquí, Pichincha, Ecuador", "rumiñahui", "-0.33037", "-78.44382"),
    ("Hospital Básico de Machachi, Av. 10 de Agosto y Rocafuerte, Machachi, Pichincha, Ecuador", "mejia", "-0.51527", "-78.57712"),
    ("Hospital Básico Raúl Maldonado Mejía, Panamericana Norte km 1, Cayambe, Pichincha, Ecuador", "cayambe", "0.04076", "-78.13555"),
    ("hospital, Avenida Jaime Roldós Aguilera, Carcelén, Quito, Pichincha, 170120, Ecuador", "quito", "-0.0899942", "-78.4759803"),
    ("hospital, Avenida Diego Vasquez de Cepeda, Carcelén, Quito, Pichincha, 170302, Ecuador", "quito", "-0.0907445", "-78.4756020"),
    ("hospital, Vía Pifo El Quinche, El Calvario, La Victoria, Yaruquí, Yaruqui, Quito, Pichincha, 170908, Ecuador", "quito", "-0.1619109", "-78.3212733"),
    ("IESS Carrera, Fausto Jarrin, Izoloma, Cangahua, Cayambe, Pichincha, 171416, Ecuador", "cayambe", "-0.0571916", "-78.1551800"),
]

def buscar_hospitales_en_canton(canton):
    encontrados = [h for h in hospitales_lista if h[1] == canton.lower()]
    with open("hospitales.txt", "w", encoding="utf-8") as f:
        if encontrados:
            print(f"\n🏥 Hospitales encontrados en el cantón de {canton.capitalize()} (Pichincha):\n")
            for i, h in enumerate(encontrados, 1):
                nombre, _, lat, lon = h
                print(f"{i}. {nombre}")
                print(f"   📍 Coordenadas: ({lat}, {lon})\n")
                f.write(f"{i}|{nombre}|{lat}|{lon}\n")
        else:
            print("No se encontraron hospitales en ese cantón.")
            f.write("No se encontraron hospitales en ese cantón.\n")

# --- Ejecutar el script ---
if __name__ == "__main__":
    cantones_validos = ["quito", "cayambe", "mejia", "rumiñahui"]
    cantones_validos_sin_tilde = [quitar_tildes(c) for c in cantones_validos]
    while True:
        print("Solo se permite buscar hospitales en los siguientes cantones de Pichincha:")
        print("- Quito\n- Cayambe\n- Mejia\n- Rumiñahui\n")
        canton_usuario = input("Ingrese el nombre del cantón: ").strip().lower()
        canton_usuario_sin_tilde = quitar_tildes(canton_usuario)
        if canton_usuario_sin_tilde in cantones_validos_sin_tilde:
            # Usar el nombre original del cantón para mostrar correctamente
            idx = cantones_validos_sin_tilde.index(canton_usuario_sin_tilde)
            canton_valido = cantones_validos[idx]
            buscar_hospitales_en_canton(canton_valido.capitalize())
            break
        else:
            print("\n❌ Error: Solo puede buscar hospitales en los cantones de Quito, Cayambe, Mejia o Rumiñahui (Pichincha). Intente de nuevo.\n")
