🚦Semáforo con ESP32

Consistencia del trabajo:

El objetivo principal de este trabajo es lograr que dos semáforos funcionen de forma coordinada, cada uno ejecutándose en un core diferente mediante el uso de multitarea. Se buscó que ambos semáforos intercambien su estado de manera continua sin entrar en conflicto, garantizando que no estén en verde al mismo tiempo. Para lograr esta sincronización, se utilizó un MUTEX (semaphore de exclusión mutua), que permite controlar el acceso compartido a los recursos críticos, asegurando que solo un semáforo pueda cambiar su estado a la vez.

La multitarea, por su parte, permite que cada semáforo funcione de manera independiente en paralelo, aprovechando los dos núcleos del microcontrolador para una ejecución más eficiente y ordenada del sistema.

Pasos a seguir:

Definiciones globales: Antes de entrar en las funciones, se definieron los pines que manejan los LEDs de cada semáforo (uno por cada core del procesador). También se declaró una variable booleana (semaforoActivo), que permite coordinar qué semáforo debe estar activo en un momento dado. Además, se creó un identificador mySemaphore () que será el encargado de gestionar la sincronización mediante un MUTEX, asegurando que ambos semáforos no se activen en verde al mismo tiempo.

Función setup():En la función setup() se inicializó la comunicación serial, se creó el MUTEX con xSemaphoreCreateMutex() y se configuraron los modos de los pines (entrada/salida). También se lanzaron dos tareas (semaphore1 y semaphore0) utilizando xTaskCreatePinnedToCore(), fijando cada tarea a un core diferente. Finalmente, se inicializó el estado de los LEDs para que ambos semáforos comiencen en rojo.

Función loop ( ) : La función loop() queda vacía porque todo el trabajo se realiza en las tareas creadas manualmente.

Tareas semaphore1() y semaphore0(): Ambas tareas se ejecutan en cores distintos (semaphore1 en el core 1 y semaphore0 en el core 0) y controlan su respectivo semáforo. Cada tarea espera su turno para actuar según el estado de la variable semaforoActivo, intentando tomar el MUTEX para asegurar que solo un semáforo esté en verde a la vez. Una vez obtenido el control, cada tarea cambia el estado de los LEDs (rojo → verde → amarillo → rojo), respetando tiempos de espera, y al finalizar libera el MUTEX y cambia el valor de semaforoActivo para ceder el turno al otro semáforo.
