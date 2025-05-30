import os
import time
import random

FILAS, COLUMNAS = 8, 8
TARIFA = 50  # por minuto
ENTRADA = (0, 0)
SALIDA = (FILAS - 1, COLUMNAS - 1)

mapa = [["." for _ in range(COLUMNAS)] for _ in range(FILAS)]
registro = {}

def mostrar_mapa():
    os.system('cls' if os.name == 'nt' else 'clear')
    for i in range(FILAS):
        fila = ""
        for j in range(COLUMNAS):
            if (i, j) == ENTRADA:
                fila += "E "
            elif (i, j) == SALIDA:
                fila += "S "
            elif (i, j) in registro:
                fila += "C "
            else:
                fila += ". "
        print(fila)

def ingresar_carro():
    placa = input("Ingrese la placa del vehículo: ")
    if not placa:
        return
    # Buscar espacio libre aleatorio
    libres = [(i, j) for i in range(FILAS) for j in range(COLUMNAS)
              if (i, j) not in registro and (i, j) != ENTRADA and (i, j) != SALIDA]
    if not libres:
        print("No hay espacios disponibles.")
        return
    destino = random.choice(libres)
    registro[destino] = {"placa": placa, "entrada": time.time()}
    animar_movimiento(ENTRADA, destino)

def retirar_carro():
    placa = input("Ingrese la placa del vehículo a retirar: ")
    pos = None
    for ubic, datos in registro.items():
        if datos["placa"] == placa:
            pos = ubic
            break
    if not pos:
        print("Vehículo no encontrado.")
        return
    tiempo = (time.time() - registro[pos]["entrada"]) / 60
    pago = tiempo * TARIFA
    animar_movimiento(pos, SALIDA)
    print(f"Vehículo {placa} retirado. Tiempo: {tiempo:.2f} min. Total: ${pago:.2f}")
    del registro[pos]

def animar_movimiento(inicio, fin):
    camino = calcular_ruta(inicio, fin)
    for paso in camino:
        mostrar_temp(paso)
        time.sleep(0.2)

def mostrar_temp(pos_c):
    os.system('cls' if os.name == 'nt' else 'clear')
    for i in range(FILAS):
        fila = ""
        for j in range(COLUMNAS):
            if (i, j) == pos_c:
                fila += "@ "  # el carro en movimiento
            elif (i, j) == ENTRADA:
                fila += "E "
            elif (i, j) == SALIDA:
                fila += "S "
            elif (i, j) in registro:
                fila += "C "
            else:
                fila += ". "
        print(fila)

def calcular_ruta(inicio, fin):
    ruta = []
    x1, y1 = inicio
    x2, y2 = fin
    while x1 != x2 or y1 != y2:
        if x1 < x2:
            x1 += 1
        elif x1 > x2:
            x1 -= 1
        elif y1 < y2:
            y1 += 1
        elif y1 > y2:
            y1 -= 1
        ruta.append((x1, y1))
    return ruta

def menu():
    while True:
        mostrar_mapa()
        print("\n1. Ingresar vehículo")
        print("2. Retirar vehículo")
        print("3. Salir")
        opc = input("Seleccione una opción: ")
        if opc == '1':
            ingresar_carro()
        elif opc == '2':
            retirar_carro()
        elif opc == '3':
            break
        else:
            print("Opción no válida")
            time.sleep(1)

menu()
