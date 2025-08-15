import sys
import qrcode
from fpdf import FPDF

# Recibe el texto para el QR y el nombre del archivo PDF
if len(sys.argv) < 3:
    print("Uso: python generar_qr_pdf.py 'texto' 'archivo.pdf'")
    sys.exit(1)

texto = sys.argv[1]
archivo_pdf = sys.argv[2]

# Generar imagen QR
img = qrcode.make(texto)
img.save("temp_qr.png")

# Crear PDF con solo el QR
pdf = FPDF()
pdf.add_page()
pdf.image("temp_qr.png", x=60, y=60, w=90, h=90)
pdf.output(archivo_pdf)

# Eliminar imagen temporal si se desea
import os
os.remove("temp_qr.png")
