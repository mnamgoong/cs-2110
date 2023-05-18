;;=============================================================
;; CS 2110 - Spring 2023
;; Homework 5 - octalStringToInt
;;=============================================================
;; Name: Michelle Namgoong
;;=============================================================

;;  Pseudocode (see PDF for explanation)
;;  Pseudocode values are based on the labels' default values.
;;
;;  String octalString = "2110";
;;  int length = 4;
;;  int value = 0;                          
;;  int i = 0;
;;  while (i < length) {
;;      int leftShifts = 3;
;;      while (leftShifts > 0) {
;;          value += value;
;;          leftShifts--;
;;      }
;;      int digit = octalString[i] - 48;
;;      value += digit;
;;      i++;
;;  }
;;  mem[mem[RESULTADDR]] = value;

.orig x3000
    LD R0, OCTALSTRING	        ;; R0 holds octalString 
    LD R1, LENGTH               ;; R1 holds length
    AND R2, R2, 0               ;; R2 holds value = 0
    AND R3, R3, 0               ;; R3 holds i = 0
WHILE
    NOT R4, R1
    ADD R4, R4, 1               ;; R4 holds 2's complement of length
    ADD R4, R4, R3              ;; set condition codes to test if i < length
    BRzp END            
    AND R5, R5, 0       
    ADD R5, R5, 3               ;; R5 holds leftShifts = 3
    BR INNERWHILE
INNERWHILE
    ADD R2, R2, R2              ;; value += value
    ADD R5, R5, -1              ;; leftShifts--
    BR ELSE
ELSE
    ADD R5, R5, 0               ;; set condition codes to test if leftShifts > 0
    BRp INNERWHILE
    ADD R6, R0, R3              ;; char at octalString[i]
    LDR R6, R6, 0               ;; digit = octalString[i]
    LD R7, ASCII                ;; R7 holds -48
    ADD R6, R6, R7              ;; digit = octalString[i] - 48
    ADD R2, R2, R6              ;; value += digit
    ADD R3, R3, 1               ;; i++
    BR WHILE
END
    STI R2, RESULTADDR          ;; mem[mem[RESULTADDR]] = value
    HALT
    
;; Do not change these values! 
;; Notice we wrote some values in hex this time. Maybe those values should be treated as addresses?

OCTALSTRING     .fill x5000
LENGTH          .fill 4
RESULTADDR      .fill x4000
.end

.orig x5000                    ;;  Don't change the .orig statement
    .stringz "2110"            ;;  You can change this string for debugging!
.end
