#pragma config(Sensor, dgtl1,  cout1,          sensorDigitalOut)
#pragma config(Sensor, dgtl2,  cout2,          sensorDigitalOut)
#pragma config(Sensor, dgtl3,  cout3,          sensorDigitalOut)
#pragma config(Sensor, dgtl4,  cin1,           sensorDigitalIn)
#pragma config(Sensor, dgtl5,  ledF1,           sensorLEDtoVCC)
#pragma config(Sensor, dgtl6,  ledF2,           sensorLEDtoVCC)
#pragma config(Sensor, dgtl7,  ledF3,           sensorLEDtoVCC)
#pragma config(Sensor, dgtl8,  ledF4,           sensorLEDtoVCC)
#pragma config(Sensor, dgtl9,  ledF5,           sensorLEDtoVCC)
#pragma config(Sensor, dgtl10, ledF6,           sensorLEDtoVCC)
#pragma config(Sensor, dgtl11, ledF7,           sensorLEDtoVCC)
#pragma config(Sensor, dgtl12, ledF8,           sensorLEDtoVCC)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#define NUMBER_OF_FLOORS 8
#define IDLE_FLOOR_TIME 4000
#define LED_MOVING_DURATION 700
#define setLED(x, y) do { leds[x].state = y; } while(0);

typedef struct
{
	int sensorAddress;
	int state;
	int startTime;
} led;

typedef struct
{
	int state;
} button;

led leds[NUMBER_OF_FLOORS];
button buttons[NUMBER_OF_FLOORS];

task ledStateTask()
{
	while(true)
	{
		bool noneWaiting = true;
		for(int i = 0; i < NUMBER_OF_FLOORS; i++)
		{
			//Switch Case
		  //0: Off
		  //1: On
		  //2+3: Blink at Normal Move Speed
			switch(leds[i].state)
			{
				case 0:
					SensorValue[leds[i].sensorAddress] = 0;
					leds[i].startTime = -1;
					break;
				case 1:
					SensorValue[leds[i].sensorAddress] = 1;
					leds[i].startTime = -1;
					break;
				case 2: //Turn on LED
				  leds[i].state = 3;
					leds[i].startTime = time1[T1];
					SensorValue[leds[i].sensorAddress] = 1;
			    break;
			  case 3: //Continue blink loop
			    if ((time1[T1] - leds[i].startTime) >= LED_MOVING_DURATION)
			    {
			     	SensorValue[leds[i].sensorAddress] = !SensorValue[leds[i].sensorAddress];
			   		leds[i].startTime = time1[T1];
			   	}
			   	break;
			}
			if(leds[i].startTime != -1)
				noneWaiting = false;
		}
		if(noneWaiting)
			clearTimer(T1);
	}
}

void requestButtonState(int index)
{
	switch(index)
	{
		case 0:
			SensorValue[cout1] = 0;
			SensorValue[cout2] = 0;
			SensorValue[cout3] = 0;
			break;
		case 1:
			SensorValue[cout1] = 0;
			SensorValue[cout2] = 0;
			SensorValue[cout3] = 1;
			break;
		case 2:
			SensorValue[cout1] = 0;
			SensorValue[cout2] = 1;
			SensorValue[cout3] = 0;
			break;
		case 3:
			SensorValue[cout1] = 0;
			SensorValue[cout2] = 1;
			SensorValue[cout3] = 1;
			break;
		case 4:
			SensorValue[cout1] = 1;
			SensorValue[cout2] = 0;
			SensorValue[cout3] = 0;
			break;
		case 5:
			SensorValue[cout1] = 1;
			SensorValue[cout2] = 0;
			SensorValue[cout3] = 1;
			break;
		case 6:
			SensorValue[cout1] = 1;
			SensorValue[cout2] = 1;
			SensorValue[cout3] = 0;
			break;
		case 7:
			SensorValue[cout1] = 1;
			SensorValue[cout2] = 1;
			SensorValue[cout3] = 1;
			break;
	}
}

task buttonTask()
{
	while(true)
	{
		for(int i=0; i < NUMBER_OF_FLOORS; i++)
	  {
		  requestButtonState(i);
		  wait1Msec(5);
	    buttons[i].state = SensorValue[cin1];
	  }
	}
}

task main()
{
	//Initialize led addresses
	leds[0].sensorAddress = ledF1;
	leds[1].sensorAddress = ledF2;
	leds[2].sensorAddress = ledF3;
	leds[3].sensorAddress = ledF4;
	leds[4].sensorAddress = ledF5;
	leds[5].sensorAddress = ledF6;
	leds[6].sensorAddress = ledF7;
	leds[7].sensorAddress = ledF8;


	startTask(ledStateTask);
	startTask(buttonTask);


	//Access and set led states using leds[i].state
	//0: Off, 1: On, 2: Blink
	//Access button states using buttons[i].state
	//0: Not set, 1: Set


	//Main Loop:
	//Check for any floor requests


	button *b;
	led *ld;
	int pendingRequests[NUMBER_OF_FLOORS];
	//Since we have a floor '0' we set the non-requested value to -1
	for(int i = 0; i < NUMBER_OF_FLOORS; i++)
		pendingRequests[i] = -1;


	int nextOpenRequest = 0;
	while(true)
	{
		for(int i = 0; i < NUMBER_OF_FLOORS; i++)
		{
			b = &buttons[i];
			ld = &leds[i];

			if(b -> state)
			{

				//Check if the floor already exists in the requested array
				int requestedFloor = i;
				bool floorAlreadyRequested = false;
				for(int j = 0; j < NUMBER_OF_FLOORS; j++)
			  {
					if(pendingRequests[j] == i)
					{
						floorAlreadyRequested = true;
					}
				}

				//Add the floor if it is already not requested
				if(!floorAlreadyRequested)
				{
					pendingRequests[nextOpenRequest] = requestedFloor;
				}

			}


		}

		//After checking all the buttons fullfill the next request
		//there will be requests up until the next open request
		//if no new requests were added in the previous button loop
		//then the next open request index would be 0
		for(int i = 0; i < nextOpenRequest; i++)
		{
			int requestedFloor = pendingRequests[i];
			//Go to the floor

			//Consider the request fullfilled and wait a predefined length
			//then move onto the next floor request
			pendingRequests[i] = -1;
			wait1Msec(IDLE_FLOOR_TIME);


		}
		//After the loop all requests should be fullfilled, so we should reset the index
		nextOpenRequest = 0;

	}

}
