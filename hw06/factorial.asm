;;=============================================================
;;  CS 2110 - Spring 2023
;;  Homework 6 - Factorial
;;=============================================================
;;  Name: Michelle Nmamgoong
;;============================================================

;;  In this file, you must implement the 'MULTIPLY' and 'FACTORIAL' subroutines.

;;  Little reminder from your friendly neighborhood 2110 TA staff: don't run
;;  this directly by pressing 'Run' in complx, since there is nothing put at
;;  address x3000. Instead, call the subroutine by doing the following steps:
;;      * 'Debug' -> 'Simulate Subroutine Call'
;;      * Call the subroutine at the 'MULTIPLY' or 'FACTORIAL' labels
;;      * Add the [a, b] or [n] params separated by a comma (,) 
;;        (e.g. 3, 5 for 'MULTIPLY' or 6 for 'FACTORIAL')
;;      * Proceed to run, step, add breakpoints, etc.
;;      * Remember R6 should point at the return value after a subroutine
;;        returns. So if you run the program and then go to 
;;        'View' -> 'Goto Address' -> 'R6 Value', you should find your result
;;        from the subroutine there (e.g. 3 * 5 = 15 or 6! = 720)

;;  If you would like to setup a replay string (trace an autograder error),
;;  go to 'Test' -> 'Setup Replay String' -> paste the string (everything
;;  between the apostrophes (')) excluding the initial " b' ". If you are 
;;  using the Docker container, you may need to use the clipboard (found
;;  on the left panel) first to transfer your local copied string over.

.orig x3000
    ;; You do not need to write anything here
    HALT

;;  MULTIPLY Pseudocode (see PDF for explanation and examples)   
;;  
;;  MULTIPLY(int a, int b) {
;;      int ret = 0;
;;      while (b > 0) {
;;          ret += a;
;;          b--;
;;      }
;;      return ret;
;;  }

MULTIPLY ;; Do not change this label! Treat this as like the name of the function in a function header
    ;; Code your implementation for the MULTIPLY subroutine here!

    ;; STACK BUILDUP
    ADD R6, R6, -4              ;; make space for RV, RA, old FP, and first LV
    STR R7, R6, 2               ;; store RA 
    STR R5, R6, 1               ;; store old FP
    ADD R5, R6, 0               ;; set new FP to SP
    ADD R6, R6, -5              ;; make space for old REGs (R0-R4)

    STR R4, R6, 4               ;; store old R4
    STR R3, R6, 3               ;; store old R3
    STR R2, R6, 2               ;; store old R2
    STR R1, R6, 1               ;; store old R1
    STR R0, R6, 0               ;; store old R0

    LDR R0, R5, 4               ;; R0 = a (first LV at R5 + 4)
    LDR R1, R5, 5               ;; R1 = b
    AND R2, R2, 0               ;; R2 = ret = 0

WHILE
    ADD R1, R1, 0               ;; set cc to b
    BRnz WHILE_END              ;; END if b >= 0
    ADD R2, R2, R0              ;; ret += a
    ADD R1, R1, -1              ;; b--
    BR WHILE

WHILE_END
    STR R2, R5, 3               ;; store ret (RV at R5 + 3)

    ;; STACK TEARDOWN
    LDR R0, R6, 0               ;; restore old R0
    LDR R1, R6, 1               ;; restore old R1
    LDR R2, R6, 2               ;; restore old R2
    LDR R3, R6, 3               ;; restore old R3
    LDR R4, R6, 4               ;; restore old R4
    ADD R6, R5, 0               ;; pop off restored registers and any LV
    LDR R5, R6, 1               ;; restore old FP
    LDR R7, R6, 2               ;; restore RA
    ADD R6, R6, 3               ;; pop off LV, old FP, and RA (but keep RV)

    RET

;;  FACTORIAL Pseudocode (see PDF for explanation and examples)
;;
;;  FACTORIAL(int n) {
;;      int ret = 1;
;;      for (int x = 2; x <= n; x++) {
;;          ret = MULTIPLY(ret, x);
;;      }
;;      return ret;
;;  }

FACTORIAL ;; Do not change this label! Treat this as like the name of the function in a function header
    ;; Code your implementation for the FACTORIAL subroutine here!

    ;; STACK BUILDUP
    ADD R6, R6, -4              ;; make space for RV, RA, old FP, and first LV
    STR R7, R6, 2               ;; store RA
    STR R5, R6, 1               ;; store old FP
    ADD R5, R6, 0               ;; set new FP to SP
    ADD R6, R6, -5              ;; make space for old REGs (R0-R4)

    STR R4, R6, 4               ;; store old R4
    STR R3, R6, 3               ;; store old R3
    STR R2, R6, 2               ;; store old R2
    STR R1, R6, 1               ;; store old R1
    STR R0, R6, 0               ;; store old R0

    LDR R0, R5, 4               ;; R0 = n (first LV at R5 + 4)
    AND R1, R1, 0
    ADD R1, R1, 1               ;; R1 = ret = 1

    ;; FOR LOOP STARTS HERE
    AND R2, R2, 0
    ADD R2, R2, 2               ;; R2 = x = 2

FOR
    NOT R3, R0
    ADD R3, R3, 1               ;; R3 = -n
    ADD R3, R2, R3              ;; set cc to x - n
    BRp FOR_END                 ;; END if x > n
    ADD R6, R6, -2              ;; make space for 2 arguments for MULTIPLY
    STR R1, R6, 0               ;; push ret
    STR R2, R6, 1               ;; push x
    JSR MULTIPLY
    LDR R1, R6, 0               ;; R1 = ret = MULTIPLY(ret, x)
    ADD R6, R6, 3               ;; pop RV, ret, and x
    
    ADD R2, R2, 1               ;; x++
    BR FOR

FOR_END
    STR R1, R5, 3               ;; store ret (RV at R5 + 3)

    ;; STACK TEARDOWN
    LDR R0, R6, 0               ;; restore old R0
    LDR R1, R6, 1               ;; restore old R1
    LDR R2, R6, 2               ;; restore old R2
    LDR R3, R6, 3               ;; restore old R3
    LDR R4, R6, 4               ;; restore old R4
    ADD R6, R5, 0               ;; pop off restored registers and any LV
    LDR R5, R6, 1               ;; restore old FP
    LDR R7, R6, 2               ;; restore RA
    ADD R6, R6, 3               ;; pop off LV, old FP, and RA (but keep RV)

    RET

;; Needed to Simulate Subroutine Call in Complx
STACK .fill xF000
.end