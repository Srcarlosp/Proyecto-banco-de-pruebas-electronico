///////////////////////////// Declaracion de Librerias /////////////////////////////

#include <LiquidCrystal.h>

///////////////////////////// Declaracion de Pines /////////////////////////////

//Multiplexor
int Y = 4;
int A = 5;
int B = 6;
int C = 7;
//Shift
int SerialCK=3;
int ParallelCK=2;
int SerialD=1;
//Voltimetros
int voltimetro0 = A0;
int voltimetro1 = A1;
int voltimetro2 = A2;
int voltimetro3 = A3;
int voltimetro4 = A4;
int voltimetro5 = A5;
///////////////////////////// Control /////////////////////////////
//IO
float valorVoltimetros[6]; //Guarda las lecturas de los voltimetros en orden
int muxRead[8] = {0,1,0,1,0,1,0,1};
int shiftWrite[8] = {0,0,0,0,0,0,0,0};
int controlOuput[8] = {0,0,0,0,0,0,0,0};
//Estados
int estadoVoltimetros[5] = {0,0,0}; // pos0 selecciona el voltimetro pos1 selecciona voltimetros 0 y 1, pos2 selecciona voltimetros 2 y 3, dicen estado del voltimetro 5V (1) o 20V (0)
//int estadoVoltimetros[4] ={20,20,20,20} selecciona el estado del voltimetro 5V (5) o 20V (20)
int estadoFuentes[3] = {0,0,0}; //pos0 y 1 correponde al valor output pos2 es la fuente seleccionada
int estadoActualBotones[8];
int estadoAnteriorBotones[8] = {0,0,0,0,0,0,0,0};
//Pantalla
LiquidCrystal lcd(8, 9, 10, 11, 12, 13);


void setup ()
{
	//Entradas Analogicas
	pinMode(voltimetro0, INPUT);
	pinMode(voltimetro1, INPUT);
	pinMode(voltimetro2, INPUT);
	pinMode(voltimetro3, INPUT);
	pinMode(voltimetro4, INPUT);
	pinMode(voltimetro5, INPUT);
	//Multiplexor
  	pinMode(Y, INPUT);
  	pinMode(A, OUTPUT);
  	pinMode(B, OUTPUT);
  	pinMode(C, OUTPUT);
  	//Shift
  	pinMode(SerialCK, OUTPUT);
  	pinMode(ParallelCK, OUTPUT);
  	pinMode(SerialD, OUTPUT);
  	//Pantalla
  	lcd.begin(16,2);
  	lcd.print("hello, world!");
}

//////////////// Funciones Primarias ///////////////////

void muxHandler () //Maneja las entradas y escribe el vector de entradas
{
	int i;
	int ii;
	int iii;
	int c = 0;

	for (i=0; i<2; i++)
	{
		for (ii=0; ii<2; ii++)
		{
			for (iii=0; iii<2; iii++)
			{
				digitalWrite(C, i);
				digitalWrite(B, ii);
				digitalWrite(A, iii);
				muxRead[c] = digitalRead(Y);
				c++;
			}
		}
	}
}

void lecturaVoltimetros() //Recoje el valor en los voltimetros
{


	valorVoltimetros[0] = analogRead(voltimetro0);
	valorVoltimetros[1] = analogRead(voltimetro1);
	valorVoltimetros[2] = analogRead(voltimetro2);
	valorVoltimetros[3] = analogRead(voltimetro3);
	valorVoltimetros[4] = analogRead(voltimetro4);
	valorVoltimetros[5] = analogRead(voltimetro5);

	/*
	valorVoltimetros[0] = analogRead(voltimetro0)*(estadoVoltimetro[0]/1023);
	valorVoltimetros[1] = analogRead(voltimetro1)*(estadoVoltimetro[1]/1023);
	valorVoltimetros[2] = analogRead(voltimetro2)*(estadoVoltimetro[2]/1023);
	valorVoltimetros[3] = analogRead(voltimetro3)*(estadoVoltimetro[3]/1023);
	valorVoltimetros[4] = analogRead(voltimetro4);
	valorVoltimetros[5] = analogRead(voltimetro5);
	*/
}

void shiftHandler ()  //Maneja las salidas
{
	int i;
	
	for(i=7; i>-1; i--)
	{
		digitalWrite(SerialD, shiftWrite[i]);
		digitalWrite(SerialCK, HIGH);
		digitalWrite(SerialCK, LOW);
	}
	digitalWrite(ParallelCK, HIGH);
	digitalWrite(ParallelCK, LOW);
}

void outputSelect ()  //Escribe el verctor de salidas
{
	//El primer valor depende de los voltimetros seleccionaodos
	if (estadoVoltimetros[0] == 1)
		shiftWrite[0]=1;
	else 
		shiftWrite[0]=0;
	//El siguiente valor correponde a la fuente seleccinada
	if (estadoFuentes[2] == 0)
		shiftWrite[1]=1;
	else
		shiftWrite[1]=0;
	//Los siguietes valores gestionan los reles selectores del voltimetro
	if (estadoVoltimetros[1]==1)
		shiftWrite[2]=1;
	else
		shiftWrite[2]=0;
	if (estadoVoltimetros[2]==1)
		shiftWrite[3]=1;
	else
		shiftWrite[3]=0;
}

void flancoSubida () //Detector de falnco para IO
{
	int i;

	for(i=0; i<8; i++)
	{
		estadoActualBotones[i] = muxRead[i];

		if(estadoActualBotones[i]!=estadoAnteriorBotones[i]&&estadoAnteriorBotones[i]==0)
			estadoActualBotones[i] = 1;
		else
			estadoActualBotones[i] = 0;

		estadoAnteriorBotones[i] = muxRead[i];
	}
}

void actualizarEstado () //Gestion de la maquina de estados de control y valores de la fuente regulable
{
	if(estadoActualBotones[0] == 1)
		estadoFuentes[estadoFuentes[3]]+=20;//Ajuste grueso
	if(estadoActualBotones[1] == 1)
		estadoFuentes[estadoFuentes[3]]+=5;//Ajuste fino
	if(estadoActualBotones[2] == 1)
		estadoFuentes[estadoFuentes[3]]-=20;//Ajuste grueso
	if(estadoActualBotones[3] == 1)
		estadoFuentes[estadoFuentes[3]]-=5;//Ajuste fino
	if(estadoActualBotones[4] == 1)//Cambio de fuente
	{
		if (estadoFuentes[2] == 0)
			estadoFuentes[2] = 1;
		else
	    	estadoFuentes[2] = 0;
	}
	if(estadoActualBotones[5] == 1)
		{
			estadoVoltimetros[0]++;
			if (estadoVoltimetros[0] > 2)
				estadoVoltimetros[0] = 1;
		}
	//if(estadoActual[6] == 1)
		
	if(estadoActualBotones[7] == 1)
		{
			if (estadoVoltimetros[estadoVoltimetros[0]] == 0)
				estadoVoltimetros[estadoVoltimetros[0]] = 1;
			else
				estadoVoltimetros[estadoVoltimetros[0]] = 0;
		}
}

//////////////// Funciones de Pantalla ///////////////////

void actualizarPantalla () //Gestión de la pantalla
{
	lcd.print(" %.1f %.1f %.1f %.1f %.1f %.1f", valorVoltimetros[0], valorVoltimetros[1], estadoFuente[0], valorVoltimetros[2], valorVoltimetros[3], estadoFuente[1]);
	//Hay que multiplicar la fuente por la K correspondiente, pendiente de revisión!!!!
}

//////////////// Funciones de Debug ///////////////////

void buttonDebug ()
{
	muxHandler();
	int i;
	for (i=0; i<8; i++)
		shiftWrite[i] = muxRead[i];
	shiftHandler();
}

void loop ()
{
  buttonDebug();
}



















