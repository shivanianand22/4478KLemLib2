// #include "liblvgl/core/lv_obj.h"
// #include "liblvgl/core/lv_obj_style.h"
// #include "liblvgl/misc/lv_area.h"
// #include "lvgl_actions.h"
// #include "liblvgl/misc/lv_style.h"
// #include "pros/apix.h"
// LV_IMG_DECLARE(REDRIGHT);
// LV_IMG_DECLARE(REDLEFT);
// LV_IMG_DECLARE(BLUELEFT);
// LV_IMG_DECLARE(BLUERIGHT);
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