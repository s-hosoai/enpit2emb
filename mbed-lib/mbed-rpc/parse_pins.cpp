/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "port_api.h"

namespace mbed {

PinName parse_pins(const char *str) {
#if defined(TARGET_LPC1768) || defined(TARGET_LPC11U24) || defined(TARGET_LPC2368)
    static const PinName pin_names[] = {p5, p6, p7, p8, p9, p10, p11, p12, p13, p14
                                , p15, p16, p17, p18, p19, p20, p21, p22, p23
                                , p24, p25, p26, p27, p28, p29, p30};
#elif defined(TARGET_LPC1114)
    static const PinName pin_names[] = {dp1, dp2, dp4, dp5, dp6, dp9, dp10, dp11
                                , dp13, dp14, dp15, dp16, dp17, dp18, dp23
                                , dp24, dp25, dp26, dp27, dp28};
#elif defined(TARGET_LPC4088)
    static const PinName pin_names[] = {NC, NC, NC, NC, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14
                                , p15, p16, p17, p18, p19, p20, NC, NC, p23
                                , p24, p25, p26, p27, p28, p29, p30, p31, p32
                                , p33, p34, NC, NC, p37, p38, p39, NC, NC, NC, NC, NC, NC, NC};
#elif defined(TARGET_LPC4088_DM)
    static const PinName pin_names[] = {p1, p2, p3, p4, NC, NC, p7, p8, p9, p10, p11, p12, p13, p14
                                , p15, p16, p17, p18, p19, p20, p21, p22, p23
                                , p24, p25, p26, NC, NC, p29, p30, NC, NC
                                , NC, NC, NC, NC, NC, NC, NC, NC, p41, p42, p43, p44, p45, p46};
#endif

#if defined(TARGET_LPC1768) || defined(TARGET_LPC11U24) || defined(TARGET_LPC2368) || defined(TARGET_LPC812) || defined(TARGET_LPC4088) || defined(TARGET_LPC4088_DM) || defined(TARGET_LPC1114) //mituhiromatuura || defined(TARGET_RZ_A1H) || defined(TARGET_VK_RZ_A1H)
    if (str[0] == 'P') {              // Pn_n
        uint32_t port = str[1] - '0';
        uint32_t pin  = str[3] - '0'; // Pn_n
        uint32_t pin2 = str[4] - '0'; // Pn_nn
        if (pin2 <= 9) {
            pin = pin * 10 + pin2;
        }
        return port_pin((PortName)port, pin);

#elif defined(TARGET_KL25Z) || defined(TARGET_KL27Z) || defined(TARGET_KL05Z) || defined(TARGET_KL46Z) || defined(TARGET_K22F) || defined(TARGET_K64F)
    if (str[0] == 'P' && str[1] == 'T') {   // PTxn
        uint32_t port = str[2] - 'A';
        uint32_t pin  = str[3] - '0';       // PTxn
        uint32_t pin2 = str[4] - '0';       // PTxnn

        if (pin2 <= 9) {
            pin = pin * 10 + pin2;
        }
        return port_pin((PortName)port, pin);

#elif defined(TARGET_NUCLEO_F072RB) || defined(TARGET_NUCLEO_F303K8) || defined(TARGET_NUCLEO_F411RE)
    if (str[0] == 'P') {   // PX_XX e.g.PA_2 PC_15
        uint32_t port = str[1] - 'A';
        uint32_t pin  = str[3] - '0';       
        uint32_t pin2 = str[4] - '0';       

        if (pin2 <= 9) {
            pin = pin * 10 + pin2;
        }
        return port_pin((PortName)port, pin);

#elif defined(TARGET_RZ_A1H) //mituhiromatuura
#if 0
    if (str[0] == 'P') {              // Pn_n
		static const char pin_names[][8] = {
			"P0_0", "P0_1", "P0_2", "P0_3", "P0_4", "P0_5", "P0_6", "P0_7", "P0_8", "P0_9", "P0_10", "P0_11", "P0_12", "P0_13", "P0_14", "P0_15",
			"P1_0", "P1_1", "P1_2", "P1_3", "P1_4", "P1_5", "P1_6", "P1_7", "P1_8", "P1_9", "P1_10", "P1_11", "P1_12", "P1_13", "P1_14", "P1_15", 
			"P2_0", "P2_1", "P2_2", "P2_3", "P2_4", "P2_5", "P2_6", "P2_7", "P2_8", "P2_9", "P2_10", "P2_11", "P2_12", "P2_13", "P2_14", "P2_15", 
			"P3_0", "P3_1", "P3_2", "P3_3", "P3_4", "P3_5", "P3_6", "P3_7", "P3_8", "P3_9", "P3_10", "P3_11", "P3_12", "P3_13", "P3_14", "P3_15", 
			"P4_0", "P4_1", "P4_2", "P4_3", "P4_4", "P4_5", "P4_6", "P4_7", "P4_8", "P4_9", "P4_10", "P4_11", "P4_12", "P4_13", "P4_14", "P4_15", 
			"P5_0", "P5_1", "P5_2", "P5_3", "P5_4", "P5_5", "P5_6", "P5_7", "P5_8", "P5_9", "P5_10", "P5_11", "P5_12", "P5_13", "P5_14", "P5_15", 
			"P6_0", "P6_1", "P6_2", "P6_3", "P6_4", "P6_5", "P6_6", "P6_7", "P6_8", "P6_9", "P6_10", "P6_11", "P6_12", "P6_13", "P6_14", "P6_15", 
			"P7_0", "P7_1", "P7_2", "P7_3", "P7_4", "P7_5", "P7_6", "P7_7", "P7_8", "P7_9", "P7_10", "P7_11", "P7_12", "P7_13", "P7_14", "P7_15", 
			"P8_0", "P8_1", "P8_2", "P8_3", "P8_4", "P8_5", "P8_6", "P8_7", "P8_8", "P8_9", "P8_10", "P8_11", "P8_12", "P8_13", "P8_14", "P8_15", 
			"P9_0", "P9_1", "P9_2", "P9_3", "P9_4", "P9_5", "P9_6", "P9_7", "P9_8", "P9_9", "P9_10", "P9_11", "P9_12", "P9_13", "P9_14", "P9_15", 
			"P10_0","P10_1","P10_2","P10_3","P10_4","P10_5","P10_6","P10_7","P10_8","P10_9","P10_10","P10_11","P10_12","P10_13","P10_14","P10_15",
			"P11_0","P11_1","P11_2","P11_3","P11_4","P11_5","P11_6","P11_7","P11_8","P11_9","P11_10","P11_11","P11_12","P11_13","P11_14","P11_15",
		};
		uint32_t pin, i;

		for(pin = 0; pin < 12 * 16; pin++) {
			i = 0;
			do {
				if(str[i] != pin_names[pin][i])break;
				i++;
			}while(str[i] && pin_names[pin][i]);
			if(!str[i] && !pin_names[pin][i])return (PinName)pin;
		}
#else
	if((str[0] == 'P')
	&& (str[1] >= '0')
	&& (str[1] <= '9')){
		// Pn_
		if((str[2] == '_')
		&& (str[3] >= '0')
		&& (str[3] <= '9')){
			// Pn_n
			if(str[4] == '\0'){
				return (PinName)((str[1] - '0') * 16 + (str[3] - '0'));
			}
			// Pn_nn
			if((str[4] >= '0')
			&& (str[4] <= '5')
			&& (str[5] == '\0')){
				return (PinName)((str[1] - '0') * 16 + (str[3] - '0') * 10 + (str[4] - '0'));
			}
		}
		//Pnn_
		if((str[2] >= '0')
		&& (str[2] <= '5')
		&& (str[3] == '_')){
			// Pnn_n
			if((str[4] >= '0')
			&& (str[4] <= '9')){
				if(str[5] == '\0'){
					return (PinName)(((str[1] - '0') * 10 + (str[2] - '0')) * 16 + (str[4] - '0'));
				}
				// Pnn_nn
				if((str[5] >= '0')
				&& (str[5] <= '5')
				&& (str[6] == '\0')){
					return (PinName)(((str[1] - '0') * 10 + (str[2] - '0')) * 16 + (str[4] - '0') * 10 + (str[5] - '0'));
				}
			}
		}
#endif
#endif



#if defined(TARGET_LPC1768) || defined(TARGET_LPC11U24) || defined(TARGET_LPC2368)
    } else if (str[0] == 'p') {       // pn
        uint32_t pin  = str[1] - '0'; // pn
        uint32_t pin2 = str[2] - '0'; // pnn
        if (pin2 <= 9) {
            pin = pin * 10 + pin2;
        }
        if (pin < 5 || pin > 30) {
            return NC;
        }
        return pin_names[pin - 5];
#elif defined(TARGET_LPC4088) || defined(TARGET_LPC4088_DM)
    } else if (str[0] == 'p') {       // pn
        uint32_t pin  = str[1] - '0'; // pn
        uint32_t pin2 = str[2] - '0'; // pnn
        if (pin2 <= 9) {
            pin = pin * 10 + pin2;
        }
        if (pin < 1 || pin > 46) {
            return NC;
        }
        return pin_names[pin - 1];
#endif
    } else if (str[0] == 'L') {  // LEDn
        switch (str[3]) {
            case '1' : return LED1;
            case '2' : return LED2;
            case '3' : return LED3;
            case '4' : return LED4;
        }

    } else if (str[0] == 'U') {  // USB?X
        switch (str[3]) {
            case 'T' : return USBTX;
            case 'R' : return USBRX;
        }
    }

    return NC;
}
}


