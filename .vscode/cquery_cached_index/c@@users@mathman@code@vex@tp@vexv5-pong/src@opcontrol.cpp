#include "main.h"
#include "ball.hpp"
#include "paddle.hpp"
#include <string>

//Made by Joseph from VRC 5588D
//Calvert Hall Robotics

void opcontrol() {
  //config
  constexpr double initial_speed = 4.5;
  constexpr double acceleration = 0.003;
  constexpr int win_score = 7;

  //initialization
  pros::Controller main_controller(pros::E_CONTROLLER_MASTER);
  main_controller.set_text(2, 8, "0  /  0");

  int left_delta, right_delta;

  int left_score = 0, right_score = 0;

  //styles
  static lv_style_t screen_style;
  lv_style_copy(&screen_style, &lv_style_plain_color);
  lv_color_t black;
  black.red = 0x00;
  black.green = 0x00;
  black.blue = 0x00;
  screen_style.body.main_color = black;
  screen_style.body.grad_color = black;

  static lv_style_t line_style;
  lv_style_copy(&line_style, &lv_style_plain_color);
  lv_color_t gray;
  gray.red = 0xB0;
  gray.green = 0xB0;
  gray.blue = 0xB0;
  line_style.body.main_color = gray;
  line_style.body.grad_color = gray;

  static lv_style_t left_paddle_style;
  lv_style_copy(&left_paddle_style, &lv_style_plain_color);
  lv_color_t red;
  red.red = 0xFF;
  red.green = 0x00;
  red.blue = 0x00;
  left_paddle_style.body.main_color = red;
  left_paddle_style.body.grad_color = red;

  static lv_style_t right_paddle_style;
  lv_style_copy(&right_paddle_style, &lv_style_plain_color);
  lv_color_t blue;
  blue.red = 0x00;
  blue.green = 0x00;
  blue.blue = 0xFF;
  right_paddle_style.body.main_color = blue;
  right_paddle_style.body.grad_color = blue;

  static lv_style_t ball_style;
  lv_style_copy(&ball_style, &lv_style_plain_color);
  lv_color_t white;
  white.red = 0xFF;
  white.green = 0xFF;
  white.blue = 0xFF;
  ball_style.body.main_color = white;
  ball_style.body.grad_color = white;

  //initialize screen
  static lv_obj_t* scr = lv_obj_create(NULL, NULL);
  lv_scr_load(scr);

  lv_obj_set_style(scr, &screen_style);
  lv_obj_refresh_style(scr);

  pros::delay(50);

  while(left_score < win_score && right_score < win_score){

    bool missed = false;
    int reset = 0;

    //initialize lvgl objects

    //middle line
    lv_obj_t* middle_line = lv_obj_create(scr, NULL);
    lv_obj_set_size(middle_line, 6, 240);
    lv_obj_set_pos(middle_line, 237, 0);

    lv_obj_set_style(middle_line, &line_style);
    lv_obj_refresh_style(middle_line);

    //paddles
    paddle left_paddle(scr, &left_paddle_style, 20, 95);
    paddle right_paddle(scr, &right_paddle_style, 455, 95);

    //ball
    movingObject ball(scr, &ball_style, 240, 120, initial_speed, get_random_heading(left_score < right_score));

    pros::delay(1000);

    while(!reset){
      //control
      left_paddle.move(
        main_controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y));
        
      right_paddle.move(
        main_controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y));

      //Update Screen
      reset = ball.iterate(left_paddle, right_paddle, &missed);

      left_paddle.print();
      right_paddle.print();

      ball.increaseVelocity(acceleration);
      pros::delay(16);
    }
    //post - round cleanup

    if(reset == 1){
      ++left_score;
    }else if(reset == 2){
      ++right_score;
    }

    const char* scoreboard = (std::to_string(left_score) + "  /  " + std::to_string(right_score)).c_str();
    main_controller.set_text(2, 8, scoreboard);

    lv_obj_clean(scr);
  }
  //post - game
  pros::delay(100);

  main_controller.set_text(0, 4, "The winner is");
  pros::delay(100);

  main_controller.set_text(1, 9, left_score > right_score ? " Left" : "Right");

  while(true){
    pros::delay(100);
  }
}
