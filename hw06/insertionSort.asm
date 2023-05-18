;;=============================================================
;;  CS 2110 - Spring 2023
;;  Homework 6 - Insertion Sort
;;=============================================================
;;  Name: Michelle Namgoong
;;============================================================

;;  In this file, you must implement the 'INSERTION_SORT' subroutine.

;;  Little reminder from your friendly neighborhood 2110 TA staff: don't run
;;  this directly by pressing 'Run' in complx, since there is nothing put at
;;  address x3000. Instead, call the subroutine by doing the following steps:
;;      * 'Debug' -> 'Simulate Subroutine Call'
;;      * Call the subroutine at the 'INSERTION_SORT' label
;;      * Add the [arr (addr), length] params separated by a comma (,) 
;;        (e.g. x4000, 5)
;;      * Proceed to run, step, add breakpoints, etc.
;;      * INSERTION_SORT is an in-place algorithm, so if you go to the address
;;        of the array by going to 'View' -> 'Goto Address' -> 'Address of
;;        the Array', you should see the array (at x4000) successfully 
;;        sorted after running the program (e.g [2,3,1,1,6] -> [1,1,2,3,6])

;;  If you would like to setup a replay string (trace an autograder error),
;;  go to 'Test' -> 'Setup Replay String' -> paste the string (everything
;;  between the apostrophes (')) excluding the initial " b' ". If you are 
;;  using the Docker container, you may need to use the clipboard (found
;;  on the left panel) first to transfer your local copied string over.

.orig x3000
    ;; You do not need to write anything here
    HALT

;;  INSERTION_SORT **RESURSIVE** Pseudocode (see PDF for explanation and examples)
;; 
;;  INSERTION_SORT(int[] arr (addr), int length) {
;;      if (length <= 1) {
;;        return;
;;      }
;;  
;;      INSERTION_SORT(arr, length - 1);
;;  
;;      int last_element = arr[length - 1];
;;      int n = length - 2;
;;  
;;      while (n >= 0 && arr[n] > last_element) {
;;          arr[n + 1] = arr[n];
;;          n--;
;;      }
;;  
;;      arr[n + 1] = last_element;
;;  }

INSERTION_SORT ;; Do not change this label! Treat this as like the name of the function in a function header
    ;; Code your implementation for the INSERTION_SORT subroutine here!
    ;; NOTE: Your implementation MUST be done recursively

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
    
    AND R0, R0, 0
    STR R0, R5, 0               
    ADD R0, R0, 1               ;; R0 = n = index of current element

CHECK_LENGTH
    LDR R1, R5, 5               ;; R1 = length
    AND R2, R2, 0
    ADD R2, R2, -1              ;; R2 = -1
    ADD R2, R1, R2              ;; R2 = length - 1
    BRnz END                    ;; end if length <= 1

LOOP
    LDR R2, R5, 5               ;; R2 = length - 1 = index of last_element
    NOT R2, R2
    ADD R2, R2, 1               ;; R2 = -(length - 1)
    ADD R2, R0, R2              ;; set cc to n - index of last_element
    BRzp STOPLOOP               ;; STOPLOOP if n <= index of last_element
    LDR R1, R5, 4               ;; R1 = arr (addr) 
    ADD R2, R0, R1              ;; R2 = arr[n] (addr)
    LDR R2, R2, 0               ;; R2 = arr[n]
    LDR R3, R5, 0               ;; R3 = n + 1 = index of previous element
    ADD R3, R1, R3              ;; R3 = arr[n + 1] (addr)
    LDR R3, R3, 0               ;; R3 = arr[n + 1]
    NOT R3, R3      
    ADD R3, R3, 1               ;; R3 = -arr[n + 1]
    ADD R3, R2, R3              ;; set cc to arr[n] - arr[n + 1]
    BRnz DONTSWAP               ;; DONTSWAP if arr[n] <= arr[n + 1] (last_element)
    STR R0, R5, 0               ;; store n

DONTSWAP
    ADD R0, R0, 1               ;; n + 1
    BR LOOP

STOPLOOP
    LDR R0, R5, 0               ;; R0 = n = index of current element
    LDR R1, R5, 4               ;; R1 = arr (addr)
    LDR R2, R5, 5               ;; R2 = length
    ADD R2, R2, -1              ;; R2 = length - 1
    ADD R0, R1, R0              ;; R0 = arr[n] (addr)
    LDR R3, R0, 0               ;; R3 = arr[n]
    ADD R2, R1, R2              ;; R2 = arr[length - 1] (addr)
    LDR R4, R2, 0               ;; R4 = arr[length - 1]
    STR R3, R2, 0               ;; R2 = R3 = arr[n]
    STR R4, R0, 0               ;; R0 = R4 = arr[length - 1]
    LDR R1, R5, 4               ;; R1 = arr (addr)
    LDR R2, R5, 5               ;; R2 = length
    ADD R6, R6, -2              ;; make space for 2 arguments
    ADD R2, R2, -1              ;; R2 = length - 1
    STR R1, R6, 0               ;; push arr
    STR R2, R6, 1               ;; push length - 1
    JSR INSERTION_SORT          ;; INSERTION_SORT(arr, length - 1)
    ADD R6, R6, 3               ;; pop off RV, length, and arr

END
    ;; STACK TEARDOWN
    LDR R4, R6, 4               ;; restore old R4
    LDR R3, R6, 3               ;; restore old R3
    LDR R2, R6, 2               ;; restore old R2
    LDR R1, R6, 1               ;; restore old R1
    LDR R0, R6, 0               ;; restore old R0
    ADD R6, R5, 0               ;; pop off old restored registers and any LV
    LDR R5, R6, 1               ;; restore old FP
    LDR R7, R6, 2               ;; restore old RA
    ADD R6, R6, 3               ;; pop off LV, old FP, and RA (but keep RV)

    RET

;; Needed to Simulate Subroutine Call in Complx
STACK .fill xF000
.end

.orig x4000	;; Array : You can change these values for debugging!
    .fill 2
    .fill 3
    .fill 1
    .fill 1
    .fill 6
.end