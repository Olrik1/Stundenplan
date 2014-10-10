#include <pebble.h>

//144*168
  
const int times[] = {0,460,505,552,572,617,664,679,724,770,815,860,905,920,965,1010,1440};
const char plan[7][17][25] = {{"Wochenende","Wochenende","Wochenende","Wochenende","Wochenende","Wochenende","Wochenende","Wochenende","Wochenende","Wochenende","Wochenende","Wochenende","Wochenende","Wochenende","Wochenende","Wochenende","."},
                              {"---","FREI","Mathematik R.83","PAUSE","Physik R.201","Physik R.201","PAUSE","Deutsch R.114","Deutsch R.114","FREI","FREI","FREI","PAUSE","Kunst R.170","Kunst R.170","---","."},
                              {"---","Englisch R.183","Englisch R.183","PAUSE","FREI","Mathe R.110","PAUSE","Religion R.83","Religion R.83","FREI","Chemie R.157","Chemie R.157","PAUSE","Sport","Sport","---","."},
                              {"---","Sozialkunde R.K83","Physik R.205","PAUSE","Geschichte R.183","Mathe R.114","PAUSE","Informatik R.173","FREI","Kunst R.170","Orchester","Orchester","---","---","---","---","."},
                              {"---","Englisch R.184","Chemie R.156","PAUSE","Mathe R.112","FREI","PAUSE","Physik R.201","FREI","FREI","Geschichte R.182","Geschichte R.182","PAUSE","Informatik R.173","Informatik R.173","---","."},
                              {"---","Sozialkunde R.K83","Gechichte R.184","PAUSE","Mathe R.153","FREI","PAUSE","Physik R.205","FREI","Deutsch R.181","Chor","Chor","WOCHENENDE!!","Wochenende","Wochenende","Wochenende","."},
                              {"Wochenende","Wochenende","Wochenende","Wochenende","Wochenende","Wochenende","Wochenende","Wochenende","Wochenende","Wochenende","Wochenende","Wochenende","Wochenende","Wochenende","Wochenende","Wochenende","."}};
  
Window *mainWindow;
TextLayer *passedNum;
TextLayer *passedPerc;
TextLayer *timeLayer;
TextLayer *remainingNum;
TextLayer *remainingPerc;
TextLayer *nextClass;

char* floatToString(char* buffer, int bufferSize, double number)
{
	char decimalBuffer[5];
  
	snprintf(buffer, bufferSize, "%d", (int)number);
	strcat(buffer, ".");
  
  if(number<0){
    snprintf(decimalBuffer, 5, "%01d", (int)((double)(number - (int)number) * (double)-10));
  }else{
    snprintf(decimalBuffer, 5, "%01d", (int)((double)(number - (int)number) * (double)10));
  }
	
	strcat(buffer, decimalBuffer);
  strcat(buffer, "%");
  
	return buffer;
}

static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);
  
  //Clock
  static char buffer[] = "00:00";
  strftime(buffer, sizeof("00:00"), "%H:%M", tick_time);
  text_layer_set_text(timeLayer, buffer);
  
  //Get Current Hour
  int curHour = 0;
  int minOfDay = tick_time->tm_hour * 60 + tick_time->tm_min;
  for(int i = 0;times[i+1];i++){
    if(times[i+1]>minOfDay){
      curHour = i;
      break;
    }
  }
  
  int maxHour = curHour;
  if(strcmp(plan[tick_time->tm_wday][maxHour+1],plan[tick_time->tm_wday][curHour])==0){
    maxHour++;
  }

  
  //set NextClass
  if((times[maxHour+1]-minOfDay)<=5){
    if((times[maxHour+1]-minOfDay)==5) vibes_short_pulse();
    text_layer_set_text(nextClass, plan[tick_time->tm_wday][maxHour+1]);
  }else{
    text_layer_set_text(nextClass, plan[tick_time->tm_wday][maxHour]);
  }
  
  //Set Time & Percent Fields
  char *passedNumInt = "1000 min";
  snprintf(passedNumInt, 10, "%d min", minOfDay-times[curHour]);
  text_layer_set_text(passedNum, passedNumInt);
  
  char *remainingNumInt = "100 min";
  snprintf(remainingNumInt, 10, "%d min", minOfDay-times[maxHour+1]);
  text_layer_set_text(remainingNum, remainingNumInt);
  
  char *passedPercInt = "2000 min";
  double passed = (((double)minOfDay-(double)times[curHour])*100/((double)times[maxHour+1]-(double)times[curHour]));
  floatToString(passedPercInt, 10, passed);
  text_layer_set_text(passedPerc, passedPercInt);
  
  char *remainingPercInt = "200 min";
  double remaining = passed-(double)100;
  floatToString(remainingPercInt, 10, remaining);
  text_layer_set_text(remainingPerc, remainingPercInt);
}

void mainWindow_load(Window *thisWindow){
  Layer *root_layer = window_get_root_layer(thisWindow);
  window_set_background_color(thisWindow, GColorBlack);

  //TimeLayer
  timeLayer = text_layer_create(GRect(0,45,144,32));
  text_layer_set_text_alignment(timeLayer, GTextAlignmentCenter);
  text_layer_set_font(timeLayer, fonts_get_system_font(FONT_KEY_BITHAM_30_BLACK));
  text_layer_set_background_color(timeLayer, GColorClear);
  text_layer_set_text_color(timeLayer, GColorWhite);
  text_layer_set_text(timeLayer, "16:50");
  layer_add_child(root_layer, text_layer_get_layer(timeLayer));
  
  //passedNum
  passedNum = text_layer_create(GRect(0,19,72,26));
  text_layer_set_text_alignment(passedNum, GTextAlignmentCenter);
  text_layer_set_font(passedNum, fonts_get_system_font(FONT_KEY_GOTHIC_24));
  text_layer_set_background_color(passedNum, GColorClear);
  text_layer_set_text_color(passedNum, GColorWhite);
  text_layer_set_text(passedNum, "00:100");
  layer_add_child(root_layer, text_layer_get_layer(passedNum));
  
  //passedPerc
  passedPerc = text_layer_create(GRect(72,19,72,26));
  text_layer_set_text_alignment(passedPerc, GTextAlignmentCenter);
  text_layer_set_font(passedPerc, fonts_get_system_font(FONT_KEY_GOTHIC_24));
  text_layer_set_background_color(passedPerc, GColorClear);
  text_layer_set_text_color(passedPerc, GColorWhite);
  text_layer_set_text(passedPerc, "90%");
  layer_add_child(root_layer, text_layer_get_layer(passedPerc));
  
  //remainingNum
  remainingNum = text_layer_create(GRect(0,77,72,26));
  text_layer_set_text_alignment(remainingNum, GTextAlignmentCenter);
  text_layer_set_font(remainingNum, fonts_get_system_font(FONT_KEY_GOTHIC_24));
  text_layer_set_background_color(remainingNum, GColorClear);
  text_layer_set_text_color(remainingNum, GColorWhite);
  text_layer_set_text(remainingNum, "00:010");
  layer_add_child(root_layer, text_layer_get_layer(remainingNum));
  
  //remainingPerc
  remainingPerc = text_layer_create(GRect(72,77,72,26));
  text_layer_set_text_alignment(remainingPerc, GTextAlignmentCenter);
  text_layer_set_font(remainingPerc, fonts_get_system_font(FONT_KEY_GOTHIC_24));
  text_layer_set_background_color(remainingPerc, GColorClear);
  text_layer_set_text_color(remainingPerc, GColorWhite);
  text_layer_set_text(remainingPerc, "10%");
  layer_add_child(root_layer, text_layer_get_layer(remainingPerc));
  
  //nextClass
  nextClass = text_layer_create(GRect(0,120,144,26));
  text_layer_set_text_alignment(nextClass, GTextAlignmentCenter);
  text_layer_set_font(nextClass, fonts_get_system_font(FONT_KEY_GOTHIC_24));
  text_layer_set_background_color(nextClass, GColorClear);
  text_layer_set_text_color(nextClass, GColorWhite);
  text_layer_set_text(nextClass, "Sozialkunde R:173");
  layer_add_child(root_layer, text_layer_get_layer(nextClass));
  
  update_time();
}



static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

//deinit for mainWindow
void mainWindow_unload(Window *thisWindow){
  text_layer_destroy(timeLayer);
  text_layer_destroy(passedNum);
  text_layer_destroy(passedPerc);
  text_layer_destroy(remainingNum);
  text_layer_destroy(remainingPerc);
  text_layer_destroy(nextClass);
}

void handle_init(void) {
  // Register with TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  
  mainWindow = window_create();
  window_set_window_handlers(mainWindow, (WindowHandlers) {
    .load = mainWindow_load,
    .unload = mainWindow_unload,
  });
  window_stack_push(mainWindow, true);
}

void handle_deinit(void) {
  window_destroy(mainWindow);
}

int main(void) {
  handle_init();
  app_event_loop();
  handle_deinit();
}