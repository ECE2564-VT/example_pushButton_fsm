/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>


// According to the schematics on page 37 of the Launchpad user guide,
// When a button is pressed, it is grounded (logic 0)
#define PRESSED 0

// This function initializes all the peripherals
void initialize();

void TurnOn_Launchpad_LED1();
void TurnOff_Launchpad_LED1();
void Toggle_Launchpad_LED1();
void Toggle_BoosterPack_BLED();

// This function returns true if the button1 is pressed
bool Launchpad_S1_pressed();

// This is the raw status of the button (0 or 1)
char SwitchStatus_Launchpad_S1();

// This function returns true if the Launchpad S1 has been pushed
bool Launchpad_S1_pushed(bool);

// This function returns true if the button1 is pressed
bool BoosterPack_S2_pressed();

// This is the raw status of the button (0 or 1)
char SwitchStatus_BoosterPack_S2();

bool BoosterPack_S2_pushed(bool);

// The type that holds the states of the button for its FSM
typedef enum {PUSHED, RELEASED} button_state_t;



int main(void)
{
    initialize();

    int LauncpadPushCount = 0;
    int BoosterPackPushCount = 0;
    bool LauunchpadButtonPressed, LauunchpadButtonPushed;


    while (true)
    {

        // Sometimes for debugging purposes, it is a good idea to do things step by step such as the below example for the Launchpad

        // read the input
        LauunchpadButtonPressed = Launchpad_S1_pressed();

        // update the FSM
        LauunchpadButtonPushed = Launchpad_S1_pushed(LauunchpadButtonPressed);

        if(LauunchpadButtonPushed) {
            LauncpadPushCount++;
            Toggle_Launchpad_LED1();
        }

        // This is similar to what we did for the Launchpad, but the code is more compact.
        if(BoosterPack_S2_pushed(BoosterPack_S2_pressed())) {
            BoosterPackPushCount++;
            Toggle_BoosterPack_BLED();
        }

    }
}

// Initialization part is always device dependent and therefore does not change much with HAL
void initialize()
{

    WDT_A_hold(WDT_A_BASE);

    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);

    GPIO_setAsInputPinWithPullUpResistor (GPIO_PORT_P1, GPIO_PIN1);

    GPIO_setAsInputPin (GPIO_PORT_P3, GPIO_PIN5);

    GPIO_setAsOutputPin(GPIO_PORT_P5, GPIO_PIN6);
    GPIO_setOutputLowOnPin(GPIO_PORT_P5, GPIO_PIN6);

}


void TurnOn_Launchpad_LED1()
{
    GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);
}
void TurnOff_Launchpad_LED1()
{
    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
}
void Toggle_Launchpad_LED1()
{
    GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);
}

void Toggle_BoosterPack_BLED()
{
    GPIO_toggleOutputOnPin(GPIO_PORT_P5, GPIO_PIN6);
}

char SwitchStatus_Launchpad_S1()
{
    return (GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN1));
}

bool Launchpad_S1_pressed()
{
    return (GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN1) == PRESSED);
}

char SwitchStatus_BoosterPack_S2()
{
    return (GPIO_getInputPinValue(GPIO_PORT_P3, GPIO_PIN5));
}

bool BoosterPack_S2_pressed()
{
    return (GPIO_getInputPinValue(GPIO_PORT_P3, GPIO_PIN5) == PRESSED);
}

bool Launchpad_S1_pushed(bool ButtonPressed)
{
    // The state of button1
    static button_state_t button_state = RELEASED;

    // The output of the FSM
    // To make the function brief and more efficient, we can assign a default value to the output
    bool take_action = false;

    switch(button_state)
    {
    // make sure you have one "case" for each state.
    // make sure there is a break at the end of each case before the next case
    case PUSHED:
        // for each state, make sure you implement each transition arc

        // transition arc from PUSHED to RELEASED
        if (!ButtonPressed)
        {
            // assign the output
            // this is not needed anymore: take_action = false;

            // next state
            button_state = RELEASED;
         }
        // The arc from PUSHED to PUSHED does not need implementation since the state does not change AND its output is also defaul

        break;


    case RELEASED:

        // transition arc from RELEASED to PUSHED
        if (ButtonPressed)
        {
            //output of the arc
            take_action = true;

            // next state
            button_state = PUSHED;
        }
        // arc from RELEASED to RELEASED does not need implemenation for a similar reason to line 172

     }

    return(take_action);

}

bool BoosterPack_S2_pushed(bool ButtonPressed)
{
    // The state of button1
    static button_state_t button_state = RELEASED;

    // The output of the FSM
    // To make the function brief and more efficient, we can assign a default value to the output
    bool take_action = false;

    switch(button_state)
    {
    // make sure you have one "case" for each state.
    // make sure there is a break at the end of each case before the next case
    case PUSHED:
        // for each state, make sure you implement each transition arc

        // transition arc from PUSHED to RELEASED
        if (!ButtonPressed)
        {
            // assign the output
            // this is not needed since it is the default: take_action = false;

            // next state
            button_state = RELEASED;
         }
        // The arc from PUSHED to PUSHED does not need implementation since the state does not change AND its output is also defaul

        break;


    case RELEASED:

        // transition arc from RELEASED to PUSHED
        if (ButtonPressed)
        {
            //output of the arc
            take_action = true;

            // next state
            button_state = PUSHED;
        }
        // arc from RELEASED to RELEASED does not need implemenation for a similar reason to line 172

     }

    return(take_action);

}
