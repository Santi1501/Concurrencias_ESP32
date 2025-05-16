
#include "data1.h"

// Colas para compartir comandos LED entre productor y consumidor
QueueHandle_t Pista1, Pista2, Pista3;

SemaphoreHandle_t pista1, pista2, pista3;

// Configuración inicial
void setup() {
  
  // 
  Pista1 = xQueueCreate(3, sizeof(AvionCommand));
  Pista2 = xQueueCreate(3, sizeof(AvionCommand));
  Pista3 = xQueueCreate(3, sizeof(AvionCommand));
  
  pista1 = xSemaphoreCreateMutex();
  pista2 = xSemaphoreCreateMutex();
  pista3 = xSemaphoreCreateMutex();

  // Iniciar la comunicación serial
  Serial.begin(9600);

  // Crear tarea del consumidor en el Core 1
  xTaskCreatePinnedToCore(
  Torre_de_Control, // Referencia a la funcion que vamos a ejecutar
  "Task_Core1", // Nombre para la funcion, sirve solamente para propositos de debugging
  4096, // Tamaño del stack la tarea
  NULL, // Parametro que recibe la funcion que le vamos a pasar
  1, // Prioridad de la tarea
  NULL, // no es importante
  1 // El core donde queremos que corra la task (0/1)
  );

  // Crear tarea productor en el Core 0
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

void Consulta_aterrizaje(AvionCommand avion, SemaphoreHandle_t pista){

  Serial.print("Avión ");
  Serial.print(avion);
  Serial.println(" está intentando aterrizar.");

  // Espera para simular tiempo de aproximación
  delay(random(1000, 5000));

  if(xSemaphoreTake(pista, portMAX_DELAY) == pdTRUE) {  // Toma el MUTEX

    Serial.print("Avión ");
    Serial.print(avion);
    Serial.println(" ha aterrizado en la pista.");

    delay(random(2000, 4000));

    Serial.print("Avión ");
    Serial.print(avion);
    Serial.println(" ha liberado la pista.");

    xSemaphoreGive(pista);
  }

  else {
    Serial.print("Avión ");
    Serial.print(avion);
    Serial.println(" no pudo aterrizar, pista ocupada.");
  }
}

void Torre_de_Control(void * pvParameters) {

  AvionCommand avion;

  for(;;){

    if (xQueueReceive(Pista1, &avion, 0) == pdTRUE) {
      
      Serial.print("Torre de Control: Autorizando aterrizaje del avión ");
      Serial.print(avion);
      Serial.println(" en pista 1.");
      Consulta_aterrizaje(avion, pista1);
    }

    if (xQueueReceive(Pista2, &avion, 0) == pdTRUE) {
      
      Serial.print("Torre de Control: Autorizando aterrizaje del avión ");
      Serial.print(avion);
      Serial.println(" en pista 2.");
      Consulta_aterrizaje(avion, pista2);
    }

    if (xQueueReceive(Pista3, &avion, 0) == pdTRUE) {
      
      Serial.print("Torre de Control: Autorizando aterrizaje del avión ");
      Serial.print(avion);
      Serial.println(" en pista 3.");
      Consulta_aterrizaje(avion, pista3);
    }
  }
}

void Tarea_de_Aviones(void * pvParameters) {

  for(;;){

    // Espera aleatoria para simular el tiempo de aproximación
    delay(random(1000, 5000));

    AvionCommand avion = randomCommand();

    Serial.print("Avión ");
    Serial.print(avion);
    Serial.println(" solicita aterrizaje.");

    int pistaSeleccionada = random(1, 4);  // Aleatoriamente seleccionamos una pista (1, 2 o 3)

    Serial.print("Avión ");
    Serial.print(avion);
    Serial.print(" solicita aterrizar en pista ");
    Serial.println(pistaSeleccionada);

    // Enviar la solicitud de aterrizaje a la cola correspondiente
    if (pistaSeleccionada == 1) {
      
      xQueueSend(Pista1, &avion, portMAX_DELAY);
    } 
    
    else if (pistaSeleccionada == 2) {
      
      xQueueSend(Pista2, &avion, portMAX_DELAY);
    } 
    
    else if (pistaSeleccionada == 3) {
      
      xQueueSend(Pista3, &avion, portMAX_DELAY);
    }

    delay(10); // Pequeño retardo para no saturar el procesador
  }
}

// Loop vacío: no se utiliza en FreeRTOS, todo corre en las tareas
void loop() {

}
