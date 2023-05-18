;; Timed Lab 3
;; Student Name: Michelle Namgoong

;; Please read the PDF for full directions.
;; The pseudocode for the program you must implement is listed below; it is also listed in the PDF.
;; If there are any discrepancies between the PDF's pseudocode and the pseudocode below, notify a TA immediately.
;; However, in the end, the pseudocode is just an example of a program that would fulfill the requirements specified in the PDF.

;; Pseudocode:
;; // (checkpoint 1)
;; int MAX(int a, int b) {
;;   if (a > b) {
;;       return 0;
;;   } else {
;;       return 1;
;;   }
;; }
;;
;;
;;
;;
;; DIV(x, y) {
;;	   // (checkpoint 2) - Base Case
;;     if (y > x) {
;;         return 0;
;;     // (checkpoint 3) - Recursion
;;     } else {
;;         return 1 + DIV(x - y, y);
;;     }
;; }
;;
;;
;;
;; // (checkpoint 4)
;; void MAP(array, length) {
;;   int i = 0;
;;   while (i < length) {
;;      int firstElem = arr[i];
;;      int secondElem = arr[i + 1];
;;      int div = DIV(firstElem, secondElem);
;;      int offset = MAX(firstElem, secondElem);
;;      arr[i + offset] = div;
;;      i += 2;
;;   }
;; }


.orig x3000
HALT

STACK .fill xF000

; DO NOT MODIFY ABOVE


; START MAX SUBROUTINE
MAX



; !!!!! WRITE YOUR CODE HERE !!!!!

    ; STACK BUILDUP
    ADD R6, R6, -4              ; make space for RV, RA, old FP, and first LV
    STR R7, R6, 2               ; store RA
    STR R5, R6, 1               ; store old FP
    ADD R5, R6, 0               ; set new FP to SP (first LV)
    ADD R6, R6, -5              ; make space for old REGs (R0-R4)
    STR R0, R6, 0               ; store old R0
    STR R1, R6, 1               ; store old R1
    STR R2, R6, 2               ; store old R2
    STR R3, R6, 3               ; store old R3
    STR R4, R6, 4               ; store old R4

    LDR R0, R5, 4               ; R0 = a
    LDR R1, R5, 5               ; R1 = b

IF
    NOT R2, R1
    ADD R2, R2, 1               ; R2 = -b
    ADD R2, R0, R2              ; R2 = a-b (set cc)
    BRnz ELSE
    AND R3, R3, 0
    STR R3, R5, 3               ; return 0 (store as RV)
    BR MAX_END

ELSE
    AND R3, R3, 0
    ADD R3, R3, 1
    STR R3, R5, 3               ; return 1 (store as RV)
    BR MAX_END

MAX_END
    ; STACK TEARDOWN
    LDR R0, R6, 0               ; restore old R0
    LDR R1, R6, 1               ; restore old R1
    LDR R2, R6, 2               ; restore old R2
    LDR R3, R6, 3               ; restore old R3
    LDR R4, R6, 4               ; restore old R4
    ADD R6, R5, 0               ; pop off old restored registers and any LV
    LDR R5, R6, 1               ; restore old FP
    LDR R7, R6, 2               ; restore old RA
    ADD R6, R6, 3               ; pop off LV, old FP, and RA (but keep RV)

RET
; END MAX SUBROUTINE




; START DIV SUBROUTINE
DIV

;; DIV(x, y) {
;;	   // (checkpoint 2) - Base Case
;;     if (y > x) {
;;         return 0;
;;     // (checkpoint 3) - Recursion
;;     } else {
;;         return 1 + DIV(x - y, y);
;;     }
;; }

; !!!!! WRITE YOUR CODE HERE !!!!!

    ; STACK BUILDUP
    ADD R6, R6, -4              ; make space for RV, RA, old FP, and first LV
    STR R7, R6, 2               ; store RA
    STR R5, R6, 1               ; store old FP
    ADD R5, R6, 0               ; set new FP to SP (first LV)
    ADD R6, R6, -5              ; make space for old REGs (R0-R4)
    STR R0, R6, 0               ; store old R0
    STR R1, R6, 1               ; store old R1
    STR R2, R6, 2               ; store old R2
    STR R3, R6, 3               ; store old R3
    STR R4, R6, 4               ; store old R4

    LDR R0, R5, 4               ; R0 = x
    LDR R1, R5, 5               ; R1 = y

    NOT R2, R1
    ADD R2, R2, 1               ; R2 = -y
    ADD R2, R0, R2              ; R2 = x-y (set cc)
    BRn BASECASE
    BRzp RECURSION

RECURSION
    ADD R6, R6, -1              ; make space for 1 argument
    STR R1, R6, 0               ; push y (R1)
    ADD R6, R6, -1              ; make space for 1 argument
    STR R2, R6, 0               ; push x-y (R2)
    JSR DIV
	LDR R3, R6, 0               ; R3 = DIV(x-y, y)
	ADD R6, R6, 1               ; pop the RV
	ADD R6, R6, 2               ; pop the arguments
	ADD R3, R3, 1               ; R3 = 1 + DIV(x-y, y)
	STR R3, R5, 3
	BR DIV_END

BASECASE
    AND R4, R4, 0
    STR R4, R5, 3               ; return 0 (store as RV)
    BR DIV_END

DIV_END
    ; STACK TEARDOWN
    LDR R0, R6, 0               ; restore old R0
    LDR R1, R6, 1               ; restore old R1
    LDR R2, R6, 2               ; restore old R2
    LDR R3, R6, 3               ; restore old R3
    LDR R4, R6, 4               ; restore old R4
    ADD R6, R5, 0               ; pop off old restored registers and any LV
    LDR R5, R6, 1               ; restore old FP
    LDR R7, R6, 2               ; restore old RA
    ADD R6, R6, 3               ; pop off LV, old FP, and RA (but keep RV)

RET
; END DIV SUBROUTINE



; START MAP SUBROUTINE
MAP



; !!!!! WRITE YOUR CODE HERE !!!!!

    ; STACK BUILDUP
    ADD R6, R6, -4              ; make space for RV, RA, old FP, and first LV
    STR R7, R6, 2               ; store RA
    STR R5, R6, 1               ; store old FP
    ADD R5, R6, 0               ; set new FP to SP (first LV)
    ADD R6, R6, -5              ; make space for old REGs (R0-R4)
    STR R0, R6, 0               ; store old R0
    STR R1, R6, 1               ; store old R1
    STR R2, R6, 2               ; store old R2
    STR R3, R6, 3               ; store old R3
    STR R4, R6, 4               ; store old R4

    LDR R0, R5, 4               ; R0 = array
    AND R1, R1, 0               ; R1 = i = 0

WHILE
    LDR R2, R5, 5               ; R2 = length
    NOT R2, R2
    ADD R2, R2, 1               ; R2 = -length
    ADD R2, R1, R2              ; R2 = i-length (set cc)
    BRzp MAP_END
    ADD R2, R0, R1              ; R2 = firstElem = array[i]
    LDR R2, R2, 0
    ADD R3, R1, 1               ; R3 = i+1
    ADD R3, R0, R3              ; R3 = secondElem = array[i+1]
    LDR R3, R3, 0

    ADD R6, R6, -1              ; make space for 1 argument
    STR R3, R6, 0               ; push secondElem (R3)
    ADD R6, R6, -1              ; make space for 1 argument
    STR R2, R6, 0               ; push firstElem (R2)
    JSR DIV
	LDR R4, R6, 0               ; R4 = div = DIV(firstElem, secondElem)
	ADD R6, R6, 1               ; pop the RV
	ADD R6, R6, 2               ; pop the arguments

    ADD R6, R6, -1              ; make space for 1 argument
    STR R3, R6, 0               ; push secondElem (R3)
    ADD R6, R6, -1              ; make space for 1 argument
    STR R2, R6, 0               ; push firstElem (R2)
    JSR MAX
	LDR R3, R6, 0               ; R3 = offset = MAX(firstElem, secondElem)
	ADD R6, R6, 1               ; pop the RV
	ADD R6, R6, 2               ; pop the arguments

    ADD R3, R1, R3              ; R3 = i+offset
    ADD R3, R0, R3              ; R3 = array[i+offset]
    STR R4, R3, 0               ; array[i+offset] = div
    ADD R1, R1, 2               ; i += 2
    BR WHILE

MAP_END
    ; STACK TEARDOWN
    LDR R0, R6, 0               ; restore old R0
    LDR R1, R6, 1               ; restore old R1
    LDR R2, R6, 2               ; restore old R2
    LDR R3, R6, 3               ; restore old R3
    LDR R4, R6, 4               ; restore old R4
    ADD R6, R5, 0               ; pop off old restored registers and any LV
    LDR R5, R6, 1               ; restore old FP
    LDR R7, R6, 2               ; restore old RA
    ADD R6, R6, 3               ; pop off LV, old FP, and RA (but keep RV)

RET
; END MAP SUBROUTINE


; LENGTH FOR TESTING

LENGTH .fill x12

; ARRAY FOR TESTING
ARRAY .fill x4000

.end

.orig x4000
.fill 12
.fill 3
.fill 5
.fill 7
.fill 16
.fill 2
.fill 5
.fill 5
.fill 25
.fill 7
.fill 48
.fill 60
.end
