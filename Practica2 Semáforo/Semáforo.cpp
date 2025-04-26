SemaphoreHandle_t mySemaphore; // Declaración de un identificador para el MUTEX, utilizado para sincronizar el acceso entre las dos tareas 

// Definición de pines para LEDs del semáforo controlado por core 1.

#define RED_1 27
#define YELLOW_1 26
#define GREEN_1 25

// Definición de pines para LEDs del semáforo controlado por core 0.

#define RED_0 5
#define YELLOW_0 18
#define GREEN_0 19

// Variable para controlar qué semáforo tiene el turno activo.

bool volatile semaforoActivo = true;

void setup() {
  
  // Creación del MUTEX para sincronizar los semáforos.

  mySemaphore = xSemaphoreCreateMutex();

  Serial.begin(9600);

  // Creación de la tarea que controlará el semáforo del core 1.

  xTaskCreatePinnedToCore(
  semaphore1, // Referencia a la funcion que vamos a ejecutar
  "Task_Core1", // Nombre para la funcion, sirve solamente para propositos de debugging
  4096, // Tamaño del stack la tarea
  NULL, // Parametro que recibe la funcion que le vamos a pasar
  1, // Prioridad de la tarea
  NULL, // no es importante
  1 // El core donde queremos que corra la task (0/1)
  );

  // Creación de la tarea que controlará el semáforo del core 0.

  xTaskCreatePinnedToCore(
  semaphore0, // Referencia a la funcion que vamos a ejecutar
  "Task_Core0", // Nombre para la funcion, sirve solamente para propositos de debugging
  4096, // Tamaño del stack la tarea
  NULL, // Parametro que recibe la funcion que le vamos a pasar
  1, // Prioridad de la tarea
  NULL, // no es importante
  0 // El core donde queremos que corra la task (0/1)
  );

  // Configuración y estado inicial de pines de los LEDs.

  pinMode(YELLOW_1, OUTPUT);
  pinMode(GREEN_1, OUTPUT);
  pinMode(RED_1, OUTPUT);

  pinMode(YELLOW_0, OUTPUT);
  pinMode(GREEN_0, OUTPUT);
  pinMode(RED_0, OUTPUT);

  digitalWrite(YELLOW_1, LOW);
  digitalWrite(GREEN_1, LOW);
  digitalWrite(RED_1, HIGH);

  digitalWrite(YELLOW_0, LOW);
  digitalWrite(GREEN_0, LOW);
  digitalWrite(RED_0, HIGH);
}

// Tarea que controla el semáforo conectado al core 1

void semaphore1(void * pvParameters) {

  for(;;){

    if(semaforoActivo){ // Solo actúa si es el turno de este semáforo

      if(xSemaphoreTake(mySemaphore, portMAX_DELAY) == pdTRUE) {  // Toma el MUTEX

        // Secuencia del semáforo.  

        digitalWrite(RED_1, LOW);
        digitalWrite(GREEN_1, HIGH);
        delay(3500);

        digitalWrite(GREEN_1, LOW);
        digitalWrite(YELLOW_1, HIGH);
        delay(750);

        digitalWrite(RED_1, HIGH);
        digitalWrite(YELLOW_1, LOW);

        semaforoActivo = false; // Pasa el turno al otro semáforo
        xSemaphoreGive(mySemaphore); // Libera el MUTEX
      }
    }

    delay(10); // Pequeño retardo para no saturar el procesador
  }
}

// Tarea que controla el semáforo conectado al core 0

void semaphore0(void * pvParameters) {

  for(;;){

    if(!semaforoActivo){

      if(xSemaphoreTake(mySemaphore, portMAX_DELAY) == pdTRUE) {  

        // Secuencia del semáforo.

        digitalWrite(RED_0, LOW);
        digitalWrite(GREEN_0, HIGH);
        delay(3500);

        digitalWrite(GREEN_0, LOW);
        digitalWrite(YELLOW_0, HIGH);
        delay(750);

        digitalWrite(RED_0, HIGH);
        digitalWrite(YELLOW_0, LOW);

        semaforoActivo = true;
        xSemaphoreGive(mySemaphore);
      }
    }

    delay(10);
  }
}

void loop() {
  
}