#include <Arduino.h>
#include "view.h"
#include "networking.h"
#include "sideled.h"

void event_handler_checkbox(struct _lv_obj_t *obj, lv_event_t event);
void event_handler_button(struct _lv_obj_t *obj, lv_event_t event);
void event_handler_dropdown(struct _lv_obj_t *obj, lv_event_t event);
void init_gui_elements();
void mqtt_callback(char *topic, byte *payload, unsigned int length);

unsigned long next_lv_task = 0;

lv_obj_t *led;

lv_obj_t *red_checkbox;
lv_obj_t *blue_checkbox;
lv_obj_t *green_checkbox;
lv_obj_t *purple_checkbox;

lv_obj_t *off_checkbox;
lv_obj_t *on_checkbox;
lv_obj_t *modeSelector;

lv_obj_t *sleep_checkbox;
lv_obj_t *party_checkbox;
lv_obj_t *single_color_checkbox;

lv_obj_t *room1;
lv_obj_t *room2;
lv_obj_t *room3;
lv_obj_t *hallway;

void event_handler_checkbox(struct _lv_obj_t *obj, lv_event_t event)
{
  if (event == LV_EVENT_VALUE_CHANGED)
  {
    if (
        (obj == red_checkbox || obj == blue_checkbox) &&
        (lv_checkbox_is_checked(red_checkbox) || lv_checkbox_is_checked(blue_checkbox)))
    {
      lv_checkbox_set_checked(blue_checkbox, obj == blue_checkbox ? lv_checkbox_is_checked(blue_checkbox) : false);
      lv_checkbox_set_checked(red_checkbox, obj == red_checkbox ? lv_checkbox_is_checked(red_checkbox) : false);
    }
  }
}

void event_handler_dropdown(lv_obj_t *obj, lv_event_t event)
{
  if (event == LV_EVENT_VALUE_CHANGED)
  {
    char buf[32];
    lv_dropdown_get_selected_str(obj, buf, sizeof(buf));
    printf("Option: %s\n", buf);
  }
}

void custom_switch_for_rooms(uint8_t startPin, uint8_t endPin, char *roomName)
{
  Serial.println(startPin + endPin);
  if (lv_checkbox_is_checked(on_checkbox))
  {
    set_sideled_state(startPin, endPin, SIDELED_STATE_ON);
    mqtt_publish(("remoanton/rooms/" + std::string(roomName)).c_str(), "on");
    if (lv_checkbox_is_checked(single_color_checkbox))
    {
      mqtt_publish(("remoanton/rooms/" + std::string(roomName) + "/mode/color").c_str(), "red");
      if (lv_checkbox_is_checked(red_checkbox))
      {
        set_sideled_color(startPin, endPin, CRGB::Red);
        mqtt_publish(("remoanton/rooms/" + std::string(roomName) + "/mode/color").c_str(), "red");
      }
      if (lv_checkbox_is_checked(blue_checkbox))
      {
        set_sideled_color(startPin, endPin, CRGB::Blue);
        mqtt_publish(("remoanton/rooms/" + std::string(roomName) + "/mode/color").c_str(), "blue");
      }
      else if (lv_checkbox_is_checked(green_checkbox))
      {
        set_sideled_color(startPin, endPin, CRGB::Green);
        mqtt_publish(("remoanton/rooms/" + std::string(roomName) + "/mode/color").c_str(), "green");
      }
      else if (lv_checkbox_is_checked(purple_checkbox))
      {
        set_sideled_color(startPin, endPin, CRGB::Purple);
        mqtt_publish(("remoanton/rooms/" + std::string(roomName) + "/mode/color").c_str(), "purple");
      }
    }
    else if (lv_checkbox_is_checked(party_checkbox))
    {
      set_sideled_state(startPin, endPin, SIDELED_STATE_FADE);
      mqtt_publish(("remoanton/rooms/" + std::string(roomName) + "/mode").c_str(), "party");
    }
    else if (lv_checkbox_is_checked(sleep_checkbox))
    {
      set_sideled_color(startPin, endPin, CRGB::Yellow);
      set_sideled_brightness(startPin, endPin, 1);
      mqtt_publish(("remoanton/rooms/" + std::string(roomName) + "/mode").c_str(), "sleep");
    }
  }
  else if (lv_checkbox_is_checked(off_checkbox))
  {
    set_sideled_state(startPin, endPin, SIDELED_STATE_OFF);
    mqtt_publish(("remoanton/rooms/" + std::string(roomName)).c_str(), "off");
  }
}

void event_handler_button(struct _lv_obj_t *obj, lv_event_t event)
{
  if (event == LV_EVENT_PRESSED)
  {
    if (obj == room1)
    {
      custom_switch_for_rooms(0, 7, "1");
    }
    else if (obj == room2)
    {
      custom_switch_for_rooms(7, 12, "2");
    }
    else if (obj == room3)
    {
      custom_switch_for_rooms(12, 25, "3");
    }
    else if (obj == hallway)
    {
      custom_switch_for_rooms(25, 30, "hallway");
    }
  }
}

void init_gui_elements()
{
  add_label("Light Configuration", 10, 10);
  on_checkbox = add_checkbox("On", 10, 40, event_handler_checkbox);
  off_checkbox = add_checkbox("Off", 100, 40, event_handler_checkbox);
  add_label("1. Select Mode", 10, 70);
  single_color_checkbox = add_checkbox("Color", 10, 100, event_handler_checkbox);
  party_checkbox = add_checkbox("Party", 100, 100, event_handler_checkbox);
  sleep_checkbox = add_checkbox("Sleep", 190, 100, event_handler_checkbox);
  add_label("2. Select Color", 10, 130);
  red_checkbox = add_checkbox("Rd", 0, 150, event_handler_checkbox);
  blue_checkbox = add_checkbox("Bl", 50, 150, event_handler_checkbox);
  green_checkbox = add_checkbox("Gr", 100, 150, event_handler_checkbox);
  purple_checkbox = add_checkbox("Prpl", 150, 150, event_handler_checkbox);
  add_label("3. Select Room:", 10, 180);
  room1 = add_button("Room 1", event_handler_button, 0, 200, 75, 38);
  room2 = add_button("Room 2", event_handler_button, 80, 200, 75, 38);
  room3 = add_button("Room 3", event_handler_button, 160, 200, 75, 38);
  hallway = add_button("Hallway", event_handler_button, 240, 200, 75, 38);
}

// ----------------------------------------------------------------------------
// MQTT callback
// ----------------------------------------------------------------------------

void mqtt_callback(char *topic, byte *payload, unsigned int length)
{
  Serial.println("Message arrived " + String(topic));
  auto topicS = String(topic);
  String room = topicS.substring(0, topicS.lastIndexOf("/"));
  room = room.substring(room.lastIndexOf("/") + 1);

  // Parse Payload into String
  char *buf = (char *)malloc((sizeof(char) * (length + 1)));
  memcpy(buf, payload, length);
  buf[length] = '\0';
  String payloadS = String(buf);
  payloadS.trim();

  uint8_t begin;
  uint8_t end;

  if (room == "1")
  {
    begin = 0;
    end = 7;
  }
  else if (room == "2")
  {
    begin = 7;
    end = 12;
  }
  else if (room == "3")
  {
    begin = 12;
    end = 25;
  }
  else if (room == "hallway")
  {
    begin = 25;
    end = 30;
  }
  if (topicS.endsWith("/mode"))
  {
    if (payloadS == "on")
    {
      set_sideled_state(begin, end, SIDELED_STATE_ON);
    }
    else if (payloadS == "off")
    {
      set_sideled_state(begin, end, SIDELED_STATE_OFF);
    }
    else if (payloadS == "party")
    {
      set_sideled_state(begin, end, SIDELED_STATE_BLINK);
    }
    else if (payloadS == "sleep")
    {
      set_sideled_brightness(begin, end, 1);
    }
  }
  else if (topicS.endsWith("/color"))
  {
    if (payloadS == "red")
    {
      set_sideled_color(begin, end, CRGB::Red);
    }
    if (payloadS == "blue")
    {
      set_sideled_color(begin, end, CRGB::Blue);
    }
    if (payloadS == "green")
    {
      set_sideled_color(begin, end, CRGB::Green);
    }
    if (payloadS == "purple")
    {
      set_sideled_color(begin, end, CRGB::Purple);
    }
  }
}

// ----------------------------------------------------------------------------
// UI event handlers
// ----------------------------------------------------------------------------

String buffer = "";

void event_handler_num(struct _lv_obj_t *obj, lv_event_t event)
{
}

lv_obj_t *mbox;

// ----------------------------------------------------------------------------
// MAIN LOOP
// ----------------------------------------------------------------------------

void loop()
{
  if (next_lv_task < millis())
  {
    lv_task_handler();
    next_lv_task = millis() + 5;
  }
  // Uncomment the following lines to enable MQTT
  mqtt_loop();
}

// ----------------------------------------------------------------------------
// MAIN SETUP
// ----------------------------------------------------------------------------
void setup()
{
  init_m5();
  init_display();
  Serial.begin(115200);
  lv_obj_t *wifiConnectingBox = show_message_box_no_buttons("Connecting to WiFi...");
  lv_task_handler();
  delay(5);
  setup_wifi();
  mqtt_init(mqtt_callback);
  close_message_box(wifiConnectingBox);
  init_gui_elements();
  init_sideled();
  set_sideled_state(0, 30, SIDELED_STATE_OFF);
}