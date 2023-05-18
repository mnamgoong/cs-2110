/**
 * CS 2110 Spring 2023 HW1
 * Part 2 - Coding with bases
 *
 * @author Michelle Namgoong
 *
 * Global rules for this file:
 * - You may not use more than 2 conditionals per method. Conditionals are
 *   if-statements, if-else statements, or ternary expressions. The else block
 *   associated with an if-statement does not count toward this sum.
 * - You may not use more than 2 looping constructs per method. Looping
 *   constructs include for loops, while loops and do-while loops.
 * - You may not use nested loops.
 * - You may not declare any file-level variables.
 * - You may not use switch statements.
 * - You may not use the unsigned right shift operator (>>>)
 * - You may not write any helper methods, or call any method from this or
 *   another file to implement any method. Recursive solutions are not
 *   permitted.
 * - The only Java API methods you are allowed to invoke are:
 *     String.length()
 *     String.charAt()
 * - You may not invoke the above methods from string literals.
 *     Example: "12345".length()
 * - When concatenating numbers with Strings, you may only do so if the number
 *   is a single digit.
 *
 * Method-specific rules for this file:
 * - You may not use multiplication, division or modulus in any method, EXCEPT
 *   decimalStringToInt (where you may use multiplication only)
 * - You may declare exactly one String variable each in intToOctalString and
 *   and binaryStringToHexString.
 */
public class Bases
{
    /**
     * Convert a string containing ASCII characters (in binary) to an int.
     *
     * You do not need to handle negative numbers. The Strings we will pass in
     * will be valid binary numbers, and able to fit in a 32-bit signed integer.
     *
     * Example: binaryStringToInt("111"); // => 7
     */
    public static int binaryStringToInt(String binary)
    {
        int binaryInt = 0;
        for (int i = 0; i < binary.length(); i++) {
            int nextBit = binary.charAt(i) - 48; // 0 is 48
            binaryInt = (binaryInt << 1) + nextBit;
        }
        return binaryInt;
    }

    /**
     * Convert a string containing ASCII characters (in decimal) to an int.
     *
     * You do not need to handle negative numbers. The Strings we will pass in
     * will be valid decimal numbers, and able to fit in a 32-bit signed integer.
     *
     * Example: decimalStringToInt("46"); // => 46
     *
     * You may use multiplication in this method.
     */
    public static int decimalStringToInt(String decimal)
    {
        int decimalInt = 0;
        for (int i = 0; i < decimal.length(); i++) {
            int nextDigit = decimal.charAt(i) - 48; // 0 is 48
            decimalInt = (decimalInt * 10) + nextDigit;
        }
        return decimalInt;
    }

    /**
     * Convert a string containing ASCII characters (in hex) to an int.
     * The input string will only contain numbers and uppercase letters A-F.
     * You do not need to handle negative numbers. The Strings we will pass in will be
     * valid hexadecimal numbers, and able to fit in a 32-bit signed integer.
     *
     * Example: hexStringToInt("A6"); // => 166
     */
    public static int hexStringToInt(String hex)
    {
        int hexInt = 0;
        int nextChar;
        for (int i = 0; i < hex.length(); i++) {
            if (hex.charAt(i) >= 65) { // A-F
                nextChar = hex.charAt(i) - 55; // A=10 is 65
            } else { // 0-9
                nextChar = hex.charAt(i) - 48; // 0 is 48
            }
            hexInt = (hexInt << 4) + nextChar;
        }
        return hexInt;
    }

    /**
     * Convert a int into a String containing ASCII characters (in octal).
     *
     * You do not need to handle negative numbers.
     * The String returned should contain the minimum number of characters
     * necessary to represent the number that was passed in.
     *
     * Example: intToOctalString(166); // => "246"
     *
     * You may declare one String variable in this method.
     */
    public static String intToOctalString(int octal)
    {
        String octalString = "";
        while (octal != 0) {
            // convert the last 3 bits of int octal to an ASCII value
            int nextDigit = (octal & 0b111) + 48;
            // add the new ASCII character (in octal) to the string
            octalString = (char) (nextDigit) + octalString;
            // shift int octal to get the next 3 bits
            octal = octal >> 3;
        }
        return octalString;
    }

    /**
     * Convert a String containing ASCII characters representing a number in
     * binary into a String containing ASCII characters that represent that same
     * value in hex.
     *
     * The output string should only contain numbers and capital letters.
     * You do not need to handle negative numbers.
     * All binary strings passed in will contain exactly 32 characters.
     * The hex string returned should contain exactly 8 characters.
     *
     * Example: binaryStringToHexString("00001111001100101010011001011100"); // => "0F32A65C"
     *
     * You may declare one String variable in this method.
     */
    public static String binaryStringToHexString(String binary)
    {
        String hexString = "";
        for (int i = 0; i < binary.length(); i += 4) {
            int bit1 = (binary.charAt(i) - 48) << 3;
            int bit2 = (binary.charAt(i + 1) - 48) << 2;
            int bit3 = (binary.charAt(i + 2) - 48) << 1;
            int bit4 = (binary.charAt(i + 3) - 48);
            int nextChar = bit1 + bit2 + bit3 + bit4;
            if (nextChar < 10) { // 0-9
                hexString += (char) (nextChar + 48); // 0 is 48
            } else { // A-F
                hexString += (char) (nextChar + 55); // A=10 is 65
            }
        }
        return hexString;
    }
}
