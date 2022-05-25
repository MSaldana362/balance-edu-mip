/**
 * @file balance_edu_mip.c
 *
 * This is meant to be a skeleton program for Robot Control projects. Change
 * this description and file name before modifying for your own purpose.
 */

#include <stdio.h>
#include <robotcontrol.h> // includes ALL Robot Control subsystems

// define motor and encoder constants
#define ENCODER_CHANNEL_L 3
#define ENCODER_CHANNEL_R 2
#define MOTOR_CHANNEL_L 3
#define MOTOR_CHANNEL_R 2
#define ENCODER_POLARITY_L 1
#define ENCODER_POLARITY_R -1
#define MOTOR_POLARITY_L 1
#define MOTOR_POLARITY_R -1
#define ENCODER_SLOTS 15
#define GEAR_RATIO 35.577
#define COUNTS_PER_SLOT 4
// define mpu constants
#define I2C_BUS 2
#define GPIO_INT_PIN_CHIP 3
#define GPIO_INT_PIN_PIN  21
// define D1 sample rate
#define SAMPLE_RATE_HZ 100

// function declarations
void on_pause_press();
void on_pause_release();
static void __balance_controller(void);		// mpu interrupt routine
double calculate_theta_accl(double,double);
double calculate_theta_gyro(double,double,double);

// global variables
rc_mpu_data_t mpu_data;


/**
 * This template contains these critical components
 * - ensure no existing instances are running and make new PID file
 * - start the signal handler
 * - initialize subsystems you wish to use
 * - while loop that checks for EXITING condition
 * - cleanup subsystems at the end
 *
 * @return     0 during normal operation, -1 on error
 */

int main()
{
	// make sure another instance isn't running
	// if return value is -3 then a background process is running with
	// higher privaledges and we couldn't kill it, in which case we should
	// not continue or there may be hardware conflicts. If it returned -4
	// then there was an invalid argument that needs to be fixed.
	if (rc_kill_existing_process(2.0) < -2) {
		return -1;
	}

	// start signal handler so we can exit cleanly
	if (rc_enable_signal_handler() == -1) {
		fprintf(stderr, "ERROR: failed to start signal handler\n");
		return -1;
	}

	// initialize pause button
	if (rc_button_init(RC_BTN_PIN_PAUSE, RC_BTN_POLARITY_NORM_HIGH, RC_BTN_DEBOUNCE_DEFAULT_US)) {
		fprintf(stderr, "ERROR: failed to initialize pause button\n");
		return -1;
	}

	// initialize motors

	// initialize adc

	// make PID file to indicate your project is running
	// due to the check made on the call to rc_kill_existing_process() above
	// we can be fairly confident there is no PID file already and we can
	// make our own safely.
	rc_make_pid_file();


	printf("\nPress and release pause button to turn green LED on and off\n");
	printf("hold pause button down for 2 seconds to exit\n");

	// set up mpu configuration
	rc_mpu_config_t mpu_config = rc_mpu_default_config();
	mpu_config.dmp_sample_rate = SAMPLE_RATE_HZ;
	mpu_config.i2c_bus = I2C_BUS;
    mpu_config.gpio_interrupt_pin_chip = GPIO_INT_PIN_CHIP;
    mpu_config.gpio_interrupt_pin = GPIO_INT_PIN_PIN;

	// set up D1 THETA controller

	// set up D2 PHI controller

	// start mpu
	if(rc_mpu_initialize_dmp(&mpu_data, mpu_config)){
                fprintf(stderr,"ERROR: can't talk to IMU, all hope is lost\n");
                rc_led_blink(RC_LED_RED, 5, 5);
                return -1;
	}

	// set mpu interrupt routine
	rc_mpu_set_dmp_callback(&__balance_controller);

	// Keep looping until state changes to EXITING
	rc_set_state(RUNNING);
	while(rc_get_state() != EXITING) {

		// do things based on the state
		if (rc_get_state() == RUNNING) {
			rc_led_set(RC_LED_GREEN, 1);
			rc_led_set(RC_LED_RED, 0);
		} else {
			rc_led_set(RC_LED_GREEN, 0);
			rc_led_set(RC_LED_RED, 1);
		}

		// always sleep at some point
		rc_usleep(100000);
	}

	// turn off LEDs and close file descriptors
	rc_led_set(RC_LED_GREEN, 0);
	rc_led_set(RC_LED_RED, 0);
	rc_led_cleanup();
	rc_button_cleanup();	// stop button handlers
	rc_remove_pid_file();	// remove pid file LAST

	return 0;
}


/**
 * Make the Pause button toggle between paused and running states.
 */
void on_pause_release()
{
	if (rc_get_state() == RUNNING) {
		rc_set_state(PAUSED);
	} else if (rc_get_state() == PAUSED) {
		rc_set_state(RUNNING);
	}

	return;
}


/**
* If the user holds the pause button for 2 seconds, set state to EXITING which
* triggers the rest of the program to exit cleanly.
**/
void on_pause_press()
{
	int i;
	const int samples = 100; // check for release 100 times in this period
	const int us_wait = 2000000; // 2 seconds

	// now keep checking to see if the button is still held down
	for(i = 0; i < samples; i++) {
		rc_usleep(us_wait/samples);
		if (rc_button_get_state(RC_BTN_PIN_PAUSE) == RC_BTN_STATE_RELEASED) {
			return;
		}
	}

	printf("long press detected, shutting down\n");
	rc_set_state(EXITING);

	return;
}


static void __balance_controller(void) {
	// balance controller

	// read sensor data from mpu
	rc_mpu_read_accel(&mpu_data);
	rc_mpu_read_gyro(&mpu_data);

	// assign sensor data
	double z_accl = mpu_data.accel[2];
	double y_accl = mpu_data.accel[1];
	double x_gyro = mpu_data.gyro[0] * DEG_TO_RAD;

	// print mpu values
	printf("%f\t%f\t%f\n",z_accl,y_accl,x_gyro);

	// get and print angle values
	double theta_accl = calculate_theta_accl(z_accl,y_accl);
	double theta_gyro = calculate_theta_gyro(x_gyro,0,SAMPLE_RATE_HZ);
	printf("%f\t%f\n",theta_accl,theta_gyro);

}


double calculate_theta_accl(double z, double y) {
	// calculate raw theta value from accel

	return atan2(-z,y);
}


double calculate_theta_gyro(double x, double past_theta, double fs) {
	// calculate raw theta value from gyro

	// time step
	double dt = 1/fs;

	return past_theta + x * dt;
}