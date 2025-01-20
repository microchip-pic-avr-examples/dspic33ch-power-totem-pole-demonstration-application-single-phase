/**
 * PWM Generated Driver Source File
 * 
 * @file      pwm.c
 * 
 * @ingroup   pwmhsdriver
 * 
 * @brief     This is the generated driver source file for PWM driver
 *
 * @skipline @version   Firmware Driver Version 1.1.6
 *
 * @skipline @version   PLIB Version 2.4.3
 *
 * @skipline  Device : dsPIC33CH512MP506S1
*/

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

// Section: Included Files


#include "../pwm.h"
#include "../../system/clock.h"

// Section: File specific functions

static void (*PWM_GeneratorEOCEventHandler)(enum PWM_GENERATOR genNum) = NULL;


// Section: Driver Interface

const struct PWM_HS_INTERFACE PWM_HS = {
    .Initialize                     = &PWM_Initialize,
    .Deinitialize                   = &PWM_Deinitialize,
    .Disable                        = &PWM_Disable,
    .Enable                         = &PWM_Enable,
    .ModeSet                        = &PWM_ModeSet,
    .MasterPeriodSet                = &PWM_MasterPeriodSet,
    .MasterDutyCycleSet             = &PWM_MasterDutyCycleSet,
    .PeriodSet                      = &PWM_PeriodSet,
    .DutyCycleSet                   = &PWM_DutyCycleSet,
    .PhaseSelect                    = &PWM_PhaseSelect,
    .PhaseSet                       = &PWM_PhaseSet,
    .OverrideDataSet                = &PWM_OverrideDataSet,
    .OverrideDataHighSet            = &PWM_OverrideDataHighSet,
    .OverrideDataLowSet             = &PWM_OverrideDataLowSet,
    .OverrideDataGet                = &PWM_OverrideDataGet,
    .OverrideHighEnable             = &PWM_OverrideHighEnable,
    .OverrideLowEnable              = &PWM_OverrideLowEnable,
    .OverrideHighDisable            = &PWM_OverrideHighDisable,
    .OverrideLowDisable             = &PWM_OverrideLowDisable,
    .DeadTimeLowSet                 = &PWM_DeadTimeLowSet,
    .DeadTimeHighSet                = &PWM_DeadTimeHighSet,
    .DeadTimeSet                    = &PWM_DeadTimeSet,
    .TriggerCompareValueSet         = &PWM_TriggerCompareValueSet,
    .GeneratorInterruptEnable       = &PWM_GeneratorInterruptEnable,
    .GeneratorInterruptDisable      = &PWM_GeneratorInterruptDisable,
    .GeneratorEventStatusGet        = &PWM_GeneratorEventStatusGet,
    .GeneratorEventStatusClear      = &PWM_GeneratorEventStatusClear,

    .SpecialEventPrimaryCompareValueSet     = NULL,    
    .SpecialEventTriggerInterruptFlagClear  = NULL,
    .SpecialEventPrimaryInterruptEnable     = NULL,
    .SpecialEventPrimaryInterruptDisable    = NULL,
    .FaultModeLatchDisable                  = NULL,
    .FaultModeLatchEnable                   = NULL,
    .MasterPhaseSet                         = &PWM_MasterPhaseSet,
    .GeneratorDisable                       = &PWM_GeneratorDisable,
    .GeneratorEnable                        = &PWM_GeneratorEnable,
    .TriggerACompareValueSet                = &PWM_TriggerACompareValueSet,
    .TriggerBCompareValueSet                = &PWM_TriggerBCompareValueSet,
    .TriggerCCompareValueSet                = &PWM_TriggerCCompareValueSet,
    .SoftwareUpdateRequest                  = &PWM_SoftwareUpdateRequest,
    .SoftwareUpdatePending                  = &PWM_SoftwareUpdatePending,
    .FaultModeLatchClear                    = &PWM_FaultModeLatchClear,
    .Trigger1Enable                         = &PWM_Trigger1Enable,
    .Trigger1Disable                        = &PWM_Trigger1Disable,
    .Trigger2Enable                         = &PWM_Trigger2Enable,
    .Trigger2Disable                        = &PWM_Trigger2Disable,
    
    .GeneratorEOCEventCallbackRegister = &PWM_GeneratorEOCEventCallbackRegister,
    
    
    .GeneratorTasks         = &PWM_GeneratorTasks,
    .CommonEventTasks       = NULL,
};

// Section: PWM Module APIs

void PWM_Initialize (void)
{
    // MODSEL Independent Edge; TRGCNT 1; CLKSEL Master clock; ON disabled; HREN enabled; 
    PG1CONL = 0x88;
    // MODSEL Independent Edge; TRGCNT 1; CLKSEL Master clock; ON disabled; HREN enabled; 
    PG2CONL = 0x88;
    // 
    PG1CAP = 0x0;
    // PCIGT disabled; TQPS Not inverted; SWPCI Drives '0'; BPEN disabled; TQSS None; SWPCIM PCI acceptance logic; BPSEL PWM Generator 1; ACP Level-sensitive; 
    PG1CLPCIH = 0x0;
    // PSS Internally connected to '1'b0; PPS Not inverted; SWTERM disabled; PSYNC disabled; TERM Manual Terminate; AQPS Not inverted; AQSS None; TSYNCDIS PWM EOC; 
    PG1CLPCIL = 0x0;
    // MSTEN disabled; TRIGMOD Single trigger mode; SOCS Self-trigger; UPDMOD SOC update; MPHSEL disabled; MPERSEL enabled; MDCSEL disabled; 
    PG1CONH = 0x4000;
    // DC1 0; 
    PG1DC = 0x0;
    // DCA1 0x0; 
    PG1DCA = 0x0;
    // DTH1 0; 
    PG1DTH = 0x0;
    // DTL1 0; 
    PG1DTL = 0x0;
    // ADTR2EN1 disabled; IEVTSEL EOC; SIEN disabled; FFIEN disabled; ADTR1OFS None; CLIEN disabled; FLTIEN disabled; ADTR2EN2 disabled; ADTR2EN3 disabled; 
    PG1EVTH = 0x0;
    // UPDTRG Duty Cycle; ADTR1PS 1:1; PGTRGSEL Trigger C compare event; ADTR1EN3 disabled; ADTR1EN1 enabled; ADTR1EN2 disabled; 
    PG1EVTL = 0x10B;
    // PCIGT disabled; TQPS Not inverted; SWPCI Drives '0'; BPEN disabled; TQSS None; SWPCIM PCI acceptance logic; BPSEL PWM Generator 1; ACP Level-sensitive; 
    PG1FFPCIH = 0x0;
    // PSS Internally connected to '1'b0; PPS Not inverted; SWTERM disabled; PSYNC disabled; TERM Manual Terminate; AQPS Not inverted; AQSS None; TSYNCDIS PWM EOC; 
    PG1FFPCIL = 0x0;
    // TQPS Not inverted; LATMOD disabled; SWPCI Drives '0'; BPEN disabled; TQSS None; SWPCIM PCI acceptance logic; BPSEL PWM Generator 1; ACP Level-sensitive; 
    PG1FPCIH = 0x0;
    // PSS Internally connected to '1'b0; PPS Not inverted; SWTERM disabled; PSYNC disabled; TERM Manual Terminate; AQPS Not inverted; AQSS None; TSYNCDIS PWM EOC; 
    PG1FPCIL = 0x0;
    // PENL enabled; DTCMPSEL PCI Sync Logic; PMOD Complementary; POLL Active-high; PENH enabled; CAPSRC Software; POLH Active-high; 
    PG1IOCONH = 0xC;
    // FLTDAT 0; DBDAT 0x0; SWAP disabled; OVRENH enabled; OVRENL enabled; OSYNC User output overrides occur when specified by UPDMOD bits; CLMOD disabled; FFDAT 0x0; CLDAT 0x0; OVRDAT 0; 
    PG1IOCONL = 0x3200;
    // PWMPCI PWM1; PLR disabled; PLF disabled; PHR disabled; PHF disabled; 
    PG1LEBH = 0x0;
    // LEB 0x0; 
    PG1LEBL = 0x0;
    // PER1 39992; 
    PG1PER = 0x9C38;
    // PHASE1 0; 
    PG1PHASE = 0x0;
    // PCIGT disabled; TQPS Not inverted; SWPCI Drives '0'; BPEN disabled; TQSS None; SWPCIM PCI acceptance logic; BPSEL PWM Generator 1; ACP Level-sensitive; 
    PG1SPCIH = 0x0;
    // PSS Internally connected to '1'b0; PPS Not inverted; SWTERM disabled; PSYNC disabled; TERM Manual Terminate; AQPS Not inverted; AQSS None; TSYNCDIS PWM EOC; 
    PG1SPCIL = 0x0;
    // TRSET disabled; UPDREQ disabled; CLEVT disabled; TRCLR disabled; CAP disabled; SEVT disabled; FFEVT disabled; UPDATE disabled; FLTEVT disabled; 
    PG1STAT = 0x0;
    // TRIGA1 0; 
    PG1TRIGA = 0x0;
    // TRIGB1 0; 
    PG1TRIGB = 0x0;
    // TRIGC1 0; 
    PG1TRIGC = 0x0;
    // 
    PG2CAP = 0x0;
    // PCIGT disabled; TQPS Not inverted; SWPCI Drives '0'; BPEN disabled; TQSS None; SWPCIM PCI acceptance logic; BPSEL PWM Generator 1; ACP Level-sensitive; 
    PG2CLPCIH = 0x0;
    // PSS Internally connected to '1'b0; PPS Not inverted; SWTERM disabled; PSYNC disabled; TERM Manual Terminate; AQPS Not inverted; AQSS None; TSYNCDIS PWM EOC; 
    PG2CLPCIL = 0x0;
    // MSTEN disabled; TRIGMOD Single trigger mode; SOCS Trigger output selected by PG1 or PG5; UPDMOD SOC update; MPHSEL disabled; MPERSEL enabled; MDCSEL disabled; 
    PG2CONH = 0x4001;
    // DC2 0; 
    PG2DC = 0x0;
    // DCA2 0x0; 
    PG2DCA = 0x0;
    // DTH2 0; 
    PG2DTH = 0x0;
    // DTL2 0; 
    PG2DTL = 0x0;
    // ADTR2EN1 disabled; IEVTSEL EOC; SIEN disabled; FFIEN disabled; ADTR1OFS None; CLIEN disabled; FLTIEN disabled; ADTR2EN2 disabled; ADTR2EN3 disabled; 
    PG2EVTH = 0x0;
    // UPDTRG Duty Cycle; ADTR1PS 1:1; PGTRGSEL EOC event; ADTR1EN3 disabled; ADTR1EN1 enabled; ADTR1EN2 disabled; 
    PG2EVTL = 0x108;
    // PCIGT disabled; TQPS Not inverted; SWPCI Drives '0'; BPEN disabled; TQSS None; SWPCIM PCI acceptance logic; BPSEL PWM Generator 1; ACP Level-sensitive; 
    PG2FFPCIH = 0x0;
    // PSS Internally connected to '1'b0; PPS Not inverted; SWTERM disabled; PSYNC disabled; TERM Manual Terminate; AQPS Not inverted; AQSS None; TSYNCDIS PWM EOC; 
    PG2FFPCIL = 0x0;
    // TQPS Not inverted; LATMOD disabled; SWPCI Drives '0'; BPEN disabled; TQSS None; SWPCIM PCI acceptance logic; BPSEL PWM Generator 1; ACP Level-sensitive; 
    PG2FPCIH = 0x0;
    // PSS Internally connected to '1'b0; PPS Not inverted; SWTERM disabled; PSYNC disabled; TERM Manual Terminate; AQPS Not inverted; AQSS None; TSYNCDIS PWM EOC; 
    PG2FPCIL = 0x0;
    // PENL enabled; DTCMPSEL PCI Sync Logic; PMOD Complementary; POLL Active-high; PENH enabled; CAPSRC Software; POLH Active-high; 
    PG2IOCONH = 0xC;
    // FLTDAT 0; DBDAT 0x0; SWAP disabled; OVRENH enabled; OVRENL enabled; OSYNC User output overrides occur when specified by UPDMOD bits; CLMOD disabled; FFDAT 0x0; CLDAT 0x0; OVRDAT 0; 
    PG2IOCONL = 0x3200;
    // PWMPCI PWM1; PLR disabled; PLF disabled; PHR disabled; PHF disabled; 
    PG2LEBH = 0x0;
    // LEB 0x0; 
    PG2LEBL = 0x0;
    // PER2 39992; 
    PG2PER = 0x9C38;
    // PHASE2 0; 
    PG2PHASE = 0x0;
    // PCIGT disabled; TQPS Not inverted; SWPCI Drives '0'; BPEN disabled; TQSS None; SWPCIM PCI acceptance logic; BPSEL PWM Generator 1; ACP Level-sensitive; 
    PG2SPCIH = 0x0;
    // PSS Internally connected to '1'b0; PPS Not inverted; SWTERM disabled; PSYNC disabled; TERM Manual Terminate; AQPS Not inverted; AQSS None; TSYNCDIS PWM EOC; 
    PG2SPCIL = 0x0;
    // TRSET disabled; UPDREQ disabled; CLEVT disabled; TRCLR disabled; CAP disabled; SEVT disabled; FFEVT disabled; UPDATE disabled; FLTEVT disabled; 
    PG2STAT = 0x0;
    // TRIGA2 0; 
    PG2TRIGA = 0x0;
    // TRIGB2 0; 
    PG2TRIGB = 0x0;
    // TRIGC2 0; 
    PG2TRIGC = 0x0;
    // CTB3EN disabled; CTB2EN disabled; CTB1EN disabled; CTB4EN disabled; CTB8EN disabled; CTB7EN disabled; CTB6EN disabled; CTB5EN disabled; 
    CMBTRIGH = 0x0;
    // CTA1EN disabled; CTA2EN disabled; CTA3EN disabled; CTA4EN disabled; CTA7EN disabled; CTA8EN disabled; CTA5EN disabled; CTA6EN disabled; 
    CMBTRIGL = 0x0;
    // FSCL 0x0; 
    FSCL = 0x0;
    // FSMINPER 0x0; 
    FSMINPER = 0x0;
    // LFSR 0x0; 
    LFSR = 0x0;
    // PWMLFA PWMS1 or PWMS2;; S1APOL Positive logic; S2APOL Positive logic; PWMLFAD No Assignment; PWMS1A PWM1H; PWMS2A PWM1H; 
    LOGCONA = 0x0;
    // PWMLFB PWMS1 | PWMS2; S2BPOL Positive logic; PWMLFBD No Assignment; S1BPOL Positive logic; PWMS2B PWM1H; PWMS1B PWM1H; 
    LOGCONB = 0x0;
    // PWMLFC PWMS1 | PWMS2; PWMLFCD No Assignment; S2CPOL Positive logic; S1CPOL Positive logic; PWMS1C PWM1H; PWMS2C PWM1H; 
    LOGCONC = 0x0;
    // PWMS1D PWM1H; S1DPOL Positive logic; PWMLFD PWMS1 | PWMS2; PWMLFDD No Assignment; S2DPOL Positive logic; PWMS2D PWM1H; 
    LOGCOND = 0x0;
    // PWMS1E PWM1H; PWMS2E PWM1H; S1EPOL Positive logic; PWMLFE PWMS1 | PWMS2; S2EPOL Positive logic; PWMLFED No Assignment; 
    LOGCONE = 0x0;
    // S1FPOL Positive logic; PWMS2F PWM1H; PWMS1F PWM1H; S2FPOL Positive logic; PWMLFFD No Assignment; PWMLFF PWMS1 | PWMS2; 
    LOGCONF = 0x0;
    // MDC 0; 
    MDC = 0x0;
    // MPER 39992; 
    MPER = 0x9C38;
    // MPHASE 0x0; 
    MPHASE = 0x0;
    // MCLKSEL AFPLLO; LOCK disabled; DIVSEL 1:2; HRERR disabled; 
    PCLKCON = 0x3;
    // EVTASEL PGTRGSEL bits; EVTASYNC Not synchronized; EVTAPOL Active-high; EVTAPGS PG1; EVTASTRD Stretched to 8 PWM clock cycles minimum; EVT1OEN disabled; 
    PWMEVTA = 0x0;
    // EVTBPGS PG1; EVTBSYNC Not synchronized; EVTBPOL Active-high; EVTBSEL PGTRGSEL bits; EVTBSTRD Stretched to 8 PWM clock cycles minimum; EVT2OEN disabled; 
    PWMEVTB = 0x0;
    // EVTCPGS PG1; EVTCPOL Active-high; EVTCSEL PGTRGSEL bits; EVTCSTRD Stretched to 8 PWM clock cycles minimum; EVTCSYNC Not synchronized; EVT3OEN disabled; 
    PWMEVTC = 0x0;
    // EVT4OEN disabled; EVTDSTRD Stretched to 8 PWM clock cycles minimum; EVTDPOL Active-high; EVTDPGS PG1; EVTDSEL PGTRGSEL bits; EVTDSYNC Not synchronized; 
    PWMEVTD = 0x0;
    // EVT5OEN disabled; EVTEPOL Active-high; EVTEPGS PG1; EVTESTRD Stretched to 8 PWM clock cycles minimum; EVTESEL PGTRGSEL bits; EVTESYNC Not synchronized; 
    PWMEVTE = 0x0;
    // EVTFPOL Active-high; EVTFPGS PG1; EVTFSTRD Stretched to 8 PWM clock cycles minimum; EVTFSEL PGTRGSEL bits; EVT6OEN disabled; EVTFSYNC Not synchronized; 
    PWMEVTF = 0x0;
    
    PWM_GeneratorEOCEventCallbackRegister(&PWM_GeneratorEOCEventCallback);
    
    

    //Wait until AUX PLL clock is locked
    while(!CLOCK_AuxPllLockStatusGet());

}

void PWM_Deinitialize (void)
{
    PWM_Disable();
    
    PG1CONL = 0x0;
    PG2CONL = 0x0;
    
    PG1CAP = 0x0;
    PG1CLPCIH = 0x0;
    PG1CLPCIL = 0x0;
    PG1CONH = 0x0;
    PG1DC = 0x0;
    PG1DCA = 0x0;
    PG1DTH = 0x0;
    PG1DTL = 0x0;
    PG1EVTH = 0x0;
    PG1EVTL = 0x0;
    PG1FFPCIH = 0x0;
    PG1FFPCIL = 0x0;
    PG1FPCIH = 0x0;
    PG1FPCIL = 0x0;
    PG1IOCONH = 0x0;
    PG1IOCONL = 0x0;
    PG1LEBH = 0x0;
    PG1LEBL = 0x0;
    PG1PER = 0x0;
    PG1PHASE = 0x0;
    PG1SPCIH = 0x0;
    PG1SPCIL = 0x0;
    PG1STAT = 0x0;
    PG1TRIGA = 0x0;
    PG1TRIGB = 0x0;
    PG1TRIGC = 0x0;
    PG2CAP = 0x0;
    PG2CLPCIH = 0x0;
    PG2CLPCIL = 0x0;
    PG2CONH = 0x0;
    PG2DC = 0x0;
    PG2DCA = 0x0;
    PG2DTH = 0x0;
    PG2DTL = 0x0;
    PG2EVTH = 0x0;
    PG2EVTL = 0x0;
    PG2FFPCIH = 0x0;
    PG2FFPCIL = 0x0;
    PG2FPCIH = 0x0;
    PG2FPCIL = 0x0;
    PG2IOCONH = 0x0;
    PG2IOCONL = 0x0;
    PG2LEBH = 0x0;
    PG2LEBL = 0x0;
    PG2PER = 0x0;
    PG2PHASE = 0x0;
    PG2SPCIH = 0x0;
    PG2SPCIL = 0x0;
    PG2STAT = 0x0;
    PG2TRIGA = 0x0;
    PG2TRIGB = 0x0;
    PG2TRIGC = 0x0;
    CMBTRIGH = 0x0;
    CMBTRIGL = 0x0;
    FSCL = 0x0;
    FSMINPER = 0x0;
    LFSR = 0x0;
    LOGCONA = 0x0;
    LOGCONB = 0x0;
    LOGCONC = 0x0;
    LOGCOND = 0x0;
    LOGCONE = 0x0;
    LOGCONF = 0x0;
    MDC = 0x0;
    MPER = 0x0;
    MPHASE = 0x0;
    PCLKCON = 0x0;
    PWMEVTA = 0x0;
    PWMEVTB = 0x0;
    PWMEVTC = 0x0;
    PWMEVTD = 0x0;
    PWMEVTE = 0x0;
    PWMEVTF = 0x0;
}

void PWM_GeneratorEOCEventCallbackRegister(void (*callback)(enum PWM_GENERATOR genNum))
{
    if(NULL != callback)
    {
        PWM_GeneratorEOCEventHandler = callback;
    }
}

void __attribute__ ((weak)) PWM_GeneratorEOCEventCallback ( enum PWM_GENERATOR genNum )
{ 

} 


void PWM_GeneratorTasks (enum PWM_GENERATOR intGen)
{
    switch(intGen)
    {
        case PWM_GENERATOR_1:
            if(IFS4bits.PWM1IF == 1)
            {
                if(NULL != PWM_GeneratorEOCEventHandler)
                {
                    (*PWM_GeneratorEOCEventHandler)(intGen);
                }
                IFS4bits.PWM1IF = 0;
            }
            break;
        case PWM_GENERATOR_2:
            if(IFS4bits.PWM2IF == 1)
            {
                if(NULL != PWM_GeneratorEOCEventHandler)
                {
                    (*PWM_GeneratorEOCEventHandler)(intGen);
                }
                IFS4bits.PWM2IF = 0;
            }
            break;
        default:
            break;
    }
}

/**
 End of File
*/
