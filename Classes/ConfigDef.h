#ifndef _CONFIGDEF_H_
#define _CONFIGDEF_H_

const int INVALID_ENTITY_ID = -1;

const int ARCHER_ENEMY_INIT_NUM = 5;
const int SNAKE_INIT_LENGTH = 5;
const float SNAKE_MOVE_INTERVAL = 0.03f;
const float SNAKE_CLIP_PATH_INTERVAL = 0.5f;
const int SNAKE_LEFT_MIN_EMPTYBODY_POS = 5;
const float RANDOM_FOOD_INTERVAL = 0.3f;
const int MAX_FOOD_NUM = 100;
const float NORMAL_AI_UPDATE_INTERVAL = 0.2f;
const float SNAKE_CHANGE_SIZE_INTERVAL = 1.0f;

const int SNAKE_SCALE_MIN_LENGTH = 20;
const int SNAKE_SCALE_ADD_MAX_LENGTH = 180;
const float SNAKE_SCALE_MIN_SCALE = 1.0f;
const float SNAKE_SCALE_ADD_MAX_SCALE = 1.0f;
const int SNAKE_SCALE_MIN_PATH_INTERVAL = 6;
const int SNAKE_SCALE_ADD_MAX_PATH_INTERVAL = 6;

const float FOOD_STAR_RANDOM_CHANCE = 0.02;
const int FOOD_STAR_SPEED = 200;

const float MAX_EAT_RANGE = 30.0f;

const float MAP_WIDTH = 2048.0f;
const float MAP_HEIGHT = 1536.0f;

const int LENGTH_TO_SCORE = 6;

const float MAP_REGION_WIDTH = 400.0f;
const float MAP_REGION_HEIGHT = 400.0f;

const int REGION_TYPE_FOOD = 1;
const int REGION_TYPE_SKANE = 2;

#endif#
