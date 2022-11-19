#include <LiquidCrystal.h>

//Crear el objeto LCD con los números correspondientes (rs, en, d4, d5, d6, d7)
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

//Sensor de corriente
float Sensibilidad=0.066; //sensibilidad en V/A para nuestro sensor
float offset=-0.50; // Equivale a la amplitud del ruido

//potenciometro
float Idis;
int count;
int RelayPin1 = 10;
int RelayPin2 = 11;
int RelayPin3 = 12;
int RelayPin4 = 13;

void setup() {
  // Inicializar el LCD con el número de  columnas y filas del LCD
  lcd.begin(16, 2);
  // Escribimos el Mensaje en el LCD.
  // lcd.print("Sen Corriente");

  // Set RelayPin1 as an output pin
	pinMode(RelayPin1, OUTPUT);
	pinMode(RelayPin2, OUTPUT);
	pinMode(RelayPin3, OUTPUT);
	pinMode(RelayPin4, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // Ubicamos el cursor en la primera posición(columna:0) de la segunda línea(fila:1)
  // lcd.setCursor(0, 1);
  // Escribimos el número de segundos trascurridos
  // lcd.print(millis()/1000);
  // lcd.print(" Segundos");
  // delay(100);

  float Ip=get_corriente();//obtenemos la corriente pico
  float Irms=Ip*0.707; //Intensidad RMS = Ipico/(2^1/2)
  // float P=Irms*220.0; // P=IV watts
  Serial.print("Ip: ");
  Serial.print(Ip,3);
  Serial.print("A , Irms: ");
  Serial.print(Irms,3);
  // Serial.print("A, Potencia: ");
  // Serial.print(P,3);  
  // Serial.println("W");

  //potenciometro
  //valor = analogRead(A3);
  Idis = analogRead(A3) * (10.0 / 1023.0);

  if (Irms<Idis){
    digitalWrite(RelayPin1,HIGH);
    digitalWrite(RelayPin2,HIGH);
    digitalWrite(RelayPin3,HIGH);
    digitalWrite(RelayPin4,HIGH);
    delay(1000);
  }else{
    Serial.print("Normal");
    digitalWrite(RelayPin1,LOW);
    digitalWrite(RelayPin2,LOW);
    digitalWrite(RelayPin3,LOW);
    digitalWrite(RelayPin4,LOW);
    Serial.print("disparo por sobrecorriente");
    count= count + 1;
    delay(1000);
  }

 
  //Imprimimos por el monitor serie
  Serial.print("El valor es = ");
  Serial.println(Idis);

  lcd.setCursor(0, 0);
  lcd.print("Ip");
  lcd.setCursor(3, 0);
  lcd.print(Ip);

  lcd.setCursor(9, 0);
  lcd.print("#");
  lcd.setCursor(11, 0);
  lcd.print(count);


  lcd.setCursor(0, 1);
  lcd.print("Im");
  lcd.setCursor(3, 1);
  lcd.print(Irms);

  lcd.setCursor(9, 1);
  lcd.print("Id");
  lcd.setCursor(12, 1);
  lcd.print(Idis);
  delay(100);
}
float get_corriente()
{
  float voltajeSensor;
  float corriente=0;
  long tiempo=millis();
  float Imax=0;
  float Imin=0;
  while(millis()-tiempo<500)//realizamos mediciones durante 0.5 segundos
  { 
    voltajeSensor = analogRead(A0) * (5.0 / 1023.0);//lectura del sensor
    corriente=0.9*corriente+0.1*((voltajeSensor-2.498)/Sensibilidad); //Ecuación  para obtener la corriente
    if(corriente>Imax)Imax=corriente;
    if(corriente<Imin)Imin=corriente;
  }
  return(((Imax-Imin)/2)-offset);
}