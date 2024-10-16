#include "main.h"

static int average_8(float x) {
    static int samples[8];
    static int i = 0;
    static int total = 0;

    total += (int) x - samples[i];
    samples[i] = (int) x;
    i = (i == 7) ? 0 : i+1;

    return total >> 3;
}

static int average_16(float x) {
    static int samples[16];
    static int i = 0;
    static int total = 0;

    total += (int) x - samples[i];
    samples[i] = (int) x;
    i = (i == 15) ? 0 : i+1;

    return total >> 4;
}

static uint8_t convertToPercentage(int x) {
	// calculate percentage and return
	return (uint8_t) ((100 * x) / 4080);
}

static void BSP_LCD_DrawArc(uint16_t centerX, uint16_t centerY, uint16_t radius, float startAngle, float endAngle, uint32_t color) {
    float angle;
    int16_t x, y;

    // Start angle and end angle should be converted to radians if they're in degrees
    for (angle = startAngle; angle <= endAngle; angle += 0.5f) {
        // Convert angle to radians
        float radian = angle * M_PI / 180.0f;

        // Calculate the (x, y) point on the circle
        x = centerX + (int16_t)(radius * cos(radian));
        y = centerY + (int16_t)(radius * sin(radian));

        // Draw the pixel at (x, y)
        BSP_LCD_DrawPixel(x, y, color);
    }
}

static void BSP_LCD_DisplayStringAtCenterInRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t textWidth, uint16_t textHeight, char* text, uint32_t color) {
	// Calculate the starting position to center the text in the rectangle
    int16_t startX = x + (width - textWidth * strlen(text)) / 2;
    int16_t startY = y + (height - textHeight) / 2;

    // Set text color
    BSP_LCD_SetTextColor(color);

    // Display the text at the computed coordinates
    BSP_LCD_DisplayStringAt(startX, startY, (uint8_t*)text, LEFT_MODE);  // Use LEFT_MODE for exact position
}

static void update_guage_value(uint16_t center_x, uint16_t center_y, uint8_t start, uint8_t stop, uint8_t percentage, uint8_t isActive){
	float max_angle = (((percentage * 300.00) / 100.00) + 120);

	for (uint8_t r = start + 1; r <= stop + 1; r++){
		if (isActive) BSP_LCD_DrawArc(center_x, center_y, r, 120, max_angle, LCD_COLOR_RED);
		else BSP_LCD_DrawArc(center_x, center_y, r, 120, max_angle, LCD_COLOR_RUBYRED);
	}

	for (uint8_t r = start + 1; r <= stop + 1; r++){
		BSP_LCD_DrawArc(center_x, center_y, r, max_angle, 420, LCD_COLOR_LIGHTGRAY);
	}

	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
	BSP_LCD_FillCircle(center_x, center_y, start);

	char percentage_buff[5];
	sprintf(percentage_buff, "%d", percentage);
	BSP_LCD_SetFont(&Font20);
	BSP_LCD_SetBackColor(LCD_COLOR_WHITE);

	if (isActive) BSP_LCD_SetTextColor(LCD_COLOR_RED);
	else BSP_LCD_SetTextColor(LCD_COLOR_RUBYRED);

	uint8_t percentage_offset[2] = {0, 8};
	if (strlen(percentage_buff) == 3) {
		percentage_offset[0] = 20; // x
	}
	else if (strlen(percentage_buff) == 2) {
		percentage_offset[0] = 14; // x
	}
	else {
		percentage_offset[0] = 7; // x
	}
	BSP_LCD_DisplayStringAt(center_x - percentage_offset[0], center_y - percentage_offset[1], (uint8_t *)percentage_buff, LEFT_MODE);
}

//static void update_percentage_display(uint16_t center_x, uint16_t center_y, uint8_t percentage, uint8_t isActive){
//	char percentage_buff[5];
//	sprintf(percentage_buff, "%d", percentage);
//	BSP_LCD_SetFont(&Font20);
//	BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
//
//	if (isActive) BSP_LCD_SetTextColor(LCD_COLOR_RED);
//	else BSP_LCD_SetTextColor(LCD_COLOR_RUBYRED);
//
//	uint8_t percentage_offset[2] = {0, 8};
//	if (strlen(percentage_buff) == 3) {
//		percentage_offset[0] = 20; // x
//	}
//	else if (strlen(percentage_buff) == 2) {
//		percentage_offset[0] = 14; // x
//	}
//	else {
//		percentage_offset[0] = 7; // x
//	}
//	BSP_LCD_DisplayStringAt(center_x - percentage_offset[0], center_y - percentage_offset[1], (uint8_t *)percentage_buff, LEFT_MODE);
//}

// rotation of the screen is almost impossible now, so we will make do with what we have
static void Effect_Circle_Display(uint16_t center_x, uint16_t center_y, uint8_t percentage, char* val_name, uint8_t isActive){
	uint16_t outer_radius = 40;
	uint16_t progress_radius = 34;
	uint16_t inner_radius = 30;

	// clear the display
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
	BSP_LCD_FillRect(center_x - outer_radius - 10, center_y - outer_radius - 10, (outer_radius + 10) * 2 + 2, (outer_radius + 10) * 2 + (2 * 20) + 5);

	BSP_LCD_SetTextColor(LCD_COLOR_DARKGRAY);
	BSP_LCD_FillCircle(center_x, center_y, outer_radius);

//	BSP_LCD_SetTextColor(LCD_COLOR_LIGHTGRAY);
//	BSP_LCD_FillCircle(center_x, center_y, progress_radius);

	// test draw arc
	for (uint8_t r = inner_radius; r <= progress_radius + 1; r++){
		BSP_LCD_DrawArc(center_x, center_y, r, 60, 120, LCD_COLOR_DARKGRAY);
	}

	update_guage_value(center_x, center_y, inner_radius, progress_radius, percentage, isActive);

	// for font 16
	// width = 11, height = 16
	// for font 12
	// width = 7, height = 12
	const char s[2] = "-";
	char *texts;
	texts = strtok(val_name, s);

	BSP_LCD_SetFont(&Font12);
	uint16_t start_y = 50;
	uint8_t text_line = 0;
	while( texts != NULL){
		BSP_LCD_DisplayStringAtCenterInRect(center_x - outer_radius, center_y + start_y, outer_radius * 2, 14, 7, 12, texts, LCD_COLOR_DARKGRAY);
		start_y += 20;
		text_line++;

		texts = strtok(NULL, s);
	}

	if (isActive){
		BSP_LCD_SetTextColor(LCD_COLOR_LIGHTRED);
		BSP_LCD_DrawRect(center_x - outer_radius - 10, center_y - outer_radius - 10, (outer_radius + 10) * 2, (outer_radius + 10) * 2 + (text_line * 20) + 5);
	}
}

static void Display_Detail(char* mode_name, char* state_name){
	BSP_LCD_SetTextColor(LCD_COLOR_RED);
	BSP_LCD_FillRect(0, 0, BSP_LCD_GetXSize(), 90);

	BSP_LCD_SetTextColor(LCD_COLOR_DARKRED);
	BSP_LCD_FillRect(0, 90, BSP_LCD_GetXSize(), 2);


	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
	BSP_LCD_SetBackColor(LCD_COLOR_RED);

	BSP_LCD_SetFont(&Font20);
	char mode_buff[50];
	sprintf(mode_buff, "EFFECT: %s", mode_name);
	BSP_LCD_DisplayStringAt(0, 20, (uint8_t*) mode_buff, CENTER_MODE);

	BSP_LCD_SetFont(&Font16);
	char state_buff[50];
	sprintf(state_buff, " - %s - ", state_name);
	BSP_LCD_DisplayStringAt(0, 50, (uint8_t*) state_buff, CENTER_MODE);

	BSP_LCD_DrawHLine(80, 42, BSP_LCD_GetXSize() - 160);
	BSP_LCD_DrawHLine(80, 43, BSP_LCD_GetXSize() - 160);
}

void change_effect_display(){
	// works needed to be done is the following :
	// clear the screen
	// call detail
	// call function which draw circle according to each function

	BSP_LCD_Clear(LCD_COLOR_WHITE);

	Display_Detail((char *) effects[EffectIndex].Name, "READY TO RECORD");

	uint32_t value_circle_height = BSP_LCD_GetYSize() / 2 + 30;
	if (effects[EffectIndex].ParametersNum == 2){
		Effect_Circle_Display((BSP_LCD_GetXSize() / 4) * 1, value_circle_height, convertToPercentage(effects[EffectIndex].Parameter1), effect_names[EffectIndex][0], 0);
		Effect_Circle_Display((BSP_LCD_GetXSize() / 4) * 3, value_circle_height, convertToPercentage(effects[EffectIndex].Parameter2), effect_names[EffectIndex][1], 0);
	}
	else if (effects[EffectIndex].ParametersNum == 3){
		Effect_Circle_Display((BSP_LCD_GetXSize() / 6) * 1, value_circle_height, convertToPercentage(effects[EffectIndex].Parameter1), effect_names[EffectIndex][0], 0);
		Effect_Circle_Display((BSP_LCD_GetXSize() / 6) * 3, value_circle_height, convertToPercentage(effects[EffectIndex].Parameter2), effect_names[EffectIndex][1], 0);
		Effect_Circle_Display((BSP_LCD_GetXSize() / 6) * 5, value_circle_height, convertToPercentage(effects[EffectIndex].Parameter3), effect_names[EffectIndex][2], 0);
	}
	else { // 4
		Effect_Circle_Display((BSP_LCD_GetXSize() / 8) * 1, value_circle_height, convertToPercentage(effects[EffectIndex].Parameter1), effect_names[EffectIndex][0], 0);
		Effect_Circle_Display((BSP_LCD_GetXSize() / 8) * 3, value_circle_height, convertToPercentage(effects[EffectIndex].Parameter2), effect_names[EffectIndex][1], 0);
		Effect_Circle_Display((BSP_LCD_GetXSize() / 8) * 5, value_circle_height, convertToPercentage(effects[EffectIndex].Parameter3), effect_names[EffectIndex][2], 0);
		Effect_Circle_Display((BSP_LCD_GetXSize() / 8) * 7, value_circle_height, convertToPercentage(effects[EffectIndex].Parameter4), effect_names[EffectIndex][3], 0);
	}
}

void change_guage_display(uint8_t isActive) {
	// draw inactive circle instead of the old active one
	uint32_t value_circle_height = BSP_LCD_GetYSize() / 2 + 30;
	uint8_t value = 0;

	switch (ParameterIndex){
		case 0:
			value = convertToPercentage(effects[EffectIndex].Parameter1);
			break;
		case 1:
			value = convertToPercentage(effects[EffectIndex].Parameter2);
			break;
		case 2:
			value = convertToPercentage(effects[EffectIndex].Parameter3);
			break;
		case 3:
			value = convertToPercentage(effects[EffectIndex].Parameter4);
			break;
		default:
			break;
	}

	if (effects[EffectIndex].ParametersNum == 2){
		Effect_Circle_Display((BSP_LCD_GetXSize() / 4) * (2 * ParameterIndex + 1), value_circle_height, value, effect_names[EffectIndex][ParameterIndex], isActive);
	}
	else if (effects[EffectIndex].ParametersNum == 3){
		Effect_Circle_Display((BSP_LCD_GetXSize() / 6) * (2 * ParameterIndex + 1), value_circle_height, value, effect_names[EffectIndex][ParameterIndex], isActive);
	}
	else { // 4
		Effect_Circle_Display((BSP_LCD_GetXSize() / 8) * (2 * ParameterIndex + 1), value_circle_height, value, effect_names[EffectIndex][ParameterIndex], isActive);
	}
}

void update_guage_value_display() {
	// draw inactive circle instead of the old active one
	uint32_t center_y = BSP_LCD_GetYSize() / 2 + 30;
	uint32_t center_x = (BSP_LCD_GetXSize() / 4) * (2 * ParameterIndex + 1);
	uint8_t value = 0;

	switch (ParameterIndex){
		case 0:
			value = convertToPercentage(effects[EffectIndex].Parameter1);
			break;
		case 1:
			value = convertToPercentage(effects[EffectIndex].Parameter2);
			break;
		case 2:
			value = convertToPercentage(effects[EffectIndex].Parameter3);
			break;
		case 3:
			value = convertToPercentage(effects[EffectIndex].Parameter4);
			break;
		default:
			break;
	}

	uint8_t inner_radius = 30;
	uint8_t outer_radius = 34;
	if (effects[EffectIndex].ParametersNum == 2){
		center_x = (BSP_LCD_GetXSize() / 4) * (2 * ParameterIndex + 1);
	}
	else if (effects[EffectIndex].ParametersNum == 3){
		center_x = (BSP_LCD_GetXSize() / 6) * (2 * ParameterIndex + 1);
	}
	else { // 4
		center_x = (BSP_LCD_GetXSize() / 8) * (2 * ParameterIndex + 1);
	}
	update_guage_value(center_x, center_y, inner_radius, outer_radius, value, 1);
//	update_percentage_display(center_x, center_y, value, 1);
}