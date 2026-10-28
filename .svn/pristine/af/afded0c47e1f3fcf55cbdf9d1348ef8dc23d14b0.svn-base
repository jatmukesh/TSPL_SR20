;******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
;* File Name          : cortexm3_macro.s
;* Author             : MCD Application Team
;* Version            : V2.0.1
;* Date               : 06/13/2008
;* Description        : Instruction wrappers for special Cortex-M3 instructions
;*                      to be used with RVMDK toolchain.
;*******************************************************************************
; THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
; WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
; AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
; INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
; CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
; INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
;*******************************************************************************
    
  THUMB
  REQUIRE8
  PRESERVE8
    
  AREA |.text|, CODE, READONLY, ALIGN=2

  ; Exported functions

  
  EXPORT __RESETPRIMASK
  EXPORT __SETPRIMASK
  


;*******************************************************************************
; Function Name  : __RESETPRIMASK
; Description    : Assembler function to reset the PRIMASK.
; Input          : None 
; Return         : None
;*******************************************************************************
__RESETPRIMASK

  CPSIE i
  BX r14

;*******************************************************************************
; Function Name  : __SETPRIMASK
; Description    : Assembler function to set the PRIMASK.
; Input          : None 
; Return         : None
;*******************************************************************************
__SETPRIMASK

  CPSID i
  BX r14
  END
  
;******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE*****


