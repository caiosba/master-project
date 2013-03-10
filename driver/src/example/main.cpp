/*
Copyright (c) 2012, Justin Philipp Heinermann and Jendrik Poloczek
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met: 

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer. 
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution. 

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The views and conclusions contained in the software and documentation are those
of the authors and should not be interpreted as representing official policies, 
either expressed or implied, of the FreeBSD Project.
*/

#include "../Logger.h" 
#include "../AttitudeSensor.h"
#include <stdio.h>
#include <iostream>
#include <math.h>

#define THRESHOLD 2
#define ZERO_YAW 0
#define ZERO_PITCH 0
#define ZERO_ROLL 19
#define LIMIT 52 
#define INTERVAL 0.2 // Seconds

AttitudeSensor * as;
const Head * head;
bool run=true;
int counter=0;

int main(){
	as = new AttitudeSensor();
	if(as->vuzixConnected){
		while(run){
			if(!as->vuzixConnected){
				run=false;
				break;
			}
			as->timerProc();
			head=as->getHeadDirection();

      int yaw = round(((head->angles.yawDeg * 180 / M_PI) / 1000) - ZERO_YAW);
      int pitch = round(((head->angles.pitchDeg * 180 / M_PI) / 1000) - ZERO_PITCH);
      int roll = round(((head->angles.rollDeg * 180 / M_PI) / 1000) - ZERO_ROLL);

      if (yaw >= -THRESHOLD && yaw <= THRESHOLD) yaw = 0;
      if (pitch >= -THRESHOLD && pitch <= THRESHOLD) pitch = 0;
      if (roll >= -THRESHOLD && roll <= THRESHOLD) roll = 0;

			//std::cout<<"yaw: "<< yaw <<std::endl;
			//std::cout<<"pitch: "<< pitch <<std::endl;
			//std::cout<<"roll: "<< roll <<std::endl;
      std::cout << yaw << " " << pitch << " " << roll << std::endl;
      counter++;
			sleep(INTERVAL);
		}
	}
}
