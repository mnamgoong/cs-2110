;;=============================================================
;; CS 2110 - Spring 2023
;; Homework 5 - modulus
;;=============================================================
;; Name: Michelle Namgoong
;;=============================================================

;;  Pseudocode (see PDF for explanation)
;;  Pseudocode values are based on the labels' default values.
;;
;;  int x = 17;
;;  int mod = 5;
;;  while (x >= mod) {
;;      x -= mod;
;;  }
;;  mem[ANSWER] = x;

.orig x3000
    LD R0, X            ;; R0 holds x
    LD R1, MOD          ;; R1 holds mod
WHILE
    NOT R2, R1
    ADD R2, R2, 1       ;; R2 holds 2's complement of mod
    ADD R3, R2, R0      ;; set condition codes to test if x >= mod
    BRn END
    ADD R0, R0, R2      ;; x -= mod
    BR WHILE
END
    ST R0, ANSWER       ;; mem[ANSWER] = x
    HALT

    ;; Feel free to change the below values for debugging. We will vary these values when testing your code.
    X      .fill 17
    MOD    .fill 5     
    ANSWER .blkw 1
.end