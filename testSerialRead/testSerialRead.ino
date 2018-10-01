/*
Receives Serial instructions to control a Serial 8ch AC Dimmer
*/

#include <SoftwareSerial.h>

#define DIMMER_1_RX_PIN  10
#define DIMMER_1_TX_PIN  11

#define DIMMER_2_RX_PIN  6
#define DIMMER_2_TX_PIN  7

#define DIMMER_3_RX_PIN  3
#define DIMMER_3_TX_PIN  4

#define LED_PIN	13

#define ACK	"ACK__"
#define INPUT_BUFFER_SIZE	6

SoftwareSerial Serial_1( DIMMER_1_RX_PIN, DIMMER_1_TX_PIN ); // RX, TX
SoftwareSerial Serial_2( DIMMER_2_RX_PIN, DIMMER_2_TX_PIN ); // RX, TX
SoftwareSerial Serial_3( DIMMER_3_RX_PIN, DIMMER_3_TX_PIN ); // RX, TX

char input_buffer[INPUT_BUFFER_SIZE];	// input instruction

void setup() {
	Serial.begin( 9600 );		// Port that receive computer instructions
	Serial_1.begin( 9600 );		// Port that sends Dimmer instructions
	Serial_1.write( 'S' ); // all off
	Serial_2.begin(9600);		// Port that sends Dimmer instructions
	Serial_2.write('S'); // all off
	Serial_3.begin(9600);		// Port that sends Dimmer instructions
	Serial_3.write('S'); // all off
						 //Serial_1.write( 'N' );	// all on
								// set the data rate for the SoftwareSerial port
	pinMode( LED_PIN, OUTPUT );
	digitalWrite( LED_PIN, HIGH );

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
	digitalWrite(LED_PIN, LOW);
	if( input_buffer[0] == '1') write_data_from_buffer( Serial_1, input_buffer );
	if (input_buffer[0] == '2') write_data_from_buffer(Serial_2, input_buffer);
	if (input_buffer[0] == '3') write_data_from_buffer(Serial_3, input_buffer);
}

void write_data_from_buffer( SoftwareSerial &serial, const char buffer[INPUT_BUFFER_SIZE] ) {
	serial.write( buffer[1] );
	unsigned char dig[3];
	unsigned char temp;

	dig[0] = buffer[INPUT_BUFFER_SIZE - 4];
	dig[1] = buffer[INPUT_BUFFER_SIZE - 3];
	dig[2] = buffer[INPUT_BUFFER_SIZE - 2];
	//temp = d / 10;
	//dig1 = d % 10;	// uniddes
	//dig2 = temp % 10;	// decenas
	//dig3 = temp / 10;	// centenas
	//dig[0] = dig3;
	//dig[1] = dig2;
	//dig[2] = dig1;

	for( int x = 0; x < 3; x++ ) {
		temp = dig[x] | 0x30;
		serial.write( temp );
	}
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
