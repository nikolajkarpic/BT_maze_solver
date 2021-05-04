unsigned int distance_fwd;
unsigned int distance_right;
unsigned int turning_threshold = 30;
unsigned int turning_time_left = 65;
unsigned int turning_time_right = 10;

unsigned int fwd = 0; //signal za kretanje napred
unsigned int bck = 0; //signal za kretanje nazad
unsigned int left = 0; //signal za skretanje levo
unsigned int right = 0;//signal za skretanje desno
unsigned int fwd_left = 0;
unsigned int fwd_right = 0;

unsigned int prefered_distance_fwd = 10;
unsigned int prefered_distance_right = 17;
unsigned int hysteresis_fwd = 5;
unsigned int hysteresis_right = 1;

enum tank_state{ idle_s, fwd_s, left_s, right_s, bck_s} current_state;
tank_state next_state;

unsigned int brojac_desno = 0, br;