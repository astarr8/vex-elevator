#pragma config(Sensor, dgtl1,  cin1,           sensorDigitalIn)
#pragma config(Sensor, dgtl2,  cin2,           sensorDigitalIn)
#pragma config(Sensor, dgtl3,  cin3,           sensorDigitalIn)
#pragma config(Sensor, dgtl4,  cout1,          sensorDigitalOut)
#pragma config(Sensor, dgtl5,  btn0,           sensorTouch)
#pragma config(Sensor, dgtl6,  btn1,           sensorTouch)
#pragma config(Sensor, dgtl7,  btn2,           sensorTouch)
#pragma config(Sensor, dgtl8,  btn3,           sensorTouch)
#pragma config(Sensor, dgtl9,  btn4,           sensorTouch)
#pragma config(Sensor, dgtl10, btn5,           sensorTouch)
#pragma config(Sensor, dgtl11, btn6,           sensorTouch)
#pragma config(Sensor, dgtl12, btn7,           sensorTouch)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

int buttons[] = {btn0,btn1,btn2,btn3,btn4,btn5,btn6,btn7};

int requestedSensor()
{
	//Convert input value to binary
	return SensorValue[cin1] * 4 + SensorValue[cin2] * 2 + SensorValue[cin3];
}

task main()
{
	while(true)
	{
			int requestedValue = requestedSensor();
			//Output the request button to the out pin
			SensorValue[cout1] = SensorValue[buttons[requestedValue]];
	}

}
