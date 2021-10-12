import  serial # Librería para obtener datos del arduino
import datetime as dt
import matplotlib.pyplot as plt
import matplotlib.animation as animation

# Se crea una figura para plotting y se definen parámetros tipo arreglos
fig = plt.figure()
at = fig.add_subplot(1, 1, 1)

xs = []
ys = []

# Iniciamos el puerto y conección
puerto = 'COM7'
arduino = serial.Serial(puerto, 9600)

def mainApp():
    # Realiza un llamado recursivo a la visualización
    visualizar('', xs, ys)
    ani = animation.FuncAnimation(fig, visualizar, fargs=(xs, ys), interval=1000)
    plt.show()
    
# Esta función es llamada períodicamente de FuncAnimation
def visualizar(i, xs, ys):
    #Arreglos para guardar los datos como float de la lectura 
    list_in_floats = []
    
    # Lee los datos, se taren en la lista como datos float
    leerDato(list_in_floats)
    temperaturaC = list_in_floats[0]
    
    # Adiciona los valores X y Y de los arreglos
    xs.append(dt.datetime.now().strftime('%H:%M:%S'))
    ys.append(temperaturaC)

    # Limita X and Y lists to 20 items
    xs = xs[-10:]
    ys = ys[-10:]
    
    # Muestra los datos
    at.clear()
    at.plot(xs, ys)

    # Formatea el Plot
    plt.xticks(rotation=45, fontsize = 7, ha='right')
    plt.subplots_adjust(bottom=0.30)
    plt.title('Variación de la temperatura en el tiempo')
    plt.ylabel('Temperatura oC')
    plt.yticks(fontsize = 7)

def leerDato (list_in_floats):
    #Inicio arreglo para capturar los datos
    list_values = []

    #Leo los datos del arduino
    arduino_data = arduino.readline()
    decoded_values = str(arduino_data[0:len(arduino_data)].decode("utf-8"))
    list_values = decoded_values.split('*')
    
    #Trasnformo los datos en float para devolver 
    for item in list_values:
        list_in_floats.append(float(item))
        
    #arduino_data = 0
    #arduino.close()
    
if __name__ == '__main__':
    mainApp()


