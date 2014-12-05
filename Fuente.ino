//Multiplexor
int Y = 4;
int A = 7;
int B = 8;
int C = 9;
int muxRead[8];
//Shift
int SerialCK=3;
int ParallelCK=2;
int SerialD=1;
int shiftWrite[8]={1,0,1,0,1,0,1,0};
//Botones
int estadoActualBotones[8];
int estadoAnteriorBotones[8]={0,0,0,0,0,0,0,0};
//Fuentes y voltimetros
int voltimetro0=A0;
int voltimetro1=A1;
int voltimetro2=A2;
int voltimetro3=A3;
int estadoVoltimetros[6];
int estadoFuentes[3]={1,0,0};//El primer valor va a decir si está seleccionada la fuente 1 o la 2, si lo ves muy lioso lo cambiamos



void setup ()
{
	//Multiplexor
  	pinMode(Y, INPUT);
  	pinMode(A, OUTPUT);
  	pinMode(B, OUTPUT);
  	pinMode(C, OUTPUT);
  	//Shift
  	pinMode(SerialCK1, OUTPUT);
  	pinMode(ParallelCK1, OUTPUT);
  	pinMode(SerialD1, OUTPUT);
}

void muxHandler ()
{
	int i;
	int ii;
	int iii;
	int c;

	for (i=0; i<2; i++)
	{
		for (ii=0; ii<2; ii++)
		{
			for (iii=0; iii<2; iii++)//creo que aqui tenias un error porque todos los contadores se llamaban i, cambiado
			{
				digitalWrite(C, i);
				digitalWrite(B, ii);
				digitalWrite(A, iii);
				muxRead[c] = digitalRead(Y);
				estadoActualBotones[c] = muxRead[c];//esto es lo que no sé si he entendido bien que tu dices que hacen falta dos vectores...
				c++;
			}
		}
	}
}

void shiftHandler ()
{
	int i;
	
	for(i=7; i>-1; i--){
		digitalWrite(SI, shiftWrite[i]);
		digitalWrite(SerialCK, HIGH);
		digitalWrite(SerialCK, LOW);
	}
	digitalWrite(ParallelCK, HIGH);
	digitalWrite(ParallelCK, LOW);
}

void flancoSubida()
{
	int i;

	for(i=0; i<8; i++)
	{
		if(estadoActualBotones[i]!=estadoAnteriorBotones[i]&&estadoAnteriorBotones==0)
		{
			actualizarEstado(estadoActualBotones);
		}
	}
	
}

void actualizarEstado()
{

	if(estadoActualBotones[1] == 1)
		estadoFuentes[estadoFuentes[0]]+=20;//Ajuste grueso
	if(estadoActualBotones[2] == 1)
		estadoFuentes[estadoFuentes[0]]+=5;//Ajuste fino
	if(estadoActualBotones[3] == 1)
		estadoFuentes[estadoFuentes[0]]-=20;//Ajuste grueso
	if(estadoActualBotones[4] == 1)
		estadoFuentes[estadoFuentes[0]]-=5;//Ajuste fino
	if(estadoActualBotones[5] == 1&&estadoFuentes[0]==2)//Cambiar
		estadoFuentes[0]=1;
	else if(estadoActualBotones[5] == 1&&estadoFuentes[0]==1)
	    estadoFuentes[0]=2;
	/*

	No me acuerdo del resto de funciones:

		if(estadoActual[6] == 1)
		
		if(estadoActual[7] == 1)
		
		if(estadoActual[8] == 1)
		
	*/

}
void lecturaVoltimetros(){

	estadoVoltimetros[0] = analogRead(voltimetro0);
	estadoVoltimetros[1] = analogRead(voltimetro1);
	estadoVoltimetros[2] = analogRead(voltimetro2);
	estadoVoltimetros[3] = analogRead(voltimetro3);
	/*
	Retroalimentación
	
	estadoVoltimetros[4] = analogRead(pin);
	estadoVoltimetros[5] = analogRead(pin);
	
	*/
}

void loop ()
{
  
}
