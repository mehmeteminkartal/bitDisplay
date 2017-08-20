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

#include <bitDisplay.h>

bitDisplay display;

void setup() {
  Serial.begin(115200);
  display.begin();
  display.showText("Hello");
}



void loop() {
  if (display.buttonA()) {
    display.showText("A");
  }

  if (display.buttonB()) {
    display.showText("B");
  }
  static double angle;
  double x = 2 * cos(angle);
  double y = 2 * sin(angle);

  angle = (angle > PI * 2) ? 0 : (angle + (PI / 6));

  display.line(x + 2, y + 2, 2, 2);
  display.drawForMillis(100);
  display.clear();
}