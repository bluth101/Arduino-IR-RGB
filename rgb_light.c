#include <IRremote.h>

#define PIN_RED 10
#define PIN_GREEN 8
#define PIN_BLUE 9
#define PIN_IRRCV 7
#define TINT_SHADE_FACTOR 0.25

int POWER;
int FADE;
IRrecv irrecv(PIN_IRRCV);

float RED = 255;
float GREEN = 255;
float BLUE = 255;

void rgb_light(int _r, int _g, int _b) {
	analogWrite(PIN_RED, _r);
	analogWrite(PIN_GREEN, _g);
	analogWrite(PIN_BLUE, _b);
}
void set_power(int _state) {
    rgb_light(RED*_state, GREEN*_state, BLUE*_state);
}
void shade() {
  	RED = RED * TINT_SHADE_FACTOR;
  	GREEN = GREEN * TINT_SHADE_FACTOR;
  	BLUE = BLUE * TINT_SHADE_FACTOR;
}
void tint() {
    RED = RED + (255 + RED) * TINT_SHADE_FACTOR;
    GREEN = GREEN + (255 + GREEN) * TINT_SHADE_FACTOR;
	BLUE = BLUE + (255 + BLUE) * TINT_SHADE_FACTOR;
}

void setup() {
	pinMode(PIN_RED, OUTPUT);
  	pinMode(PIN_GREEN, OUTPUT);
  	pinMode(PIN_BLUE, OUTPUT);
  
  	Serial.begin(9600);
  	irrecv.enableIRIn();
  	irrecv.blink13(true);
  
  	set_power(POWER = false);
}

void loop() {
  	if( FADE > 0 && POWER ) {
      	switch(FADE) {
        	case 1:
          		RED += 1;
          		if( RED >= 255 ) {
                	FADE = 2;
          			RED = 0;
                }
          		break;
         	case 2:
          		GREEN += 1;
          		if( GREEN >= 255 ) {
                	FADE = 3;
                  	GREEN = 0;
        		}		
          		break;
         	case 3:
          		BLUE += 1;
          		if( BLUE >= 255 ) {
            		FADE = 1;
            		BLUE = 0;
          		}
      	}
    	rgb_light(RED, GREEN, BLUE);
    }
  
	decode_results results;
  	if( irrecv.decode(&results) ) {
      	// Handle button press
      	switch(results.value) {
          	// OTHER BTNS
          	case 0xFD50AF: // Up
          		tint();
          		break;
          	case 0xFD10EF: // Down
          		shade();
          		break;
          	case 0xFDB04F: // Eq
          		if( FADE > 0 )
                	FADE = 0;
          		else
                  	FADE = 1;
          		break;
          
          	// COL 1
        	case 0xFD00FF: // Power
          		set_power(POWER = !POWER);
          		break;
         	case 0xFD30CF: // 0, White
          		RED = 255; GREEN = 255; BLUE = 255;
          		break;
          	case 0xFD08F7: // 1, Red
          		RED = 255; GREEN = 0; BLUE = 0;
          		break;
          	case 0xFD28D7: // 4, Pink
          		RED = 255; GREEN = 0; BLUE = 255;
          		break;
          	case 0xFD18E7: // 7, Purple
          		RED = 152; GREEN = 69; BLUE = 170;
          		break;
          
          	// COL 2
          	case 0xFD8877: // 2, Green
          		RED = 0; GREEN = 255; BLUE = 0;
          		break;
         	case 0xFDA857: // 5, Orange
          		RED = 200; GREEN = 100; BLUE = 0;
          		break;
          	case 0xFD9867: // 8, Yellow
          		RED = 255; GREEN = 255; BLUE = 0;
          		break;
          
          	// COL 3
          	case 0xFD48B7: // 3, Blue
          		RED = 0; GREEN = 0; BLUE = 255;
          		break;
          	case 0xFD6897: // 6, Light Blue
          		RED = 12; GREEN = 93; BLUE = 219;
          		break;
          	case 0xFD58A7: // 9, Cyan
          		RED = 0; GREEN = 255; BLUE = 255;
          		break;
      	}
      	// If power is on, update LED
      	if( POWER )
      		rgb_light(RED, GREEN, BLUE);
      
    	Serial.println(results.value, HEX);
    	irrecv.resume();
  	}
}
