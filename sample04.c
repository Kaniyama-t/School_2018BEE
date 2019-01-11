/**
 * sample04.c
 *
 * 
 * 右に曲がり，線から外れたら黒い線になるまで左に曲がる
 * COPYRIGHT(c)2019 kaniyama_t.
 */

#include <itron.h> 
#include "sample04.h"
#include "kernel_id.h"
#include "ecrobot_interface.h"

#define LIGHT_THRESHOLD 600	//閾値
#define SOUND_VALUE 600

//液晶ディスプレイに表示するシステム名設定
const char target_subsystem_name[] = "JSP Sample04";

// プロトタイプ宣言
void turnback(void);
void removeAll(void);
void initMove(void);

void jsp_systick_low_priority(void) {
	isig_tim();
	check_NXT_buttons();
}

//初期処理
void ecrobot_device_initialize(void) {
	ecrobot_set_light_sensor_active(NXT_PORT_S1);
	ecrobot_set_light_sensor_active(NXT_PORT_S2);
	//ecrobot_set_sound_sensor_active(NXT_PORT_S3);
}

//後始末処理
void ecrobot_device_terminate(void) {
	ecrobot_set_motor_speed(NXT_PORT_A, 0);
	ecrobot_set_motor_speed(NXT_PORT_C, 0);
	ecrobot_set_light_sensor_inactive(NXT_PORT_S1);
	ecrobot_set_light_sensor_inactive(NXT_PORT_S2);
	//ecrobot_set_sound_sensor_inactive(NXT_PORT_S3);
}

//周期ハンドラ0用の関数
void cyc0(VP_INT exinf) {

	//タスクを起動する
	iact_tsk(TSK0);
}

//周期ハンドラ1用の関数
void cyc1(VP_INT exinf) {

	//タスクを起動する
	iact_tsk(TSK1);
}

//タスク0用の関数
void tsk0(VP_INT exinf){

	//モニタを表示する
	ecrobot_status_monitor(target_subsystem_name);

	//自タスクの終了
	ext_tsk();
}

/*
S1...モード識別カラーセンサー
S2...地面設置部カラーセンサー
S3...サウンドセンサー
S4...正面タッチセンサー
*/

//タスク1用の関数
void tsk1(VP_INT exinf){
	
	// TODO Light Sensor ポート番号変える
	if(ecrobot_get_light_sensor(NXT_PORT_S1) < LIGHT_THRESHOLD){
		//白い場合，turnback()
		initMove();
		
		turnback();
	}else{
		//黒い場合，removeALL()
		initMove();
		
		ecrobot_set_motor_speed(NXT_PORT_C, 0);	
		removeAll();
	}

	//自タスクの終了
	ext_tsk();
}

void initMove(void){
	ecrobot_set_motor_speed(NXT_PORT_A, 80);
	ecrobot_set_motor_speed(NXT_PORT_C, 80);
	// TODO Sound SENSOR ポート番号多分間違ってる
	while(ecrobot_get_sound_sensor(NXT_PORT_S3) <= SOUND_VALUE) systick_wait_ms(50);
	while(ecrobot_get_sound_sensor(NXT_PORT_S3) >= SOUND_VALUE) systick_wait_ms(50);	
	//	systick_wait_ms(2000);
	ecrobot_set_motor_speed(NXT_PORT_A, -80);
	ecrobot_set_motor_speed(NXT_PORT_C, 80);
	systick_wait_ms(2500);
	ecrobot_set_motor_speed(NXT_PORT_A, 0);
	ecrobot_set_motor_speed(NXT_PORT_C, 0);	
}

void turnback(void){
	ecrobot_set_motor_speed(NXT_PORT_A, -100);
	ecrobot_set_motor_speed(NXT_PORT_C, -100);
	
	systick_wait_ms(1000);	
	
	ecrobot_set_motor_speed(NXT_PORT_A, -40);
	ecrobot_set_motor_speed(NXT_PORT_C, -80);
	systick_wait_ms(500);
	
	ecrobot_set_motor_speed(NXT_PORT_A, -80);
	ecrobot_set_motor_speed(NXT_PORT_C, -80);
	
//	while(true){
//		if(ecrobot_get_light_sensor(NXT_PORT_S1) > LIGHT_THRESHOLD){
//			break;
//		}
//		systick_wait_ms(50);
//	}
	
	// TODO Sound SENSOR ポート番号多分間違ってる
	while(ecrobot_get_sound_sensor(NXT_PORT_S3) <= SOUND_VALUE);
	while(ecrobot_get_sound_sensor(NXT_PORT_S3) >= SOUND_VALUE);
	
	ecrobot_set_motor_speed(NXT_PORT_A, 0);
	ecrobot_set_motor_speed(NXT_PORT_C, 0);
	return;
}


void removeAll(void){
	ecrobot_set_motor_speed(NXT_PORT_B, 30);
	systick_wait_ms(100);
	ecrobot_set_motor_speed(NXT_PORT_B, 0);

	ecrobot_set_motor_speed(NXT_PORT_A, -80);
	ecrobot_set_motor_speed(NXT_PORT_C, -80);
	
	// TODO TOUCHセンサー位置を変更
	while(1){
		if(!ecrobot_get_touch_sensor(NXT_PORT_S4)){
			systick_wait_ms(100);
		}else{
			break;
		}
	}
	
	ecrobot_set_motor_speed(NXT_PORT_A, 0);
	ecrobot_set_motor_speed(NXT_PORT_C, 0);
	ecrobot_set_motor_speed(NXT_PORT_B, -30);
	systick_wait_ms(1000);
	ecrobot_set_motor_speed(NXT_PORT_B, 0);
	
}
