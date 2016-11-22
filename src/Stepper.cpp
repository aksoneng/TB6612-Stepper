#include "Stepper.h"

/* Motor()
address:
	Shield I2C address
freq:
	PWM's frequency
*/
Stepper::Motor(uint8_t address, uint8_t motor, uint32_t freq)
{
	_use_STBY_IO=false;

	if(motor==_MOTOR_A)
		_motor=_MOTOR_A;
	else
		_motor=_MOTOR_B;

	Wire.begin();

	_address=address;

	setfreq(freq);

}


Stepper::Motor(uint8_t address, uint8_t motor, uint32_t freq, uint8_t STBY_IO)
{



	_use_STBY_IO=true;
	_STBY_IO=STBY_IO;

	if(motor==_MOTOR_A)
		_motor=_MOTOR_A;
	else
		_motor=_MOTOR_B;

	Wire.begin();

	_address=address;

	setfreq(freq);

	pinMode(_STBY_IO,OUTPUT);
	digitalWrite(_STBY_IO,LOW);
}


/* setfreq() -- set PWM's frequency
freq:
	PWM's frequency
*/
void Stepper::setfreq(uint32_t freq)
{
	Wire.beginTransmission(_address);
	Wire.write(((byte)(freq >> 16)) & (byte)0x0f);
	Wire.write((byte)(freq >> 16));
	Wire.write((byte)(freq >> 8));
	Wire.write((byte)freq);
	Wire.endTransmission();     // stop transmitting
	delay(100);
}

/* setmotor() -- set motor
motor:
	_MOTOR_A	0	Motor A
	_MOTOR_B	1	Motor B
dir:
	_SHORT_BRAKE	0
	_CCW			1
	_CCW			2
	_STOP			3
	_STANDBY		4
pwm_val:
	0.00 - 100.00  (%)
*/
void Stepper::setmotor(uint8_t dir, float pwm_val)
{
	uint16_t _pwm_val;

	if(_use_STBY_IO==true){

		if(dir==_STANDBY)
		{
			digitalWrite(_STBY_IO,LOW);
			return;
		}
		else
			digitalWrite(_STBY_IO,HIGH);
	}

	Wire.beginTransmission(_address);
	Wire.write(_motor | (byte)0x10);
	Wire.write(dir);

	_pwm_val=uint16_t(pwm_val*100);

	if(_pwm_val>10000)
		_pwm_val=10000;

	Wire.write((byte)(_pwm_val >> 8));
	Wire.write((byte)_pwm_val);
	Wire.endTransmission();     // stop transmitting


	delay(100);
}

void Stepper::setmotor(uint8_t dir)
{
	setmotor(dir,100);
}

void Stepper:step(int steps)
{
    if(steps >0)
    {
        for (int i =0; i<steps; i++)
        {
            // step 1
            setmotor(M_A, CW, _PWM); setmotor(M_B, STOP, _PWM);
            delay(_delay);
            // step 2
            setmotor(M_B, CW, _PWM); setmotor(M_A, STOP, _PWM);
            delay(_delay);
            // step 3
            setmotor(M_A, CCW, _PWM); setmotor(M_B, STOP, _PWM);
            delay(_delay);
            // step 4
            setmotor(M_B, CCW, _PWM); setmotor(M_A, STOP, _PWM);
            delay(_delay);
        }
        setmotor(M_A, STOP, 0); setmotor(M_B, STOP, 0);
        delay(_delay);
    }
    else if(steps <0)
    {
        for (int i =0; i<(-steps); i++)
        {
            // step 1
            setmotor(M_A, CW, _PWM); setmotor(M_B, STOP, _PWM);
            delay(_delay);
            // step 2
            setmotor(M_B, CCW, _PWM); setmotor(M_A, STOP, _PWM);
            delay(_delay);
            // step 3
            setmotor(M_A, CCW, _PWM); setmotor(M_B, STOP, _PWM);
            delay(_delay);
            // step 4
            setmotor(M_B, CW, _PWM); setmotor(M_A, STOP, _PWM);
            delay(_delay);
        }
        setmotor(M_A, STOP, 0); setmotor(M_B, STOP, 0);
        delay(_delay);
    }
}

void Stepper::setdelay(uint delay)
{
    _delay =delay;
}
