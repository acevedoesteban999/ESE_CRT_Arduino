/////////////////////////////////ESE_CTR_Arduino v-1.0.0//////////////////////////////////////////
                                         /*
                                      Comandos
                                00000111-7   (Referenciar)
                                00001011-11  (Pintar)
                                00010011-19  (Codigo Verificacion)
                                         */
//////////////////////////////////VARIABLES GLOBALES//////////////////////////////////////////////// 
int PinA[3]={4,5,6};                 //Pines de A
int PinB[3]={7,8,9};                 //Pines de B
int LA[3];                           //LastA
int LB[3];                           //LastB
unsigned int cont=0;                 //Cont para Codigo Verificacion
const unsigned int ButReferenciar=2; //Boton para interrupcion 0
const unsigned int ButPintar=3;      //Boton para interrupcion 1
bool BoolReferenciar=false;          //Bool para interrupcion 0
bool BoolPintar=false;               //Bool para interrupcion 1
long T;
//////////////////////////////////FUNCIONES INTERRUPCION/////////////////////////////////////////////                                     
void Referenciar(){                  //Funcion de interrupcion 0
  if(millis()>T+250){                //Evitar Rebote
    BoolReferenciar=true;            //Pasar picha al loop
    T=millis();                      //Guardo Tiempo
  } 
}
void Pintar(){                       //Funcion de interrupcion 1
  if(millis()>T+250){                //Evitar Rebote
    BoolPintar=true;                 //Pasar picha al loop
    T=millis();                      //Guardo Tiempo
  }
}
//////////////////////////////////SETUP////////////////////////////////////////////////
void setup() 
{
  Serial.begin(2000000);                    //2000000  115200
  attachInterrupt(0,Referenciar,FALLING);   //Interupcion 0
  attachInterrupt(1,Pintar,FALLING);        //Interupcion 1
  T=millis();                               //Guardo Tiempo
  for (int i = 0; i<3;i++){
    pinMode(PinA[i],INPUT);                 //Pin Modo INPUT
    pinMode(PinB[i],INPUT);                 //Pin Modo INPUT
    pinMode(ButReferenciar,INPUT_PULLUP);   //Pin Modo INPUT_PULLUP
    pinMode(ButPintar,INPUT_PULLUP);        //Pin Modo INPUT_PULLUP
    LA[i] = digitalRead(PinA[i]);           // primera lectura
    LB[i]= digitalRead(PinB[i]);            // primera lectura
  }
}
//////////////////////////////////LOOP////////////////////////////////////////////////
void loop()
{
  if(BoolReferenciar){                      //Si la interrupcion manda
     int VerifData=7;                       //Codigo de Referenciar
     byte VerifByte=lowByte(VerifData);     //Solo asigno el LowByte
     Serial.write(VerifByte);               //Envio
     BoolReferenciar=false;                 //Reinicio el boton
  }
  if(BoolPintar){                           //Si la interrupcion manda
      int VerifData=11;                     //Codigo de Pintar
      byte VerifByte=lowByte(VerifData);    //Solo asigno el LowByte
      Serial.write(VerifByte);              //Envio
      BoolPintar=false;                     //Reinicio el boton
  } 
    int data=1;                             //dato a enviar 
    bool Env=false;                         //Para Enviar
    for(unsigned int i =0; i<3;i++){
      int A=digitalRead(PinA[i]);           //Leer pin
      int B=digitalRead(PinB[i]);           //Leer pin
      if(A^LA[i] | B^LB[i]){                //Verificar cambio
        Env=true;                           //enviar
        bitWrite(data,7-i,1);               //mover eje
        bitWrite(data,4-i,A^LB[i]);         //direccion del movimiento
        LA[i]=A;                            //guardo el valor anterior
        LB[i]=B;                            //guardo el valor anterior
      }
    }
    if(Env){   //Enviar
      if(++cont==10){   //Verificacion
         int VerifData=19;                  //Codigo Verificacion
         byte VerifByte=lowByte(VerifData); //Solo asigno el LowByte
         Serial.write(VerifByte);           //Envio
      }
      byte Byte=lowByte(data);              //Solo asigno el LowByte
      Serial.write(Byte);                   //Envio
    }
}
