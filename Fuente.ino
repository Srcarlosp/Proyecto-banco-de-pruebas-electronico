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
		for (i=0; i<2; i++)
		{
			for (i=0; i<2; i++)
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

void loop ()
{
  
}