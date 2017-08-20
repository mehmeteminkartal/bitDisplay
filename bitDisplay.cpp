/*
 *   Copyright (C) 2017  Muhammet Mehmet Emin KARTAL
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *   Muhammet Mehmet Emin KARTAL <mehmet@mekatrotekno.com>
 */

#include "bitDisplay.h"

#include "Fonts.h"

// Rows and colums, the microbits display is really 3x8 display but displays at 5x5 :D
int cols[] = {LED_COL1, LED_COL2, LED_COL3, LED_COL4, LED_COL5, LED_COL6, LED_COL7, LED_COL8, LED_COL9};
int rows[] = {LED_ROW1, LED_ROW2, LED_ROW3};

// Relative positions from the leds see: https://lancaster-university.github.io/microbit-docs/ubit/display/
const int colPos[5][5] = {
	{1, 2, 1, 2, 1},
	{3, 3, 3, 3, 3},
	{2, 1, 2, 3, 2},
	{1, 1, 1, 1, 1},
	{3, 2, 3, 2, 3},
};

const int rowPos[5][5] = {
	{1, 4, 2, 5, 3},
	{4, 5, 6, 7, 8},
	{2, 9, 3, 9, 1},
	{8, 7, 6, 5, 4},
	{3, 7, 1, 6, 2},
};

bitDisplay::bitDisplay() {}

// Sets the display pins up
void bitDisplay::begin() {
	for (int i = 0; i < 3; i++) {
		pinMode(rows[i], OUTPUT);
		digitalWrite(rows[i], 0);
	}
	for (int i = 0; i < 9; i++) {
		pinMode(cols[i], OUTPUT);
		digitalWrite(cols[i], 0);
	}
	pinMode(PIN_BUTTON_A, INPUT_PULLUP);
	pinMode(PIN_BUTTON_B, INPUT_PULLUP);
}

void bitDisplay::setPoint(int x, int y) {
	output[colPos[x][y] - 1][rowPos[x][y] - 1] = 1;
}

void bitDisplay::clearPoint(int x, int y) {
	output[colPos[x][y] - 1][rowPos[x][y] - 1] = 0;
}

void bitDisplay::clear() {
	for (int x = 0; x < 3; x++ ) {
		for (int y = 0; y < 9; y++ ) {
			output[x][y] = 0;
		}
	}
}

// Prints the char to given coordinates
void bitDisplay::print(const char c, int x, int y) {
	unsigned char v;
	int x1, y1;
	if (x >= MICROBIT_FONT_WIDTH || y >= MICROBIT_FONT_HEIGHT || c < MICROBIT_FONT_ASCII_START || c > MICROBIT_FONT_ASCII_END)
		return;
	int offset = (c - MICROBIT_FONT_ASCII_START) * 5;
	
	for (int row = y; row < MICROBIT_FONT_HEIGHT; row++) {
		v = (char) * (pendolino3 + offset);
		offset++;
		y1 = 0;
		for (int col = x; col < MICROBIT_FONT_WIDTH; col++) {
			if (col <= 5 && col >= 0 && row <= 5 && row >= 0) {
				if (v & (0x10 >> y1)) {
					setPoint(row, col);
				} else {
					//          clearPoint(row, col);
				}
			}
			y1++;
		}
	}
}

// Draws the buffer on the screen
void bitDisplay::draw() {
	for (int i = 0; i < 3; i++ ) {
		int data = this->col2data(this->output[i]);
		if (data != 0 ) {
			selectRow(i);
			writeCol(data);
			delay(3);
			writeCol(0);
		}
	}
}

void bitDisplay::drawForMillis(unsigned long time) {
	unsigned long start = millis();
	while(start + time > millis()) {
		this->draw();
	}
}


//  http://www.brackeen.com/vga/source/djgpp20/lines.c.html
void bitDisplay::line(double x1, double y1, double x2, double y2) {
	int i, dx, dy, sdx, sdy, dxabs, dyabs, x, y, px, py;
	
	if((x1 < 0 || x1 > 4) || (x2 < 0 || x2 > 4) || (y1 < 0 || y1 > 4) || (y2 < 0 || y2 > 4)) {
		return;
	}
	dx = x2 - x1;  /* the horizontal distance of the line */
	dy = y2 - y1;  /* the vertical distance of the line */
	dxabs = abs(dx);
	dyabs = abs(dy);
	sdx = sgn(dx);
	sdy = sgn(dy);
	x = dyabs >> 1;
	y = dxabs >> 1;
	px = x1;
	py = y1;
	setPoint(x1, y1);
	setPoint(x2, y2);
	if (dxabs >= dyabs) { /* the line is more horizontal than vertical */
		for (i = 0; i < dxabs; i++) {
			y += dyabs;
			if (y >= dxabs) {
				y -= dxabs;
				py += sdy;
			}
			px += sdx;
			this->setPoint(px, py);
		}
	} else { /* the line is more vertical than horizontal */
		for (i = 0; i < dyabs; i++) {
			x += dxabs;
			if (x >= dyabs) {
				x -= dyabs;
				px += sdx;
			}
			py += sdy;
			this->setPoint(px, py);
		}
	}
}

void bitDisplay::showText(String text) {
	char* textPresent = (char*)malloc(sizeof(char) * text.length() + 1);
	text.toCharArray(textPresent, text.length() + 1);
	
	int length = text.length();
	int animFrame = -5;
	int c = 0;
	unsigned long p = 0;
	
	while (1) {
		if (p + 130 < millis()) {
			p = millis();
			animFrame++;
			c = (animFrame - (animFrame % 5)) / 5;
			clear();
			print(textPresent[c], -(animFrame % 5), 0);
			if (c != length && textPresent[c + 1] != 0) {
				print(textPresent[c + 1], -( (animFrame % 5) - 5), 0);
			}
			if (c >= length) {
				return; // End of animation
			}
		}
		draw();
		delay(1);
	}
}

void bitDisplay::writeCol(int data) {
	for (int i = 0; i < 9; i++) {
		digitalWrite(cols[i], (data & (1 << i)) != 0 ? 0 : 1);
	}
}

int bitDisplay::col2data(int* col) {
	int data = 0;
	for (int i = 0; i < 9; i++) {
		data += col[i] == 1 ? (1 << i) : 0;
	}
	return data;
}

void bitDisplay::selectRow(int row) {
	digitalWrite(LED_ROW1, row == 0);
	digitalWrite(LED_ROW2, row == 1);
	digitalWrite(LED_ROW3, row == 2);
}


bool bitDisplay::buttonA() {
	if (oldA != digitalRead(PIN_BUTTON_A)) {
		oldA = digitalRead(PIN_BUTTON_A);
		if (!oldA) {
			return true;
		}
	}
	return false;
}

bool bitDisplay::buttonB() {
	if (oldB != digitalRead(PIN_BUTTON_B)) {
		oldB = digitalRead(PIN_BUTTON_B);
		if (!oldB) {
			return true;
		}
	}
	return false;
}

