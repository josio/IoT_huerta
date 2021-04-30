#include <DHT.h>    // importa la Librerias DHT
#include <DHT_U.h>
const int SensorHSuelo = A0;
int SENSOR = 2;     // pin DATA de DHT11 a pin digital 2
int TEMPERATURA;
int HUMEDAD;
int HUMEDAD_SUELO;//Agregar variables si gregar si se agregan mas sensores
DHT dht(SENSOR, DHT11);    // creacion del objeto, cambiar segundo parametro
        // por DHT11 si se utiliza en lugar del DHT22
    int Humedad_S(void);
    
void setup() {
 Serial1.begin(19200);
 Serial.begin(19200);
 
}

void loop() {

/*variables con datos ramdon para simular sensores
Agregar uno por cada sensor
Por cada sensor agregar los campos en la cuenta de thingspeak*/
TEMPERATURA=dht.readTemperature();  // obtencion de valor de temperatura
 HUMEDAD = dht.readHumidity();   // obtencion de valor de humedad
 HUMEDAD_SUELO=Humedad_S(); //valores de 0 a 100%
  Serial.print("Temperatura: ");  // escritura en monitor serial de los valores
  Serial.print(TEMPERATURA);
  Serial.print(" Humedad: ");
  Serial.println(HUMEDAD);
  Serial.print(" Humedad del suelo: ");
  Serial.println(HUMEDAD_SUELO);
  
  delay(500);
                         


Connect2Server();       // Estableciendo conexion
SendingData(TEMPERATURA,"field1");
//Field1(sensor1,"field1");       // Subiendo al campo 1 de Thingspeak
EndConnection();        // Terminando la conexion
 delay(1000);
Connect2Server();
SendingData(HUMEDAD_SUELO,"field2");
 
EndConnection(); 
 delay(1000);
Connect2Server();
SendingData(HUMEDAD,"field3");
 
EndConnection();

}

//metodo para conexin al servidor
void Connect2Server()
{//cuidado al reducir mucho los delay
  Serial1.println("AT");
  delay(300);

  Serial1.println("AT+CPIN?");
  delay(300);

  Serial1.println("AT+CREG?");
  delay(300);

  Serial1.println("AT+CGATT?");
  delay(300);

  Serial1.println("AT+CIPSHUT");
  delay(300);

  Serial1.println("AT+CIPSTATUS");
  delay(1000);

  Serial1.println("AT+CIPMUX=0");
  delay(1000);

  ShowSerialData();

  Serial1.println("AT+CSTT=\"internet\"");//inicia la tarea y configura el APN
  delay(300);

  ShowSerialData();
   
  Serial1.println("AT+CIICR");//abre la conexion inalambrica
  delay(2000);

  ShowSerialData();

  Serial1.println("AT+CIFSR");//obtiene la direccion IP local
  delay(2000);

  ShowSerialData();

  Serial1.println("AT+CIPSPRT=0");
  delay(1000);

  ShowSerialData();

  Serial1.println("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",\"80\"");//inicia la conexion
  delay(2000);

  ShowSerialData();

  Serial1.println("AT+CIPSEND");//inicia/abre el envio de datos al servidor remoto
  delay(2000);
  ShowSerialData();
}

//metodo para cerrar la conexion
void EndConnection() {
  Serial1.println("AT+CIPSHUT");//cierra la conexion
  delay(100);
  ShowSerialData();

}

//metodo para mostrar por consola los mensajes intercmbiados con el servidor
void ShowSerialData()
{
  while(Serial1.available()!=0)
    Serial.write(Serial1.read());
}

//nueva version reutilizacion de codigo
//metodo para enviar datos a cada uno de los campos definidos en thinkspeak

void SendingData(int data, String field) {
  String str = "GET http://api.thingspeak.com/update?api_key=SQ4AV8JCUDOPNG8N&" + field + "=" + String (data);
  Serial1.println(str);//envia datos al servidor remoto

  delay(2000);
  ShowSerialData();
  Serial1.println((char)26);//enviando
  delay(4000);//esperando respuesta, imortante! el tiempo de respuesta se basa en la conexion de internet
  Serial1.println();

  ShowSerialData();

}

int Humedad_S(void)
  {int humedadsuelo;
   int sensor = analogRead(SensorHSuelo);
     //se debe tener en cuenta que para valores entre 0 y 240 el sensor está totalmente sumergido,
    //estos rangos se deben calibrar segun el sensor, 
    //se calibra tomandos los valores de conversión por cada raya del sensor sumergida
    if(sensor<=240)
   humedadsuelo=100;
   else if(sensor>240 && sensor<260)
   humedadsuelo=90;
   else if(sensor>=260 && sensor<280)
   humedadsuelo=80;
   else if(sensor>=280 && sensor<320)
    humedadsuelo=70;
   else if (sensor>=320 && sensor<380)
   humedadsuelo=60;
   else if (sensor>=380 && sensor<500)
   humedadsuelo=50;
   else if (sensor>=500 && sensor<600)
   humedadsuelo=40;
    else if (sensor>=600 && sensor<700)
   humedadsuelo=30;
    else if (sensor>=700 && sensor<800)
   humedadsuelo=20;
    else if (sensor>=800 && sensor<900)
   humedadsuelo=10;
   else humedadsuelo=0;
  //Serial.println(humedadsuelo);
  return humedadsuelo; }
