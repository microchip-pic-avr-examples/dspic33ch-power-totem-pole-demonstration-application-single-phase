/*
© [2024] Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use Microchip 
    software and any derivatives exclusively with Microchip products. 
    You are responsible for complying with 3rd party license terms  
    applicable to your use of 3rd party software (including open source  
    software) that may accompany Microchip software. SOFTWARE IS ?AS IS.? 
    NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS 
    SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT,  
    MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT 
    WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY 
    KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF 
    MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE 
    FORESEEABLE. TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP?S 
    TOTAL LIABILITY ON ALL CLAIMS RELATED TO THE SOFTWARE WILL NOT 
    EXCEED AMOUNT OF FEES, IF ANY, YOU PAID DIRECTLY TO MICROCHIP FOR 
    THIS SOFTWARE.
*/

#ifndef MCC_ADD_ON_H
#define	MCC_ADD_ON_H

/*
 Section: Included Files
*/

#include <xc.h>

/** PWM Generator Definition
 
 @Summary 
   Defines the PWM generators available for PWM
 
 @Description
   This routine defines the PWM generators that are available for the module to use.
 
 Remarks:
   None
 */
typedef enum 
{
    PWM_GENERATOR_1 =  1,       
    PWM_GENERATOR_2 =  2,       
    PWM_GENERATOR_3 =  3,       
} PWM_GENERATOR;



/**
  @Summary
    Enables the specific PWM Swap.

  @Description
    This routine is used to enable the specific PWM generator selected by the argument PWM_GENERATOR.

  @Param
    genNum - PWM generator number.

  @Returns
    None
 
  @Example 
    <code>
    PWM_SwapEnable(PWM_GENERATOR_1);
    </code>
*/
inline static void PWM_SwapEnable(PWM_GENERATOR genNum)
{
    switch(genNum) { 
        case PWM_GENERATOR_1:
                PG1IOCONLbits.SWAP = 1;            
                break;       
        case PWM_GENERATOR_2:
                PG2IOCONLbits.SWAP = 1;             
                break;       
        case PWM_GENERATOR_3:
                PG3IOCONLbits.SWAP = 1;              
                break;       
         default:break;    
    }     
}

/**
  @Summary
    Disables the specific PWM Swap.

  @Description
    This routine is used to enable the specific PWM generator selected by the argument PWM_GENERATOR.

  @Param
    genNum - PWM generator number.

  @Returns
    None
 
  @Example 
    <code>
    PWM_SwapDisable(PWM_GENERATOR_1);
    </code>
*/
inline static void PWM_SwapDisable(PWM_GENERATOR genNum)
{
    switch(genNum) { 
        case PWM_GENERATOR_1:
                PG1IOCONLbits.SWAP = 0;            
                break;       
        case PWM_GENERATOR_2:
                PG2IOCONLbits.SWAP = 0;             
                break;       
        case PWM_GENERATOR_3:
                PG3IOCONLbits.SWAP = 0;              
                break;       
         default:break;    
    }     
}

/**
  @Summary
    Updates PWM Deadtime low register with the requested value for a specific PWM generator.

  @Description
    This routine is used to updates PWM Deadtime low register with the requested
    value for a specific PWM generator selected by the argument PWM_GENERATOR.

  @Param
    genNum      - PWM generator number.
    deadtimeLow - Deadtime low value.

  @Returns
    None
 
  @Example 
    <code>    
    uint16_t deadtimeLow;

    deadtimeLow = 0x01;
    
    PWM_DeadTimeLowSet(PWM_GENERATOR_1, deadtimeLow);
    </code>
*/  
inline static void PWM_DeadTimeLowSet(PWM_GENERATOR genNum,uint16_t deadtimeLow)
{
    switch(genNum) { 
        case PWM_GENERATOR_1:
                PG1DTL = deadtimeLow;              
                break;       
        case PWM_GENERATOR_2:
                PG2DTL = deadtimeLow;              
                break;       
        case PWM_GENERATOR_3:
                PG3DTL = deadtimeLow;              
                break;       
        default:break;    
    }
}

/**
  @Summary
    Updates PWM Deadtime high register with the requested value for a specific PWM generator.

  @Description
    This routine is used to updates PWM Deadtime high register with the requested
    value for a specific PWM generator selected by the argument PWM_GENERATOR.

  @Param
    genNum          - PWM generator number.
    deadtimeHigh    - Deadtime high value.

  @Returns
    None
 
  @Example 
    <code>    
    uint16_t deadtimeHigh;

    deadtimeHigh = 0x01;
    
    PWM_DeadTimeHighSet(PWM_GENERATOR_1, deadtimeHigh);
    </code>
*/  
inline static void PWM_DeadTimeHighSet(PWM_GENERATOR genNum,uint16_t deadtimeHigh)
{
    switch(genNum) { 
        case PWM_GENERATOR_1:
                PG1DTH = deadtimeHigh;              
                break;       
        case PWM_GENERATOR_2:
                PG2DTH = deadtimeHigh;              
                break;       
        case PWM_GENERATOR_3:
                PG3DTH = deadtimeHigh;              
                break;       
        default:break;    
    }
}

/**
  @Summary
    Enables the specific PWM generator.

  @Description
    This routine is used to enable the specific PWM generator selected by the argument PWM_GENERATOR.

  @Param
    genNum - PWM generator number.

  @Returns
    None
 
  @Example 
    <code>
    PWM_GeneratorEnable(PWM_GENERATOR_1);
    </code>
*/
inline static void PWM_GeneratorEnable(PWM_GENERATOR genNum)
{
    switch(genNum) { 
        case PWM_GENERATOR_1:
                PG1CONLbits.ON = 1;              
                break;       
        case PWM_GENERATOR_2:
                PG2CONLbits.ON = 1;              
                break;       
        case PWM_GENERATOR_3:
                PG3CONLbits.ON = 1;              
                break;       
        default:break;    
    }     
}

/**
  @Summary
    Enables PWM override on PWMH output for specific PWM generator.

  @Description
    This routine is used to enables PWM override on PWMH output for specific PWM generator 
    selected by the argument PWM_GENERATOR.

  @Param
    genNum - PWM generator number.

  @Returns
    None
 
  @Example 
    <code>    
    PWM_OverrideHighEnable(PWM_GENERATOR_1);
    </code>
*/  
inline static void PWM_OverrideHighEnable(PWM_GENERATOR genNum)
{
    switch(genNum) { 
        case PWM_GENERATOR_1:
                PG1IOCONLbits.OVRENH = 1;              
                break;
        case PWM_GENERATOR_2:
                PG2IOCONLbits.OVRENH = 1;              
                break;
        case PWM_GENERATOR_3:
                PG3IOCONLbits.OVRENH = 1;              
                break;
        default:break;    
    }
}

/**
  @Summary
    Enables PWM override on PWML output for specific PWM generator.

  @Description
    This routine is used to enables PWM override on PWML output for specific PWM generator
    selected by the argument PWM_GENERATOR.

  @Param
    genNum - PWM generator number.

  @Returns
    None
 
  @Example 
    <code>    
    PWM_OverrideLowEnable(PWM_GENERATOR_1);
    </code>
*/  
inline static void PWM_OverrideLowEnable(PWM_GENERATOR genNum)
{
    switch(genNum) { 
        case PWM_GENERATOR_1:
                PG1IOCONLbits.OVRENL = 1;              
                break; 
        case PWM_GENERATOR_2:
                PG2IOCONLbits.OVRENL = 1;              
                break; 
        case PWM_GENERATOR_3:
                PG3IOCONLbits.OVRENL = 1;              
                break; 
        default:break;    
    }
}

/**
  @Summary
    Used to set the PWM generator specific duty cycle register.

  @Description
    This routine is used to set the PWM generator specific duty cycle register.

  @Param
    genNum      - PWM generator number.
    dutyCycle   - PWM generator duty cycle.

  @Returns
    None
 
  @Example 
    <code>    
    uint16_t dutyCyle;
    
    dutyCycle = 0xFF;
    PWM_DutyCycleSet(PWM_GENERATOR_1, dutyCycle);
    </code>
*/
inline static void PWM_DutyCycleSet(PWM_GENERATOR genNum,uint16_t dutyCycle)
{
    switch(genNum) { 
        case PWM_GENERATOR_1:
                PG1DC = dutyCycle;              
                break;       
        case PWM_GENERATOR_2:
                PG2DC = dutyCycle;              
                break;       
        case PWM_GENERATOR_3:
                PG3DC = dutyCycle;              
                break;       
        default:break;    
    }  
}

/**
  @Summary
    Disables PWM override on PWMH output for specific PWM generator.

  @Description
    This routine is used to disable PWM override on PWMH output for specific PWM generator 
    selected by the argument PWM_GENERATOR.


  @Param
    genNum - PWM generator number.

  @Returns
    None
 
  @Example 
    <code>    
    PWM_OverrideHighDisable(PWM_GENERATOR_1);
    </code>
*/  
inline static void PWM_OverrideHighDisable(PWM_GENERATOR genNum)
{
    switch(genNum) { 
        case PWM_GENERATOR_1:
                PG1IOCONLbits.OVRENH = 0;              
                break;
        case PWM_GENERATOR_2:
                PG2IOCONLbits.OVRENH = 0;              
                break;
        case PWM_GENERATOR_3:
                PG3IOCONLbits.OVRENH = 0;              
                break;
        default:break;    
    }
}

/**
  @Summary
    Disables PWM override on PWML output for specific PWM generator.

  @Description
    This routine is used to disable PWM override on PWML output for specific PWM generator 
    selected by the argument PWM_GENERATOR.

  @Param
    genNum - PWM generator number.

  @Returns
    None
 
  @Example 
    <code>    
    PWM_OverrideLowDisable(PWM_GENERATOR_1);
    </code>
*/  
inline static void PWM_OverrideLowDisable(PWM_GENERATOR genNum)
{
    switch(genNum) { 
        case PWM_GENERATOR_1:
                PG1IOCONLbits.OVRENL = 0;              
                break;   
        case PWM_GENERATOR_2:
                PG2IOCONLbits.OVRENL = 0;              
                break;   
        case PWM_GENERATOR_3:
                PG3IOCONLbits.OVRENL = 0;              
                break;   
        default:break;    
    }
}


#endif	/* MCC_ADD_ON_H */

