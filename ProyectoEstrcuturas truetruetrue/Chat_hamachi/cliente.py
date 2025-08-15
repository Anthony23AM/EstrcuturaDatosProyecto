import socket
import threading

#creamos un socket TCP/IP
socket_cliente = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

#Especificamos la direccion y el puerto del servidor
direccion_servidor = ('25.44.150.125', 12345)
print('Conectandose a {} puerto {}'.format(*direccion_servidor))
socket_cliente.connect(direccion_servidor)

#Funcion para manejar los mensajes del servidor
def manejar_servidor():
    while True:
        try:
            #Recibimos mensajes del servidor
            data=socket_cliente.recv(1024).decode('utf-8')
            if not data:
                break
            mensaje_recibido = data
            print(mensaje_recibido)
        except Exception as e:
            print(f'Error: {e}')
            break

#hilo para manejar los mensajes del servidor
threading.Thread(target=manejar_servidor).start()

#solicitamos al usuario que introduzca su nombre
nombre_usuario = input('Por favor ingresa tu nombre para identificarte: ')
socket_cliente.sendall(nombre_usuario.encode('utf-8'))

#Ciclo para enviar mensajes al servidor
try:
    while True:
        mensaje = input('Escribe tu mensaje: ')
        if mensaje.lower() == 'salir':
            socket_cliente.sendall(mensaje.encode('utf-8'))
            print('Desconectandose del chat...')
            break
        socket_cliente.sendall(mensaje.encode('utf-8'))
except Exception as e:
    print(f'Error: {e}')

#cerramos la conexion con el servidor
print('Cerrando conexion con el servidor...')
socket_cliente.close()