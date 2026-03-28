#include <naraapi.h>


// The main Sayonara window
void sn_window();


/* Everything in the Color Config section */

// The main color config interface function
void color_config(Nara::Sayo *sayo);

// The color picker widget
void color_picker(Nara::Color *in_color);

// The led picker element
// NOTE: I feel like the signature is crowded and unorganized.
void led_picker(Nara::Sayo *sayo, Nara::Color *color, int buttons, int *key, int fn);

// The led mode switcher element
Nara::LED_Modes led_mode_switcher(Nara::Sayo *sayo, int key, int fn);

// The color mode switcher element
Nara::LED_ColorModes color_mode_switcher(Nara::Sayo *sayo, int key, int fn);

// The color table switcher element
int color_table_switcher(Nara::Sayo *sayo, int key, int fn);

// The trigger event switcher element 
int trigger_event_switcher(Nara::Sayo *sayo, int key, int fn);


/* Miscellaneous Windows and Elements*/

// Shows when there are no devices
void device_not_found_window();

// Shows information about the application
void about_window(bool *p_open);