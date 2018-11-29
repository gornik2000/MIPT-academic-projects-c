//-----------------------------------------------------------------------------
#include "differentiator.cpp"
//-----------------------------------------------------------------------------
int main ()
{
  printf (" # Differentiator 2.0\n"
          " # This program helps to count derivatives of the inputted data \n"
          " # Input expression in \"data.txt\" without enter and"
          " with all brackets\n"
          " # Derivative can be found in \"out.txt\". Good luck! \n\n");

  int diffNumber = 0;
  printf (" # enter Derivative number ");
  scanf  ("%d", &diffNumber);
  printf ("\n");

  differentiator ("data.txt", "out.txt", diffNumber);
  printf (" # Derivative was successfully counted\n"
          " # Thanks for using this program\n");
}
//-----------------------------------------------------------------------------
