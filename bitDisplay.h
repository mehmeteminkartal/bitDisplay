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

#pragma once
#include <Arduino.h>

#define MICROBIT_FONT_WIDTH 5
#define MICROBIT_FONT_HEIGHT 5
#define MICROBIT_FONT_ASCII_START 32
#define MICROBIT_FONT_ASCII_END 126


#define LED_ROW1 26
#define LED_ROW2 27
#define LED_ROW3 28

#define LED_COL1 3
#define LED_COL2 4
#define LED_COL3 10
#define LED_COL4 23
#define LED_COL5 24
#define LED_COL6 25
#define LED_COL7 9
#define LED_COL8 7
#define LED_COL9 6

#define sgn(x) ((x<0)?-1:((x>0)?1:0)) /* macro to return the sign of a number */


class bitDisplay {
  public:
    bitDisplay();
	void begin();
    void setPoint(int x, int y);

    void clearPoint(int x, int y);
    void clear();

    void print(const char c, int x, int y);
    void draw();
    void showText(String text);
	void line(double x1, double y1, double x2, double y2);
	void drawForMillis(unsigned long time);
	bool buttonA();
	bool buttonB();
  private:
    int output[3][9];
    void writeCol(int data);
    int col2data(int* col);
	bool oldB = 1;
	bool oldA = 1;
    void selectRow(int row);

};
