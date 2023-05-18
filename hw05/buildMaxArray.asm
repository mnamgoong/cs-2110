;;=============================================================
;; CS 2110 - Spring 2023
;; Homework 5 - buildMaxArray
;;=============================================================
;; Name: Michelle Namgoong
;;=============================================================

;;  Pseudocode (see PDF for explanation)
;;  Pseudocode values are based on the labels' default values.
;;
;;	int A[] = {-2, 2, 1};
;;	int B[] = {1, 0, 3};
;;	int C[3];
;;	int length = 3;
;;
;;	int i = 0;
;;	while (i < length) {
;;		if (A[i] >= B[length - i - 1]) {
;;			C[i] = 1;
;;		}
;;		else {
;;			C[i] = 0;
;;		}
;;		i++;
;;	}

.orig x3000
	LD R0, LENGTH		;; R3 holds length
	AND R1, R1, 0		;; R4 holds i = 0
WHILE
	NOT R2, R0
    ADD R2, R2, 1       ;; R2 holds 2's complement of length
    ADD R2, R1, R2      ;; set condition codes to test if i < length
    BRzp END
	LD R3, A			
	ADD R4, R3, R1		
	LDR R4, R4, 0		;; R4 holds A[i]
	NOT R5, R1
	ADD R5, R5, 1 		;; R5 holds 2's complement i
	ADD R5, R0, R5		;; length - 1
	ADD R5, R5, -1		;; length - i - 1
	LD R3, B
	ADD R6, R3, R5		
	LDR R6, R6, 0		;; R6 holds B[length - i - 1]
	NOT R6, R6
	ADD R6, R6, 1		;; R6 holds 2's complement B[length - i - 1]
	LD R7, C
	ADD R7, R7, R1 		;; R7 holds C[i]
	AND R3, R3, 0
	AND R2, R2, 0
	ADD R2, R4, R6		;; set condition codes to test if A[i] >= B[length - i - 1]
	BRn ELSE
	ADD R3, R3, 1
	STR R3, R7, 0		;; C[i] = 0
	BR ENDIF
ELSE
	STR R3, R7, 0		;; C[i] = 1
	BR ENDIF
ENDIF
	ADD R1, R1, 1
	BR WHILE
END
	HALT

;; Do not change these addresses! 
;; We populate A and B and reserve space for C at these specific addresses in the orig statements below.
A 		.fill x3200		
B 		.fill x3300		
C 		.fill x3400		
LENGTH 	.fill 3			;; Change this value if you decide to increase the size of the arrays below.
.end

;; Do not change any of the .orig lines!
;; If you decide to add more values for debugging, make sure to adjust LENGTH and .blkw 3 accordingly.
.orig x3200				;; Array A : Feel free to change or add values for debugging.
	.fill -2
	.fill 2
	.fill 1
.end

.orig x3300				;; Array B : Feel free change or add values for debugging.
	.fill 1
	.fill 0
	.fill 3
.end

.orig x3400
	.blkw 3				;; Array C: Make sure to increase block size if you've added more values to Arrays A and B!
.end