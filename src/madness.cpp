// #include "pros/apix.h"
// LV_IMG_DECLARE(REDRIGHT);
// LV_IMG_DECLARE(REDLEFT);
// LV_IMG_DECLARE(BLUELEFT);
// LV_IMG_DECLARE(BLUERIGHT);
// LV_IMG_DECLARE(Rings);
// LV_IMG_DECLARE(Mogo);
// bool blueAuton = false;
// bool mogoAuton = false;
// extern void loadAutonScreen();
// // In a .cpp file (e.g., lvgl_screens.cpp)
// lv_obj_t *autonTypeScreen = NULL;
// lv_obj_t *redLeftScreen = NULL;
// lv_obj_t *redRightScreen = NULL;
// lv_obj_t *blueLeftScreen = NULL;
// lv_obj_t *blueRightScreen = NULL;
// lv_obj_t *allianceScreen = NULL;


// // Callback function for Button 1
// void blue_btn_event_handler(lv_event_t * e) {
//   lv_obj_t * btn = lv_event_get_target(e);
//   blueAuton = true;
//   lv_scr_load_anim(autonTypeScreen, LV_SCR_LOAD_ANIM_OVER_BOTTOM, 500, 0, true);
// }

// // Callback function for Button 2
// void red_btn_event_handler(lv_event_t * e) {
//   lv_obj_t * btn = lv_event_get_target(e);
//   blueAuton = false;
//   lv_scr_load_anim(autonTypeScreen, LV_SCR_LOAD_ANIM_OVER_BOTTOM, 500, 0, true);
// }

// // Callback function for Button 3
// void mogo_side_event_handler(lv_event_t * e) {
//   lv_obj_t * btn = lv_event_get_target(e);
//   mogoAuton = true;
//   loadAutonScreen();


// }

// // Callback function for Button 4
// void ring_side_event_handler(lv_event_t * e) {
//   lv_obj_t * btn = lv_event_get_target(e);
//   mogoAuton = false;
//   loadAutonScreen();
  
// }
// void displayRedLeft(){
//   lv_obj_t * img = lv_img_create(redLeftScreen);
//   lv_img_set_src(img, &REDLEFT);
//   lv_obj_align(img, LV_ALIGN_BOTTOM_LEFT, 0, 0);
// }
// void displayRedRight(){
//   lv_obj_t * img = lv_img_create(redRightScreen);
//   lv_img_set_src(img, &REDRIGHT);
//   lv_obj_align(img, LV_ALIGN_BOTTOM_LEFT, 0, 0);
// }
// void displayBlueLeft(){
//   lv_obj_t * img = lv_img_create(blueLeftScreen);
//   lv_img_set_src(img, &BLUELEFT);
//   lv_obj_align(img,  LV_ALIGN_BOTTOM_LEFT, 0, 0);
// }
// void displayBlueRight(){
//   lv_obj_t * img = lv_img_create(blueRightScreen);
//   lv_img_set_src(img, &BLUERIGHT);
//   lv_obj_align(img, LV_ALIGN_BOTTOM_LEFT, 0,0);
// }
// void createScreens(){
//   allianceScreen = lv_img_create(lv_scr_act());
// 	autonTypeScreen = lv_img_create(NULL);
// 	redLeftScreen = lv_img_create(NULL);
// 	redRightScreen = lv_img_create(NULL);
// 	blueLeftScreen = lv_img_create(NULL);
// 	blueRightScreen = lv_img_create(NULL);
//   lv_obj_set_style_bg_color(allianceScreen, lv_palette_main(LV_PALETTE_DEEP_ORANGE), 0);
//   lv_obj_set_style_bg_color(autonTypeScreen, lv_palette_main(LV_PALETTE_DEEP_ORANGE), 0);
//   lv_obj_set_style_bg_color(redLeftScreen, lv_palette_main(LV_PALETTE_DEEP_ORANGE), 0);
//   lv_obj_set_style_bg_color(redRightScreen, lv_palette_main(LV_PALETTE_DEEP_ORANGE), 0);
//   lv_obj_set_style_bg_color(blueLeftScreen, lv_palette_main(LV_PALETTE_DEEP_ORANGE), 0);
//   lv_obj_set_style_bg_color(blueRightScreen, lv_palette_main(LV_PALETTE_DEEP_ORANGE), 0);
// }
// void loadAutonScreen(){
//   if(blueAuton && mogoAuton){
//     displayBlueLeft();
//   }
//   else if(blueAuton && !mogoAuton){
//     displayBlueRight();
//   }
//   else if(!blueAuton && !mogoAuton){
//     displayRedLeft();
//   }
//   else{
//     displayRedRight();
//   }
// }
// void create_buttons() {
//   // Create a blue button
//   lv_obj_t * blue_btn = lv_btn_create(allianceScreen);
//   lv_obj_remove_style_all(blue_btn);                          
//   lv_obj_add_flag(blue_btn, LV_OBJ_FLAG_CHECKABLE);  // Make the button checkable

//   // Default Blue Style
//   static lv_style_t blueStyle;
//   lv_style_init(&blueStyle);
//   lv_style_set_bg_color(&blueStyle, lv_palette_main(LV_PALETTE_BLUE));

//   // Dark Blue Checked Style
//   static lv_style_t darkBlueStyle;
//   lv_style_init(&darkBlueStyle);
//   lv_style_set_bg_color(&darkBlueStyle, lv_palette_darken(LV_PALETTE_BLUE, 3));  // Darker blue

//   // Apply default style
//   lv_obj_add_style(blue_btn, &blueStyle, LV_STATE_DEFAULT);
//   lv_obj_add_style(blue_btn, &darkBlueStyle, LV_STATE_CHECKED);  // Apply dark blue when checked
//   lv_obj_add_event_cb(blue_btn, blue_btn_event_handler, LV_EVENT_CLICKED, NULL);
//   lv_obj_set_size(blue_btn, 120, 240);
//   lv_obj_align(blue_btn, LV_ALIGN_LEFT_MID, 0, 0);

//   // Create a red button
//   lv_obj_t * red_btn = lv_btn_create(allianceScreen);
//   lv_obj_remove_style_all(red_btn);                          
//   lv_obj_add_flag(red_btn, LV_OBJ_FLAG_CHECKABLE);  // Make the button checkable

//   // Default Red Style
//   static lv_style_t redStyle;
//   lv_style_init(&redStyle);
//   lv_style_set_bg_color(&redStyle, lv_palette_main(LV_PALETTE_RED));

//   // Dark Red Checked Style
//   static lv_style_t darkRedStyle;
//   lv_style_init(&darkRedStyle);
//   lv_style_set_bg_color(&darkRedStyle, lv_palette_darken(LV_PALETTE_RED, 3));  // Darker red

//   // Apply default style
//   lv_obj_add_style(red_btn, &redStyle, LV_STATE_DEFAULT);
//   lv_obj_add_style(red_btn, &darkRedStyle, LV_STATE_CHECKED);  // Apply dark red when checked
//   lv_obj_add_event_cb(red_btn, red_btn_event_handler, LV_EVENT_CLICKED, NULL);
//   lv_obj_set_size(red_btn, 120, 240);


//   // Create a button with an image
  
//   lv_obj_t * mogoSide = lv_imgbtn_create(autonTypeScreen);
//   lv_imgbtn_set_src(mogoSide, LV_IMGBTN_STATE_RELEASED, NULL, &Mogo, NULL);
//   lv_obj_align(mogoSide, LV_ALIGN_CENTER, -120, 100);
//   lv_obj_add_event_cb(mogoSide, mogo_side_event_handler, LV_EVENT_CLICKED, NULL);
//   // Create another button with an image
  
//   lv_obj_t * ringSide = lv_imgbtn_create(autonTypeScreen);
//   lv_imgbtn_set_src(ringSide, LV_IMGBTN_STATE_RELEASED, NULL, &Rings, NULL);
//   lv_obj_align(ringSide, LV_ALIGN_CENTER, 120, 100);
//   lv_obj_add_event_cb(ringSide, ring_side_event_handler, LV_EVENT_CLICKED, NULL);
// }
