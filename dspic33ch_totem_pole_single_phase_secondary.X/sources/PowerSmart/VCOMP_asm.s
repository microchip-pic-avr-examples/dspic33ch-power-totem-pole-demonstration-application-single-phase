; **********************************************************************************
;  SDK Version: PowerSmart Digital Control Library Designer v1.9.15.709
;  CGS Version: Code Generator Script v3.0.11 (01/06/2022)
;  Author:      M91114
;  Date/Time:   06/30/2023 09:45:11
; **********************************************************************************
;  2P2Z Control Library File (Single Output Factor Scaling Mode)
; **********************************************************************************
    
;------------------------------------------------------------------------------
;file start
    .nolist                                 ; (no external dependencies)
    .list                                   ; list of all external dependencies
    
;------------------------------------------------------------------------------
;local inclusions.
    .section .data                          ; place constant data in the data section
    
;------------------------------------------------------------------------------
; Define status flags bit positions
    .equ NPNZ16_STATUS_ENABLED,      15     ; bit position of the ENABLE control bit
    .equ NPNZ16_STATUS_INVERT_INPUT, 14     ; bit position of the INVERT_INPUT control bit
    .equ NPNZ16_STATUS_SWAP_SOURCE,  13     ; bit position of the SWAP_SOURCE control bit
    .equ NPNZ16_STATUS_SWAP_TARGET,  12     ; bit position of the SWAP_TARGET control bit
    .equ NPNZ16_STATUS_AGC_ENABLED,  11     ; bit position of the AGC_ENABLED control bit
    .equ NPNZ16_STATUS_USAT,         1      ; bit position of the UPPER_SATURATION_FLAG status bit
    .equ NPNZ16_STATUS_LSAT,         0      ; bit position of the LOWER_SATURATION_FLAG status bit
    
;------------------------------------------------------------------------------
; NPNZ16b_s data structure address offset declarations for data structure addressing
    .equ Status,                                0 ; controller object status word at address-offset = 0
    .equ ptrSourceRegister,                     2 ; parameter group Ports.Source: pointer to source memory address
    .equ SourceNormShift,                       4 ; parameter group Ports.Source: bit-shift scaler of normalization factor
    .equ SourceNormFactor,                      6 ; parameter group Ports.Source: Q15 normalization factor
    .equ SourceOffset,                          8 ; parameter group Ports.Source: value of source input signal/value offset
    .equ ptrAltSourceRegister,                 10 ; parameter group Ports.AltSource: pointer to alternate source memory address
    .equ AltSourceNormShift,                   12 ; parameter group Ports.AltSource: bit-shift scaler of normalization factor
    .equ AltSourceNormFactor,                  14 ; parameter group Ports.AltSource: Q15 normalization factor
    .equ AltSourceOffset,                      16 ; parameter group Ports.AltSource: value of alternate source input signal/value offset
    .equ ptrTargetRegister,                    18 ; parameter group Ports.Target: pointer to target memory address
    .equ TargetNormShift,                      20 ; parameter group Ports.Target: bit-shift scaler of normalization factor
    .equ TargetNormFactor,                     22 ; parameter group Ports.Target: Q15 normalization factor
    .equ TargetOffset,                         24 ; parameter group Ports.Target: value of target output signal/value offset
    .equ ptrAltTargetRegister,                 26 ; parameter group Ports.AltTarget: pointer to alternate target memory address
    .equ AltTargetNormShift,                   28 ; parameter group Ports.AltTarget: bit-shift scaler of normalization factor
    .equ AltTargetNormFactor,                  30 ; parameter group Ports.AltTarget: Q15 normalization factor
    .equ AltTargetOffset,                      32 ; parameter group Ports.AltTarget: value of alternate target output signal/value offset
    .equ ptrControlReference,                  34 ; parameter group Ports.ConrolReference: pointer to control reference variable/register memory address
    .equ ptrACoefficients,                     36 ; parameter group Filter: pointer to A-coefficients array start address
    .equ ptrBCoefficients,                     38 ; parameter group Filter: pointer to B-coefficients array start address
    .equ ptrControlHistory,                    40 ; parameter group Filter: pointer to control history array start address
    .equ ptrErrorHistory,                      42 ; parameter group Filter: pointer to error history array start address
    .equ ACoefficientsArraySize,               44 ; parameter group Filter: size of the A-coefficients array
    .equ BCoefficientsArraySize,               46 ; parameter group Filter: size of the B-coefficients array
    .equ ControlHistoryArraySize,              48 ; parameter group Filter: size of the control history array
    .equ ErrorHistoryArraySize,                50 ; parameter group Filter: size of the error history array
    .equ normPreShift,                         52 ; parameter group Filter: value of input value normalization bit-shift scaler
    .equ normPostShiftA,                       54 ; parameter group Filter: value of A-term normalization bit-shift scaler
    .equ normPostShiftB,                       56 ; parameter group Filter: value of B-term normalization bit-shift scaler
    .equ normPostScaler,                       58 ; parameter group Filter: control loop output normalization factor
    .equ PTermScaler,                          60 ; parameter group Filter: P-Term coefficient scaler
    .equ PTermFactor,                          62 ; parameter group Filter: P-Term coefficient fractional factor
    .equ AgcScaler,                            64 ; parameter group GainControl: bit-shift scaler of Adaptive Gain Control Modulation factor
    .equ AgcFactor,                            66 ; parameter group GainControl: Q15 value of Adaptive Gain Control Modulation factor
    .equ AgcMedian,                            68 ; parameter group GainControl: Q15 value of Adaptive Gain Control Modulation nominal operating point
    .equ ptrAgcObserverFunction,               70 ; parameter group GainControl: function pointer to observer function updating the AGC modulation factor
    .equ MinOutput,                            72 ; parameter group Limits: minimum clamping value of primary control output
    .equ MaxOutput,                            74 ; parameter group Limits: maximum clamping value of primary control output
    .equ AltMinOutput,                         76 ; parameter group Limits: minimum clamping value of alternate control output
    .equ AltMaxOutput,                         78 ; parameter group Limits: maximum clamping value of alternate control output
    .equ ptrADCTriggerARegister,               80 ; parameter group ADCTriggerControl: pointer to ADC trigger A register memory address
    .equ ADCTriggerAOffset,                    82 ; parameter group ADCTriggerControl: value of ADC trigger A offset
    .equ ptrADCTriggerBRegister,               84 ; parameter group ADCTriggerControl: pointer to ADC trigger B register memory address
    .equ ADCTriggerBOffset,                    86 ; parameter group ADCTriggerControl: value of ADC trigger B offset
    .equ ptrDProvControlInput,                 88 ; parameter group DataProviders: pointer to external variable/register the most recent, raw control input will be pushed to
    .equ ptrDProvControlInputComp,             90 ; parameter group DataProviders: pointer to external variable/register the most recent, compensated control input will be pushed to
    .equ ptrDProvControlError,                 92 ; parameter group DataProviders: pointer to external variable/register the most recent control error will be pushed to
    .equ ptrDProvControlOutput,                94 ; parameter group DataProviders: pointer to external variable/register the most recent control output will be pushed to
    .equ ptrExtHookStartFunction,              96 ; parameter group ExtensionHooks: pointer to external extension function which will be called by this controller at the beginning after evaluating the enable flag. This function call will be bypassed when the controller is disabled.
    .equ ExtHookStartFunctionParam,            98 ; parameter group ExtensionHooks: 16-bit wide function parameter variable or pointer to a parameter variable or data structure of extension function
    .equ ptrExtHookSourceFunction,            100 ; parameter group ExtensionHooks: pointer to external extension function which will be called by this controller after the most recent controller input has been read and compensated but before the most recent error is calculated.
    .equ ExtHookSourceFunctionParam,          102 ; parameter group ExtensionHooks: 16-bit wide function parameter variable or pointer to a parameter variable or data structure of extension function
    .equ ptrExtHookPreAntiWindupFunction,     104 ; parameter group ExtensionHooks: pointer to external extension function which will be called by this controller after the most recent compensation filter result has been computed and before  anti-windup clamping is applied
    .equ ExtHookPreAntiWindupFunctionParam,   106 ; parameter group ExtensionHooks: 16-bit wide function parameter variable or pointer to a parameter variable or data structure of extension function
    .equ ptrExtHookPreTargetWriteFunction,    108 ; parameter group ExtensionHooks: pointer to external extension function which will be called by this controller before the most recent controller output is written to target, after the value has passed through anti-windup clamping.
    .equ ExtHookPreTargetWriteFunctionParam,  110 ; parameter group ExtensionHooks: 16-bit wide function parameter variable or pointer to a parameter variable or data structure of extension function
    .equ ptrExtHookEndOfLoopFunction,         112 ; parameter group ExtensionHooks: pointer to external extension function which will be called by this controller after the compensation loop computation has been completed. This function call will be bypassed when the controller is disabled.
    .equ ExtHookEndOfLoopFunctionParam,       114 ; parameter group ExtensionHooks: 16-bit wide function parameter variable or pointer to a parameter variable or data structure of extension function
    .equ ptrExtHookExitFunction,              116 ; parameter group ExtensionHooks: pointer to external extension function which will be called by this controller after the compensation loop computation has been completed, regardless of the loop execution being bypassed or not
    .equ ExtHookExitFunctionParam,            118 ; parameter group ExtensionHooks: 16-bit wide function parameter variable or pointer to a parameter variable or data structure of extension function
    .equ usrParam0,                           120 ; parameter group Advanced: generic 16-bit wide, user-defined parameter #1 for user-defined, advanced control options
    .equ usrParam1,                           122 ; parameter group Advanced: generic 16-bit wide, user-defined parameter #2 for user-defined, advanced control options
    .equ usrParam2,                           124 ; parameter group Advanced: generic 16-bit wide, user-defined parameter #3 for user-defined, advanced control options
    .equ usrParam3,                           126 ; parameter group Advanced: generic 16-bit wide, user-defined parameter #4 for user-defined, advanced control options
    .equ usrParam4,                           128 ; parameter group Advanced: generic 16-bit wide, user-defined parameter #5 for user-defined, advanced control options
    .equ usrParam5,                           130 ; parameter group Advanced: generic 16-bit wide, user-defined parameter #6 for user-defined, advanced control options
    .equ usrParam6,                           132 ; parameter group Advanced: generic 16-bit wide, user-defined parameter #7 for user-defined, advanced control options
    .equ usrParam7,                           134 ; parameter group Advanced: generic 16-bit wide, user-defined parameter #8 for user-defined, advanced control options
    
;------------------------------------------------------------------------------
;source code section.
    .section .text                          ; place code in the text section
    
;------------------------------------------------------------------------------
; Global function declaration
; This function calls the z-domain controller processing the latest data point input
;------------------------------------------------------------------------------

    .global _VCOMP_Update                   ; provide global scope to routine
    _VCOMP_Update:                          ; local function label
    
;------------------------------------------------------------------------------
; Save working registers
    push.s                                  ; save shadowed working registers (WREG0, WREG1, WREG2, WREG3)
    
;------------------------------------------------------------------------------
; Save working registers
    push w4                                 ; save MAC operation working register WREG4
    push w6                                 ; save MAC operation working register WREG6
    push w8                                 ; save MAC operation working register WREG8
    push w10                                ; save MAC operation working register WREG10
    push ACCAL                              ; save accumulator A register (LOW WORD:   bit <15…0>)
    push ACCAH                              ; save accumulator A register (HIGH WORD:  bit <31…16>)
    push ACCAU                              ; save accumulator A register (UPPER BYTE: bit <39…32>)
    push ACCBL                              ; save accumulator B register (LOW WORD:   bit <15…0>)
    push ACCBH                              ; save accumulator B register (HIGH WORD:  bit <31…16>)
    push ACCBU                              ; save accumulator B register (UPPER BYTE: bit <39…32>)
    push CORCON                             ; save CPU configuration register
    push SR                                 ; save CPU status register
    
;------------------------------------------------------------------------------
; Configure DSP for fractional operation with normal saturation (Q1.31 format)
    mov #0x00C0, w4                         ; load default value of DSP core configuration enabling accumulator saturation and signed fractional multiply
    mov w4, _CORCON                         ; load default configuration into CORCON register
    
;------------------------------------------------------------------------------
; Setup pointers to A-Term data arrays
    mov [w0 + #ptrACoefficients], w8        ; load pointer to first index of A coefficients array
    
;------------------------------------------------------------------------------
; Load pointer to first element of control history array
    mov [w0 + #ptrControlHistory], w10      ; load pointer address into working register
    
;------------------------------------------------------------------------------
; Compute compensation filter term
    clr a, [w8]+=4, w4, [w10]+=2, w6        ; clear accumulator A and prefetch first operands
    mac w4*w6, a, [w8]+=4, w4, [w10]+=2, w6 ; multiply control output (n-1) from the delay line with coefficient A1
    mac w4*w6, a                            ; multiply & accumulate last control output with coefficient of the delay line (no more prefetch)
    
;------------------------------------------------------------------------------
; Setup pointer to first element of error history array
    mov [w0 + #ptrErrorHistory], w10        ; load pointer address into working register
    
;------------------------------------------------------------------------------
; Update error history (move error one tick down the delay line)
    mov [w10 + #2], w6                      ; move entry (n-2) into buffer
    mov w6, [w10 + #4]                      ; move buffered value one tick down the delay line
    mov [w10 + #0], w6                      ; move entry (n-1) into buffer
    mov w6, [w10 + #2]                      ; move buffered value one tick down the delay line
    
;------------------------------------------------------------------------------
; Read data from input source
    mov [w0 + #ptrSourceRegister], w2       ; load pointer to input source register
    mov [w2], w1                            ; move value from input source into working register
    
;------------------------------------------------------------------------------
; Load reference and calculate error input to transfer function
    mov [w0 + #ptrControlReference], w2     ; move pointer to control reference into working register
    subr w1, [w2], w1                       ; calculate error (=reference - input)
    mov [w0 + #normPreShift], w2            ; move error input scaler into working register
    sl w1, w2, w1                           ; normalize error result to fractional number format
    
;------------------------------------------------------------------------------
; Setup pointers to B-Term data arrays
    mov [w0 + #ptrBCoefficients], w8        ; load pointer to first index of B coefficients array
    mov w1, [w10]                           ; add most recent error input to history array
    
;------------------------------------------------------------------------------
; Compute B-Term of the compensation filter
    clr b, [w8]+=4, w4, [w10]+=2, w6        ; clear accumulator B and prefetch first operands
    mac w4*w6, b, [w8]+=4, w4, [w10]+=2, w6 ; multiply & accumulate error input (n-0) from the delay line with coefficient B0 and prefetch next operands
    mac w4*w6, b, [w8]+=4, w4, [w10]+=2, w6 ; multiply & accumulate error input (n-1) from the delay line with coefficient B1 and prefetch next operands
    mac w4*w6, b                            ; multiply & accumulate last error input with coefficient of the delay line (no more prefetch)
    
;------------------------------------------------------------------------------
; Add accumulators finalizing LDE computation
    add a                                   ; add accumulator b to accumulator a
    
;------------------------------------------------------------------------------
; Initialize Output Factor Scaleing and multiply control output with scaling factor
; (includes Backward normalization with accumulator scaling)
    mov  #ACCAH, w2                         ; Load pointer to accumulator A high word into working register
    fbcl [w2], w1                           ; Find first bit change from left and store position in working register
    sftac a, w1                             ; shift accumulator A by 'n' bits to scale number into ACCH
    sac.r a, w4                             ; store accumulator A high register contents in working register
    mov [w0 + #normPostScaler],  w6         ; load post scaler fractional value into working register
    mpy w4*w6, a                            ; multiply control output by scaler value loaded into working register
    mov [w0 + #normPostShiftA], w6          ; load post scaler bit-shift scaler value into working register
    neg w1, w1                              ; negate previously stored accumulator shift value
    add w1, w6, w6                          ; add post scaler bit-shift scaler and accumulator scaler
    sftac a, w6                             ; shift accumulator A by 'n' bits to scale control output
    sac.r a, w4                             ; store most recent accumulator result in working register
    
;------------------------------------------------------------------------------
; Controller Anti-Windup (control output value clamping)
     
; Check for lower limit violation
    mov [w0 + #MinOutput], w6               ; load lower limit value
    cpsgt w4, w6                            ; compare values and skip next instruction if control output is within operating range (control output > lower limit)
    mov w6, w4                              ; override most recent controller output
    VCOMP_CLAMP_MIN_EXIT:
     
; Check for upper limit violation
    mov [w0 + #MaxOutput], w6               ; load upper limit value
    cpslt w4, w6                            ; compare values and skip next instruction if control output is within operating range (control output < upper limit)
    mov w6, w4                              ; override most recent controller output
    VCOMP_CLAMP_MAX_EXIT:
    
;------------------------------------------------------------------------------
; Write control output value to target
    mov [w0 + #ptrTargetRegister], w8       ; capture pointer to target in working register
    mov w4, [w8]                            ; move control output to target address
    
;------------------------------------------------------------------------------
; Update ADC trigger locations
    asr w4, #1, w6                          ; half control output by shifting value one bit to the right
    ; Update ADC trigger A position
    mov [w0 + #ADCTriggerAOffset], w8       ; load user-defined ADC trigger A offset value into working register
    add w6, w8, w10                         ; add user-defined ADC trigger A offset to half of control output
    mov [w0 + #ptrADCTriggerARegister], w8  ; load pointer to ADC trigger A register into working register
    mov w10, [w8]                           ; push new ADC trigger value to ADC trigger A register
    
;------------------------------------------------------------------------------
; Load pointer to first element of control history array
    mov [w0 + #ptrControlHistory], w10      ; load pointer address into working register
    
;------------------------------------------------------------------------------
; Update control output history (move entries one tick down the delay line)
    mov [w10 + #0], w6                      ; move entry (n-1) one tick down the delay line
    mov w6, [w10 + #2]
    mov w4, [w10]                           ; add most recent control output to history
    
;------------------------------------------------------------------------------
; Restore working registers in reverse order
    pop SR                                  ; restore CPU status registers
    pop CORCON                              ; restore CPU configuration registers
    pop ACCBU                               ; restore accumulator B register (UPPER BYTE: bit <39…32>)
    pop ACCBH                               ; restore accumulator B register (HIGH WORD:  bit <31…16>)
    pop ACCBL                               ; restore accumulator B register (LOW WORD:   bit <15…0>)
    pop ACCAU                               ; restore accumulator A register (UPPER BYTE: bit <39…32>)
    pop ACCAH                               ; restore accumulator A register (HIGH WORD:  bit <31…16>)
    pop ACCAL                               ; restore accumulator A register (LOW WORD:   bit <15…0>)
    pop w10                                 ; restore MAC operation working register WREG10
    pop w8                                  ; restore MAC operation working register WREG8
    pop w6                                  ; restore MAC operation working register WREG6
    pop w4                                  ; restore MAC operation working register WREG4
    
;------------------------------------------------------------------------------
; Restore working registers in reverse order
    pop.s                                   ; restore shadowed working registers (WREG0, WREG1, WREG2, WREG3)
    
;------------------------------------------------------------------------------
; End of routine
    return                                  ; end of function; return to caller
    
;------------------------------------------------------------------------------

    
;------------------------------------------------------------------------------
; Global function declaration VCOMP_Reset
; This function clears control and error histories enforcing a reset
;------------------------------------------------------------------------------

    .global _VCOMP_Reset                    ; provide global scope to routine
    _VCOMP_Reset:                           ; local function label
    
;------------------------------------------------------------------------------
; Clear control history array
    push w0                                 ; save contents of working register WREG0
    mov [w0 + #ptrControlHistory], w0       ; set pointer to the base address of control history array
    clr [w0++]                              ; clear next address of control history array
    clr [w0]                                ; clear last address of control history array
    pop w0                                  ; restore contents of working register WREG0
    
;------------------------------------------------------------------------------
; Clear error history array
    push w0                                 ; save contents of working register WREG0
    mov [w0 + #ptrErrorHistory], w0         ; set pointer to the base address of error history array
    clr [w0++]                              ; Clear next address of error history array
    clr [w0++]                              ; Clear next address of error history array
    clr [w0]                                ; clear last address of error history array
    pop w0                                  ; restore contents of working register WREG0
    
;------------------------------------------------------------------------------
; End of routine
    return                                  ; end of function; return to caller
    
;------------------------------------------------------------------------------

    
;------------------------------------------------------------------------------
; Global function declaration VCOMP_Precharge
; This function loads user-defined default values into control and error histories
;------------------------------------------------------------------------------

    .global _VCOMP_Precharge                ; provide global scope to routine
    _VCOMP_Precharge:                       ; local function label
    
;------------------------------------------------------------------------------
; Charge error history array with defined value
    push w0                                 ; save contents of working register WREG0
    push w1                                 ; save contents of working register WREG1
    mov  [w0 + #ptrErrorHistory], w0        ; set pointer to the base address of error history array
    mov w1, [w0++]                          ; Load user value into next address of error history array
    mov w1, [w0++]                          ; Load user value into next address of error history array
    mov w1, [w0]                            ; load user value into last address of error history array
    pop w1                                  ; restore contents of working register WREG1
    pop w0                                  ; restore contents of working register WREG0
    
;------------------------------------------------------------------------------
; Charge control history array with defined value
    push w0                                 ; save contents of working register WREG0
    push w2                                 ; save contents of working register WREG2
    mov  [w0 + #ptrControlHistory], w0      ; set pointer to the base address of control history array
    mov w2, [w0++]                          ; Load user value into next address of control history array
    mov w2, [w0]                            ; Load user value into last address of control history array
    pop w2                                  ; restore contents of working register WREG2
    pop w0                                  ; restore contents of working register WREG0
    
;------------------------------------------------------------------------------
; End of routine
    return                                  ; end of function; return to caller
    
;------------------------------------------------------------------------------

    
;------------------------------------------------------------------------------
; End of file
    .end                                    ; end of file VCOMP_asm.s
    
;------------------------------------------------------------------------------

     
; **********************************************************************************
;  Download latest version of this tool here: 
;//      https://www.microchip.com/powersmart
; **********************************************************************************
