# IoT Test Program

El objetivo de este repositorio es dar un ejemplo de un cliente en Arduino usando ESP32 como tarjeta de desarrollo.
El código de este repositorio está pensado como ejemplo y no se espera que se use como solución definitiva.

### Arduino

Se utliza un sensor digital de temperatura y humedad y un sensor analógico de humedad
Sensores:
- DHT11 -> sensor digital de temperatura y humedad (conectado al pin D2)
- Sensor capacitivo -> sensor analógico que mide la humedad (conectado al pin A0)

En el archivo secrets.h se tiene que modificar con la configuración de red y las respectivas credenciales.

>[!IMPORTANT]
>No tener cables conectados al pin D2 al subir código


### API Testing

La carpeta test-api contiene una API rest que permite probar el código.
Se puede iniciar en la terminal con el siguiente comando:
```
node test-api.js
```

> [!NOTE]
> No está pensada para producción ya que no está estructurada para escalabilidad y mantenimiento


Última revisión: 22 Noviembre 2024
