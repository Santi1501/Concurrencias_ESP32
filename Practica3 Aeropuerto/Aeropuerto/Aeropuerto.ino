// Se incluye la librería 'data1.h' que define el tipo 'AvionCommand' y otras funciones
#include "data1.h"

// Definición de 3 colas para las solicitudes de aterrizaje en cada pista
QueueHandle_t Pista1, Pista2, Pista3;

// Definición de 3 semáforos mutex, uno para cada pista
SemaphoreHandle_t pista1, pista2, pista3;

// Configuración inicial
void setup() {
  
  // Crear colas con capacidad para 3 elementos (hasta 3 aviones pueden esperar para aterrizar en cada pista)
  Pista1 = xQueueCreate(3, sizeof(AvionCommand));
  Pista2 = xQueueCreate(3, sizeof(AvionCommand));
  Pista3 = xQueueCreate(3, sizeof(AvionCommand));
  
  // Crear mutex para cada pista (para asegurar acceso exclusivo a las pistas)
  pista1 = xSemaphoreCreateMutex();
  pista2 = xSemaphoreCreateMutex();
  pista3 = xSemaphoreCreateMutex();

  // Iniciar la comunicación serial
  Serial.begin(9600);

  // Crear tarea Torre_de_Control en el Core 1
  xTaskCreatePinnedToCore(
  Torre_de_Control, // Referencia a la funcion que vamos a ejecutar
  "Task_Core1", // Nombre para la funcion, sirve solamente para propositos de debugging
  4096, // Tamaño del stack la tarea
  NULL, // Parametro que recibe la funcion que le vamos a pasar
  1, // Prioridad de la tarea
  NULL, // no es importante
  1 // El core donde queremos que corra la task (0/1)
  );

  // Crear tarea Tarea_de_Aviones en el Core 0
  xTaskCreatePinnedToCore(
  Tarea_de_Aviones, // Referencia a la funcion que vamos a ejecutar
  "Task_Core0", // Nombre para la funcion, sirve solamente para propositos de debugging
  4096, // Tamaño del stack la tarea
  NULL, // Parametro que recibe la funcion que le vamos a pasar
  1, // Prioridad de la tarea
  NULL, // no es importante
  0 // El core donde queremos que corra la task (0/1)
  );
}

// Función que simula la autorización de aterrizaje de un avión en una pista específica
void Consulta_aterrizaje(AvionCommand avion, SemaphoreHandle_t pista){

  Serial.print("Avión ");
  Serial.print(avion); // Imprime el comando del avión
  Serial.println(" está intentando aterrizar.");

  // Espera aleatoria para simular el tiempo de aproximación del avión
  delay(random(1000, 5000));

  // Intentar tomar el mutex (debe obtenerlo para aterrizar el avión)
  if(xSemaphoreTake(pista, portMAX_DELAY) == pdTRUE) {  // Si logra tomar el mutex de la pista

    Serial.println("Avión ");
    Serial.println(avion); // Imprime el avión que aterrizó
    Serial.println(" ha aterrizado en la pista.");

    // Simula un tiempo de aterrizaje
    delay(random(2000, 4000));

    Serial.println("Avión ");
    Serial.println(avion); // Imprime que el avión desocupó la pista
    Serial.println(" ha liberado la pista.");

    // Libera el mutex de la pista, dejando la pista libre
    xSemaphoreGive(pista);
  }

  // Si no pudo tomar el mutex, significa que la pista estaba ocupada
  else {
    Serial.println("Avión ");
    Serial.println(avion);
    Serial.println(" no pudo aterrizar, pista ocupada.");
  }
}

// Tarea que actúa como la torre de control, gestionando los aterrizajes
void Torre_de_Control(void * pvParameters) {

  AvionCommand avion; // Variable para almacenar el comando de un avión

  for(;;){
    
    // Revisamos cada cola de pistas para ver si hay un avión esperando
    if (xQueueReceive(Pista1, &avion, 0) == pdTRUE) { // Si hay un avión en la cola de la pista 1
      
      Serial.println("Torre de Control: Autorizando aterrizaje del avión ");
      Serial.println(avion); // Imprime el avión que intenta aterrizar
      Serial.println(" en pista 1.");
      Consulta_aterrizaje(avion, pista1); // Llama a la función para aterrizarlo en la pista 1
    }

    if (xQueueReceive(Pista2, &avion, 0) == pdTRUE) { // Si hay un avión en la cola de la pista 2
      
      Serial.println("Torre de Control: Autorizando aterrizaje del avión ");
      Serial.println(avion);
      Serial.println(" en pista 2.");
      Consulta_aterrizaje(avion, pista2); // Llama a la función para aterrizarlo en la pista 2
    }

    if (xQueueReceive(Pista3, &avion, 0) == pdTRUE) { // Si hay un avión en la cola de la pista 3
      
      Serial.println("Torre de Control: Autorizando aterrizaje del avión ");
      Serial.println(avion);
      Serial.println(" en pista 3.");
      Consulta_aterrizaje(avion, pista3);  // Llama a la función para aterrizarlo en la pista 3
    }
    }
  }
}

// Tarea que actúa como el productor de aviones (generando solicitudes de aterrizaje)
void Tarea_de_Aviones(void * pvParameters) {

  for(;;){

    // Espera aleatoria para simular el tiempo de aproximación de los aviones
    delay(random(1000, 5000));

     // Generar un comando de avión aleatorio
    AvionCommand avion = randomCommand();

    // Aleatoriamente seleccionamos una pista (1, 2 o 3)
    int pistaSeleccionada = random(1, 4); 

    Serial.println("Avión ");
    Serial.println(avion); // Imprime el comando del avión que solicita el aterrizaje
    Serial.println(" solicita aterrizar en pista ");
    Serial.println(pistaSeleccionada);

    // Enviar la solicitud de aterrizaje a la cola correspondiente
    if (pistaSeleccionada == 1) { // Envia el avión a la cola de la pista 1
      
      xQueueSend(Pista1, &avion, portMAX_DELAY);
    } 
    
    else if (pistaSeleccionada == 2) { // Envia el avión a la cola de la pista 2
      
      xQueueSend(Pista2, &avion, portMAX_DELAY);
    } 
    
    else if (pistaSeleccionada == 3) { // Envia el avión a la cola de la pista 3
      
      xQueueSend(Pista3, &avion, portMAX_DELAY);
    }

    delay(10); // Pequeño retardo para no saturar el procesador
  }
}

// Loop vacío: no se utiliza en FreeRTOS, todo corre en las tareas
void loop() {

}
