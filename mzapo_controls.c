/* This lib will handle actions and its effects on the game, main menu etc.
   So if we will work on mzapo board, this lib will be taking care of reading,
   actions from knobs and sending needed info to the game.  */

#include <stdio.h>
#include "mzapo_controls.h"

/*  If we register some actio, use this function to inform and modify game.  */
void ActOnAction(SceneData* scene_data, Canvas* game_canvas, int action){
	if(CURRENT_SCENE == SCENE_MENU){ // We are in the menu, button 1 has no effects
		
		switch(action){
			case(ACTION_BUTTON_1_CLICKED):
				printf("Action ACTION_BUTTON_1_CLICKED\n");
				
				switch(scene_data[SCENE_MENU].knob_menu_position){
					case(KNOB_MENU_PLAY):
						// Load MAP
						printf("\n! STARTING GAME !\n");
						///// Load game map scene
						LoadScene(scene_data, game_canvas, SCENE_GAME_MAP);
						/////////////////////
						break;
					case(KNOB_MENU_WIND): 
						// Toggle wind
						scene_data[SCENE_MENU].wind_on = !scene_data[SCENE_MENU].wind_on;
						break;
					case(KNOB_MENU_PLAYER_1):
						// Add tanks
						if(scene_data[SCENE_MENU].player_1_tanks < scene_data[SCENE_MENU].player_max_tanks){
							scene_data[SCENE_MENU].player_1_tanks++;
						}else{
							scene_data[SCENE_MENU].player_1_tanks = 1;
						}
						break;
					case(KNOB_MENU_PLAYER_2):
						// Add tanks
						if(scene_data[SCENE_MENU].player_2_tanks < scene_data[SCENE_MENU].player_max_tanks){
							scene_data[SCENE_MENU].player_2_tanks++;
						}else{
							scene_data[SCENE_MENU].player_2_tanks = 1;
						}
						break;
				}
				break;
			case(ACTION_BUTTON_1_MOVE_LEFT):
				printf("Action ACTION_BUTTON_1_MOVE_LEFT\n");
				
				if(scene_data[SCENE_MENU].knob_menu_position > 0){
					scene_data[SCENE_MENU].knob_menu_position--;
				}else{
					scene_data[SCENE_MENU].knob_menu_position = KNOB_MENU_TOTAL_POSITIONS - 1;
				}
				
				break;
			case(ACTION_BUTTON_1_MOVE_RIGHT):
				printf("Action ACTION_BUTTON_1_MOVE_RIGHT\n");
				
				if(scene_data[SCENE_MENU].knob_menu_position < (KNOB_MENU_TOTAL_POSITIONS - 1)){
					scene_data[SCENE_MENU].knob_menu_position++;
				}else{
					scene_data[SCENE_MENU].knob_menu_position = 0;
				}
				
				break;
		}
	}else if(CURRENT_SCENE == SCENE_GAME_MAP){
		printf("scene_data[SCENE_MENU].movement_confirmed: %d\n", scene_data[SCENE_MENU].movement_confirmed);
		// If we confirmed our tank movement, use button 1 for something else. -> 'movement_confirmed'
		switch(action){
			case(ACTION_BUTTON_1_CLICKED):
				printf("Action ACTION_BUTTON_1_CLICKED\n");
				// CONFIRM POSITION -> THEN USE BUTTON 1 FOR SETTING PROJECTAL VELOCITY
				if(scene_data[SCENE_GAME_MAP].movement_confirmed == false){
					// Confirm movement
					scene_data[SCENE_GAME_MAP].movement_confirmed = true;
					printf("Movement confirmed! Ready to set projectile velocity and shoot!\n");
				}else{
					/* SHOOT i.e. fill data to projectile variable in scene_data, set animating to true
					   while animating will be true, input will be ignored and each frame
					   we will move by bit. If the 'to_explode' projectile variable is true
					   start explosion sequence. */
					printf("SHOOT: Starting projectile animation!\n");
					StartProjectileAnimation(scene_data);
				}
				
				break;
			case(ACTION_BUTTON_1_MOVE_LEFT):
				if(scene_data[SCENE_GAME_MAP].movement_confirmed == false){
					printf("Action ACTION_BUTTON_1_MOVE_LEFT\n");
					// MOVE LEFT 
					MoveTank(scene_data, true); // MOVE LEFT
				}else{
					// DECREASE PROJECTAL VELOCITY
					ChangeProjectileVelocity(scene_data, false); // false-decrease velocity
				}
				RefreshAimPath(scene_data, false, false); // Refresh aim_path_public data in scenedata
				break;
			case(ACTION_BUTTON_1_MOVE_RIGHT):
				if(scene_data[SCENE_GAME_MAP].movement_confirmed == false){
					printf("Action ACTION_BUTTON_1_MOVE_RIGHT\n");
					// MOVE RIGHT
					MoveTank(scene_data, false); // MOVE RIGHT
				}else{
					// INCRESE PROJECTAL VELOCITY
					ChangeProjectileVelocity(scene_data, true); // true-increase velocity
				}
				RefreshAimPath(scene_data, false, false); // Refresh aim_path_public data in scenedata
				break;
			case(ACTION_BUTTON_2_CLICKED):
				printf("Action ACTION_BUTTON_1_CLICKED\n");
				// Change weapon
				ChangeWapon(scene_data);
				RefreshAimPath(scene_data, false, false); // Refresh aim_path_public data in scenedata
				printf("Changing weapon!\n");
				break;
			case(ACTION_BUTTON_2_MOVE_LEFT):
				printf("Action ACTION_BUTTON_1_MOVE_LEFT\n");
				// MOVE TANK BARREL FROM 0 TO 180 
				MoveBarrel(scene_data, true); // true->add 1 degree
				RefreshAimPath(scene_data, false, false); // Refresh aim_path_public data in scenedata
				break;
			case(ACTION_BUTTON_2_MOVE_RIGHT):
				printf("Action ACTION_BUTTON_1_MOVE_RIGHT\n");
				// MOVE TANK BARREL FROM 180 TO 0 
				MoveBarrel(scene_data, false); // false->subtract 1 degree
				RefreshAimPath(scene_data, false, false); // Refresh aim_path_public data in scenedata
				break;
		}
	}
	
	scene_data[CURRENT_SCENE].last_action = action;
	return;
}


// Read knob data and decide if its a new action
// void CheckUserInput(){}






