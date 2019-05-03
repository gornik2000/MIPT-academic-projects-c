//===========================================================================*/
//                                                                           */
//  example of function sum asm in C, including C print log info from asm    */
//                                                                           */
//  1) asm files:                                                            */
//  nasm -f elf sum.asm                                                      */
//  ...                                                                      */
//  2) c file:                                                               */
//  gcc -c -m32 main.c                                                       */
//  3)linking:                                                               */
//  gcc -m32 -o a.out main.o sum.o ...                                       */
//                                                                           */
//===========================================================================*/

#include <stdio.h>
extern int _my_sum (int, ...);

int main (void)
{
	printf ("\nThe sum is %d!\n", _my_sum (1, 1, 2, 3, 5, 8, 13, 21, 0));
	return 0;
}

//---------------------------------------------------------------------------*/
//                      © Gorbachev Nikita, April 2019                       //
//---------------------------------------------------------------------------*/
