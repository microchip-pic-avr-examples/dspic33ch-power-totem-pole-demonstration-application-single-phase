/**
  Summary:
  This header file was created using Microchip Digital Compensator Design Tool (DCDT)
  and includes system summary and digital compensator coefficients. This file
  should be included in the MPLAB X project directory.

  File Name:     icomp1hv_dcdt.h
  Project Name:  icomp1hv
  Date:          3/21/2024
  Time:          12:51.26

  Software License Agreement

  Copyright � 2024 Microchip Technology Inc.  All rights reserved.
  Microchip licenses to you the right to use, modify, copy and distribute
  Software only when embedded on a Microchip microcontroller or digital
  signal controller, which is integrated into your product or third party
  product (pursuant to the sublicense terms in the accompanying license
  agreement).

  You should refer to the license agreement accompanying this Software
  for additional information regarding your rights and obligations.

  SOFTWARE AND DOCUMENTATION ARE PROVIDED �AS IS� WITHOUT WARRANTY OF ANY
  KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY
  OF MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR
  PURPOSE. IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR
  OBLIGATED UNDER CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION,
  BREACH OF WARRANTY, OR OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT
  DAMAGES OR EXPENSES INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL,
  INDIRECT, PUNITIVE OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA,
  COST OF PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY
  CLAIMS BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF),
  OR OTHER SIMILAR COSTS.
 **/
#ifndef _ICOMP1HV_DCDT_DEFINES_H
#define _ICOMP1HV_DCDT_DEFINES_H

/**
  Compensator Type:  2P2Z
      Entry                Value  
    ---------            ---------
  Pole 0                 4.0000e+02 Hz
  Pole 2                 5.0000e+04 Hz
  Zero 1                 6.0000e+02 Hz
  Gain(Kdc)              10.000
  Warp                   false
  PWM Frequency          1.0000e+05
  PWM Sampling Ratio     1
  Sampling Frequency     1.0000e+05
  PWM Max Resolution     2.5000e-10
  Computational Delay    1.0000e-06
  Gate Drive Delay       5.0000e-08
  Control Output Min.    0
  Control Output Max.    19200
  Kuc Gain               3.2234e+01
  Use Kuc Gain           false


  PWM Calculations
      Name                Value  
    ---------           ---------
  Bits of Resolution    15.288
  Gain                  2.500e-05


  s-domain transfer function

               Wp0   Wp2(Wz1 + s)
  H(s) = Kdc X --- X ------------
                s    Wz1(Wp2 + s)

                  2.51e+03   3.14e+05(3.77e+03 + s)
  H(s) = 10.000 X -------- X ----------------------
                     s       3.77e+03(3.14e+05 + s)



  Digital Compensator Coefficients

  Name    Value     Normalized    Q15      Hex
  ----    -----     ----------    ---      ---
  a1      0.778     0.187         6141     0x17FD
  a2      0.222     0.053         1752     0x06D8
  b0      4.150     1.000         32764    0x7FFC
  b1      0.154     0.037         1212     0x04BC
  b2      -3.997    -0.963        -31552   0x84C0


  z-domain transfer function

         u(z)  B0 + B1z^(-1) + B2z^(-2)
  H(z) = --- = ------------------------
         e(z)  A0 - A1z^(-1) - A2z^(-2)

          (4.150) + (0.154)z^(-1) + (-3.997)z^(-2)
  H(z) = ---------------------------------------------
          1 - (0.778)z^(-1) - (0.222)z^(-2)

**/


// Compensator Coefficient Defines
#define ICOMP1HV_COMP_2P2Z_COEFF_A1      0x17FD
#define ICOMP1HV_COMP_2P2Z_COEFF_A2      0x06D8
#define ICOMP1HV_COMP_2P2Z_COEFF_B0      0x7FFC
#define ICOMP1HV_COMP_2P2Z_COEFF_B1      0x04BC
#define ICOMP1HV_COMP_2P2Z_COEFF_B2      0x84C0
#define ICOMP1HV_COMP_2P2Z_POSTSCALER    0x4268
#define ICOMP1HV_COMP_2P2Z_POSTSHIFT     0xFFFD
#define ICOMP1HV_COMP_2P2Z_PRESHIFT      0x0000


// Compensator Clamp Limits
#define ICOMP1HV_COMP_2P2Z_MIN_CLAMP    0x0000
#define ICOMP1HV_COMP_2P2Z_MAX_CLAMP    38000


#endif
