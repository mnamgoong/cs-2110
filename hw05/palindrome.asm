;;=============================================================
;; CS 2110 - Spring 2023
;; Homework 5 - palindrome
;;=============================================================
;; Name: Michelle Namgoong
;;=============================================================

;;  NOTE: Let's decide to represent "true" as a 1 in memory and "false" as a 0 in memory.
;;
;;  Pseudocode (see PDF for explanation)
;;  Pseudocode values are based on the labels' default values.
;;
;;  String str = "aibohphobia";
;;  boolean isPalindrome = true
;;  int length = 0;
;;  while (str[length] != '\0') {
;;		length++;
;;	}
;; 	
;;	int left = 0
;;  int right = length - 1
;;  while(left < right) {
;;		if (str[left] != str[right]) {
;;			isPalindrome = false;
;;			break;
;;		}
;;		left++;
;;		right--;
;;	}
;;	mem[mem[ANSWERADDR]] = isPalindrome;

.orig x3000
	LD R0, STRING				;; R0 holds str
	AND R1, R1, 0
	ADD R1, R1, 1				;; R1 holds isPalindrome = true (1)
	AND R2, R2, 0				;; R2 holds length = 0
FIRSTWHILE
	ADD R3, R0, R2				
	LDR R3, R3, 0				;; set condition codes to test if str[length] != 0
	BRnp LENGTH 
	AND R4, R4, 0				;; R4 holds left = 0
	ADD R2, R2, -1				;; R2 holds right = length - 1
	BR SECONDWHILE
SECONDWHILE
	NOT R5, R2
	ADD R5, R5, 1				;; R6 holds 2's complement right
	ADD R3, R4, R5				;; set condition codes to test if left < right
	BRzp END
	ADD R6, R0, R4
	LDR R6, R6, 0				;; R6 holds str[left]
	ADD R7, R0, R2
	LDR R7, R7, 0				;; R7 holds str[right]
	NOT R7, R7
	ADD R7, R7, 1				;; R7 holds 2's complement str[right]
	ADD R3, R6, R7				;; set condition codes to test if str[left] != str[right]
	BRnp IF
	ADD R4, R4, 1				;; left++
	ADD R2, R2, -1				;; right--
	BR SECONDWHILE
IF
	ADD R1, R1, -1				;; isPalindrome = false (0)
	BR END
LENGTH
	ADD R2, R2, 1				;; length++
	BR FIRSTWHILE
END
	STI R1, ANSWERADDR          ;; mem[mem[ANSWERADDR]] = isPalindrome  
	HALT

;; Do not change these values!
STRING	.fill x4004
ANSWERADDR 	.fill x5005
.end

;; Do not change any of the .orig lines!
.orig x4004				   
	.stringz "aibohphobia" ;; Feel free to change this string for debugging.
.end

.orig x5005
	ANSWER  .blkw 1
.end