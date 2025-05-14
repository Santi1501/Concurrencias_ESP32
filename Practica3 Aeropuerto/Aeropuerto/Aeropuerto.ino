// Cola para compartir comandos LED entre productor y consumidor
QueueHandle_t myQueue;

SemaphoreHandle_t pista1, pista2, pista3;

bool volatile semaforoActivo = true;

// Configuración inicial
void setup() {
  
  // Crear una cola con capacidad para 5 comandos LED
  myQueue = xQueueCreate(5, sizeof(LedCommand));
  
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

void Tarea_de_Aviones(void * pvParameters) {

  for(;;){

    delay(1000);

    AvionCommand avion = randomCommand();

    xQueueSend(myQueue, &avion, portMAX_DELAY);

    if(semaforoActivo){ // Solo actúa si es el turno de este semáforo

      if(xSemaphoreTake(pista1, portMAX_DELAY) == pdTRUE) {  // Toma el MUTEX

      
        /*semaforoActivo = false; // Pasa el turno al otro semáforo
        xSemaphoreGive(mySemaphore); // Libera el MUTEX */
      }

      if(xSemaphoreTake(pista2, portMAX_DELAY) == pdTRUE) {  // Toma el MUTEX

      
        /*semaforoActivo = false; // Pasa el turno al otro semáforo
        xSemaphoreGive(mySemaphore); // Libera el MUTEX */
      }

      if(xSemaphoreTake(pista3, portMAX_DELAY) == pdTRUE) {  // Toma el MUTEX

      
        /*semaforoActivo = false; // Pasa el turno al otro semáforo
        xSemaphoreGive(mySemaphore); // Libera el MUTEX */
      }
    }

    delay(10); // Pequeño retardo para no saturar el procesador
  }

}

void Torre_de_Control(void * pvParameters) {


}

// Loop vacío: no se utiliza en FreeRTOS, todo corre en las tareas
void loop() {

}
