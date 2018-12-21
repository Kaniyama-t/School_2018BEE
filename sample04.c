/**
 * sample04.c
 *
 * ���C���g���[�X
 * �E�ɋȂ���C������O�ꂽ�獕�����ɂȂ�܂ō��ɋȂ���
 * COPYRIGHT(c)2009 Afrel Co., Ltd.
 * All rights reserved
 */

#include <itron.h> 
#include "sample04.h"
#include "kernel_id.h"
#include "ecrobot_interface.h"

#define LIGHT_THRESHOLD 600	//臒l
#define SOUND_VALUE 600

//�t���f�B�X�v���C�ɕ\������V�X�e�����ݒ�
const char target_subsystem_name[] = "JSP Sample04";

// �v���g�^�C�v�錾
void turnback(void);
void removeAll(void);
void initMove(void);

void jsp_systick_low_priority(void) {
	isig_tim();
	check_NXT_buttons();
}

//��������
void ecrobot_device_initialize(void) {
	ecrobot_set_light_sensor_active(NXT_PORT_S1);
	ecrobot_set_light_sensor_active(NXT_PORT_S2);
	//ecrobot_set_sound_sensor_active(NXT_PORT_S3);
}

//��n������
void ecrobot_device_terminate(void) {
	ecrobot_set_motor_speed(NXT_PORT_A, 0);
	ecrobot_set_motor_speed(NXT_PORT_C, 0);
	ecrobot_set_light_sensor_inactive(NXT_PORT_S1);
	ecrobot_set_light_sensor_inactive(NXT_PORT_S2);
	//ecrobot_set_sound_sensor_inactive(NXT_PORT_S3);
}

//�����n���h��0�p�̊֐�
void cyc0(VP_INT exinf) {

	//�^�X�N���N������
	iact_tsk(TSK0);
}

//�����n���h��1�p�̊֐�
void cyc1(VP_INT exinf) {

	//�^�X�N���N������
	iact_tsk(TSK1);
}

//�^�X�N0�p�̊֐�
void tsk0(VP_INT exinf){

	//���j�^��\������
	ecrobot_status_monitor(target_subsystem_name);

	//���^�X�N�̏I��
	ext_tsk();
}

/*
S1...���[�h���ʃJ���[�Z���T�[
S2...�n�ʐݒu���J���[�Z���T�[
S3...�T�E���h�Z���T�[
S4...���ʃ^�b�`�Z���T�[
*/

//�^�X�N1�p�̊֐�
void tsk1(VP_INT exinf){
	
	// TODO Light Sensor �|�[�g�ԍ��ς���
	if(ecrobot_get_light_sensor(NXT_PORT_S1) < LIGHT_THRESHOLD){
		//�����ꍇ�Cturnback()
		initMove();
		
		turnback();
	}else{
		//�����ꍇ�C�E�ɋȂ���
		initMove();
		
		ecrobot_set_motor_speed(NXT_PORT_C, 0);	
		removeAll();
	}

	//���^�X�N�̏I��
	ext_tsk();
}

void initMove(void){
	ecrobot_set_motor_speed(NXT_PORT_A, 0);
	ecrobot_set_motor_speed(NXT_PORT_C, -80);
	systick_wait_ms(2000);
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
	
	// TODO Sound SENSOR �|�[�g�ԍ������Ԉ���Ă�
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
	
	// TODO TOUCH�Z���T�[�ʒu��ύX
	while(!ecrobot_get_touch_sensor(NXT_PORT_S4)){
		systick_wait_ms(100);
	}
	
	ecrobot_set_motor_speed(NXT_PORT_A, 0);
	ecrobot_set_motor_speed(NXT_PORT_C, 0);
	
}
