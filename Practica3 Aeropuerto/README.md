✈ AEROPUERTO con ESP32.

Consistencia del trabajo:

Este código simula el sistema de control de aterrizajes de un aeropuerto utilizando un microcontrolador ESP32 y FreeRTOS. La idea principal es manejar varios aviones intentando aterrizar en diferentes pistas, de manera que cada pista solo puede ser utilizada por un avión a la vez. La sincronización se logra mediante el uso de semáforos (mutex) para proteger el acceso a las pistas, evitando que más de un avión aterrice en la misma pista simultáneamente.

Pasos a seguir:
1. Inicialización de colas y mutex

El código comienza definiendo tres colas (Pista1, Pista2, Pista3) para gestionar las solicitudes de aterrizaje para cada una de las tres pistas del aeropuerto. Además, se crean tres semáforos mutex (pista1, pista2, pista3) para garantizar que solo un avión pueda aterrizar en una pista a la vez.
2. Configuración inicial (setup)

En la función setup(), se crean las colas y los mutex, se inicia la comunicación serial y se configuran las tareas para que se ejecuten en diferentes núcleos del ESP32. La tarea Torre_de_Control corre en el Core 1, y la tarea Tarea_de_Aviones corre en el Core 0.
3. Función de consulta de aterrizaje (Consulta_aterrizaje)

Esta función se encarga de simular la autorización de aterrizaje de un avión en una pista. El avión espera un tiempo aleatorio antes de intentar aterrizar. Luego, intenta obtener el mutex de la pista (si la pista está libre). Si obtiene el mutex, simula el aterrizaje durante un tiempo aleatorio, y después libera la pista (libera el mutex).
Si no puede obtener el mutex, significa que la pista está ocupada y el avión no puede aterrizar en ese momento.
4. Función de la torre de control (Torre_de_Control)

La torre de control se encarga de autorizar los aterrizajes. Revisa las colas de las pistas (1, 2 y 3) y cuando encuentra un avión en una cola, autoriza su aterrizaje y llama a la función Consulta_aterrizaje correspondiente.
La torre de control también maneja la sincronización al asegurarse de que los aviones reciban permiso para aterrizar en el orden en que llegaron.
5. Función de los aviones (Tarea_de_Aviones)

Esta tarea simula el comportamiento de los aviones. Cada avión espera un tiempo aleatorio (simulando el tiempo de aproximación) y luego genera una solicitud de aterrizaje, que se envía a la cola correspondiente según la pista seleccionada aleatoriamente (1, 2 o 3).
6. Comunicación y sincronización

La comunicación entre las tareas (aviones y torre de control) se maneja a través de colas (xQueueSend y xQueueReceive), y la sincronización de los aterrizajes se maneja con semáforos mutex para garantizar que una pista no sea utilizada por más de un avión a la vez.
Los aviones envían su solicitud a la cola correspondiente.


La torre de control autoriza el aterrizaje, haciendo que el avión intente tomar el mutex de la pista.


Una vez que el avión aterriza, libera el mutex para permitir que otros aviones puedan usar la pista.

7. Función loop()
La función loop() queda vacía, ya que todo el trabajo se realiza dentro de las tareas definidas en FreeRTOS. Las tareas son las que gestionan el flujo del programa en lugar de un bucle principal.


