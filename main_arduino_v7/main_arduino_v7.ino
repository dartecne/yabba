/*
Receives Serial instructions to control 3 Serial 8ch AC Dimmer
Manages:
 3 Dimmers 8ch
 Fog Machine
 Light 
 New 8ch dimmer board with new reconnections
*/

#include <SoftwareSerial.h>
#include <SPI.h>
#include <DmxSimple.h>


#define DIMMER_1_RX_PIN  12 // not really used
#define DIMMER_1_TX_PIN  6

#define DIMMER_2_RX_PIN  12  // not really used
#define DIMMER_2_TX_PIN  7

#define DIMMER_3_RX_PIN  12  // not really used
#define DIMMER_3_TX_PIN  8

#define LED_PIN	13

#define ACK	"ACK__"
#define INPUT_BUFFER_SIZE	6

SoftwareSerial Serial_1( DIMMER_1_RX_PIN, DIMMER_1_TX_PIN ); // RX, TX
SoftwareSerial Serial_2( DIMMER_2_RX_PIN, DIMMER_2_TX_PIN ); // RX, TX
SoftwareSerial Serial_3( DIMMER_3_RX_PIN, DIMMER_3_TX_PIN ); // RX, TX

//byte address = 0x00;
byte address = B10001;
int CS= 10;

char input_buffer[INPUT_BUFFER_SIZE];	// input instruction

void setup() {
  pinMode (CS, OUTPUT);
  SPI.begin();
	Serial.begin( 9600 );		// Port that receive computer instructions
	Serial_1.begin( 9600 );		// Port that sends Dimmer instructions
	Serial_1.write( 'S' ); // all off
	Serial_2.begin(9600);		// Port that sends Dimmer instructions
	Serial_2.write('S'); // all off
  Serial_3.begin(9600);   // Port that sends Dimmer instructions
  Serial_3.write('N'); // all on
  delay(1000);
  Serial_3.write('S'); // all off
						 //Serial_1.write( 'N' );	// all on
								// set the data rate for the SoftwareSerial port
	pinMode( LED_PIN, OUTPUT );
  DmxSimple.usePin(3);
 
	digitalWrite( LED_PIN, HIGH );
  unsigned char f = 100;
//  DmxSimple.write(1, f);
  delay(500);
  DmxSimple.write(1, 0);
  delay(500);
	while ( Serial.available() == 0 ); // wait ACK for begining loop
	Serial.readBytes(input_buffer, INPUT_BUFFER_SIZE); // waits until reading INPUT_BUFFER_SIZE bytes or timeout
	digitalWrite( LED_PIN, LOW );
	delay( 500 );
}

void loop() {
	digitalWrite(LED_PIN, HIGH);
	while (Serial.available() == 0 ); // wait data
	Serial.readBytes( input_buffer, INPUT_BUFFER_SIZE);
	/* echo
	for (int i = 0; i < INPUT_BUFFER_SIZE; i++) {
		Serial.print( input_buffer[i] );
	}*/
	if( input_buffer[0] == '1') {
    if( input_buffer[1] == 'E'  ) {
      input_buffer[1] = 'A'; // 1E goes to 3A
      write_data_from_buffer( Serial_3, input_buffer );
    } else if( input_buffer[1] == 'C'  ) {
      input_buffer[1] = 'E'; // 1C goes to 3E
      write_data_from_buffer( Serial_3, input_buffer );
    } else if( input_buffer[1] == 'F'  ) {
      input_buffer[1] = 'B'; // 1F goes to 3B
      write_data_from_buffer( Serial_3, input_buffer );
    } else if( input_buffer[1] == 'G'  ) {
      input_buffer[1] = 'C'; // 1G goes to 3D
      write_data_from_buffer( Serial_3, input_buffer );
    } else if( input_buffer[1] == 'H'  ) {
      input_buffer[1] = 'D'; // 1H goes to 3C
      write_data_from_buffer( Serial_3, input_buffer );
    } else
      write_data_from_buffer( Serial_1, input_buffer );
	}
	else if (input_buffer[0] == '2') {
    if( input_buffer[1] == 'F'  ) {
      input_buffer[1] = 'G'; // 2F goes to 3G
      write_data_from_buffer( Serial_3, input_buffer );
    } else if( input_buffer[1] == 'H'  ) {
      input_buffer[1] = 'H'; // 2H goes to 3H
      write_data_from_buffer( Serial_3, input_buffer );
    } else if( input_buffer[1] == 'G'  ) {
      input_buffer[1] = 'F'; // 2G goes to 3H
      write_data_from_buffer( Serial_3, input_buffer );
    } else
  	  write_data_from_buffer(Serial_2, input_buffer);
	}
  else if (input_buffer[0] == '3') {
    unsigned int fog = buffer2number( input_buffer );
    DmxSimple.write(1, fog);
    delay(10);
  }
	else if (input_buffer[0] == '4') {
    unsigned char light = buffer2number( input_buffer );
    light = map( light, 0, 100, 0, 256);
	  digitalPotWrite( light );
	}
  digitalWrite(LED_PIN, LOW);
}

int digitalPotWrite(int value) {
  digitalWrite(CS, LOW);
  SPI.transfer(address);
  SPI.transfer(value);
  digitalWrite(CS, HIGH);
}

void write_data_from_buffer( SoftwareSerial serial, const char buffer[INPUT_BUFFER_SIZE] ) {
	serial.write( buffer[1] );
	unsigned char dig[3];
	unsigned char temp;

	dig[0] = buffer[INPUT_BUFFER_SIZE - 4];
	dig[1] = buffer[INPUT_BUFFER_SIZE - 3];
	dig[2] = buffer[INPUT_BUFFER_SIZE - 2];

	for( int x = 0; x < 3; x++ ) {
		temp = dig[x] | 0x30;
		serial.write( temp );
	}
}
unsigned int buffer2number( const char buffer[INPUT_BUFFER_SIZE]  ) {
  unsigned char dig[3];
  unsigned int temp;

  dig[0] = buffer[INPUT_BUFFER_SIZE - 4] - 48; // 48 is '0'
  dig[1] = buffer[INPUT_BUFFER_SIZE - 3] - 48;
  dig[2] = buffer[INPUT_BUFFER_SIZE - 2] - 48;


  temp = 100 * dig[0] + 10 * dig[1] + dig[2];
  
  return temp;  
}

void write_data_from_int( unsigned int d ) {
	unsigned char dig1, dig2, dig3, dig[3];
	unsigned char x;
	unsigned char temp;
	temp = d;
	temp = temp / 10;
	dig1 = d % 10;
	dig2 = temp % 10;
	dig3 = temp / 10;
	dig[0] = dig3;
	dig[1] = dig2;
	dig[2] = dig1;
	for (x = 0; x < 3; x++) {
		temp = dig[x] | 0x30;
		Serial_1.write(temp);
	}
}
