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
void led_mode_switcher(Nara::Sayo *sayo, int key, int fn);

// The color mode switcher element
void color_mode_switcher(Nara::Sayo *sayo, int key, int fn);


/* Miscellaneous Windows and Elements*/

// Shows when there are no devices
void device_not_found_window();

// Shows information about the application
void about_window(bool *p_open);