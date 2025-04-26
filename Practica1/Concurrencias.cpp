void taskFunction(void * pvParameters) {

  for(;;){

    int Resultado = xPortGetCoreID(); 
    vTaskDelay(pdMS_TO_TICKS(1000));
    Serial.println(Resultado);
  }
}

void setup() {
  
  Serial.begin(9600);
  xTaskCreatePinnedToCore(
  taskFunction, // Referencia a la funcion que vamos a ejecutar
  "Task_Core1", // Nombre para la funcion, sirve solamente para propositos de debugging
  4096, // Tamaño del stack la tarea
  NULL, // Parametro que recibe la funcion que le vamos a pasar
  1, // Prioridad de la tarea
  NULL, // no es importante
  1 // El core donde queremos que corra la task (0/1)
  );

  xTaskCreatePinnedToCore(
  taskFunction, // Referencia a la funcion que vamos a ejecutar
  "Task_Core0", // Nombre para la funcion, sirve solamente para propositos de debugging
  4096, // Tamaño del stack la tarea
  NULL, // Parametro que recibe la funcion que le vamos a pasar
  1, // Prioridad de la tarea
  NULL, // no es importante
  0 // El core donde queremos que corra la task (0/1)
  );

}

void loop() {
  
}