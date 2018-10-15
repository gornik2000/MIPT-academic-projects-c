//------------------------------------------------------------------------------
/*!
//  \file
//
//  \brief Main file which contains description of the program
*/
/*!
//  \mainpage Description
//
//  \author Nikita Gorbachev
//  \version 1.1
//  \date September 2018
//
//  This project takes a,b,c from the console and then prints
//  the solution of the quadratic equation x^2+bx+c=0,
//  including roots(if they exist) and their number
*/
//------------------------------------------------------------------------------

#include <stdio.h>
#include <assert.h>
#include <math.h>

//------------------------------------------------------------------------------
/*!
//  \var const int INFINITE
//
//  \brief Constant used in solveQuadraticEquation() and solveLinearEquation()
//         to describe infinite number of roots
*/
//------------------------------------------------------------------------------

const int INFINITE = 3;

//------------------------------------------------------------------------------
/*!
//  \var const int EPSILON
//
//  \brief Constant used in isEqual() to compare double numbers with this
//         precision
*/
//------------------------------------------------------------------------------

const double EPSILON = 0.0000001;


int solveQuadraticEquation (double a, double b, double c,
                            double *rootOne, double *rootTwo);


int solveLinearEquation (double a, double b, double *rootLinear);


int isEqual (double numberOne, double numberTwo);


int main ()
{
  printf (" # Enter  3 coefficients \"a b c\" of the"
          " quadratic equation (ax^2+bx+c=0)\n\n   ");

  double a = 0.0;
  double b = 0.0;
  double c = 0.0;

  int coefficientsNumber = scanf (" %lf %lf %lf", &a, &b, &c);
  // fflush fseek
  printf (" \n");

  if (coefficientsNumber != 3)
  {
    printf (" # ERROR\n");
    printf (" # Scanf returned invalid value of inputed"
              " coefficients :\n\n   %d\n", coefficientsNumber);
    return 0;
  }

  assert (!isnan (a));
  assert (!isnan (b));
  assert (!isnan (c));

  double rootOne    = 0.0;
  double rootTwo    = 0.0;
  int numberOfRoots = solveQuadraticEquation(a, b, c, &rootOne, &rootTwo);

  assert (!isnan (rootOne));
  assert (!isnan (rootTwo));
  assert (!isnan (numberOfRoots));

  switch (numberOfRoots)
  {
    case 2 :
      printf (" # There are two roots of the quadratic equation :\n\n"
              "   %lf and %lf\n", rootOne, rootTwo);
      break;

    case 1 :
      printf (" # There is one root of the quadratic equation :\n\n"
              "   %lf\n", rootOne);
      break;

    case 0 :
      printf (" # There are no roots of the quadratic equation\n\n");
      break;

    case INFINITE :
      printf (" # There are infinite roots of the quadratic equation\n\n");
      break;

    default :
      printf (" # ERROR\n");
      printf (" # solveQuadraticEquation returned invalid value"
              " of roots :\n\n   %d\n", numberOfRoots);
      break;
  }

  return 0;
}

//------------------------------------------------------------------------------
/*!
//  \fn int solveQuadraticEquation(double a, double b, double c,
//                                 double *rootOne, double *rootTwo)
//
//  \brief Solves the quadratic equation ax^2+bx+c=0
//  \param a,b,c coefficients a,b,c of the quadratic equation
//  \param rootOne,rootTwo Pointers to roots of the equation
//  \return Number of roots: 0,1,2 or INFINITE
*/
//------------------------------------------------------------------------------

int solveQuadraticEquation (double a, double b, double c,
                            double *rootOne, double *rootTwo)
{
  assert (rootOne);
  assert (rootTwo);
  assert (rootOne != rootTwo);
  assert (!isnan (a));
  assert (!isnan (b));
  assert (!isnan (c));

  if (isEqual (a, 0.0))
  {
    /* bx+c=0 or bx=0 or c=0 or 0=0 */
    return solveLinearEquation (b, c, rootOne);
  }

  if (isEqual (b, 0.0))
  {
    if (isEqual (c, 0.0))
    {
      /* ax^2=0 */
      *rootOne = 0.0;
      return 1;
    }
    /* ax^2+c=0 */
    if (a * c < 0.0)
    {
      *rootOne = sqrt (- c / a);
      *rootTwo = - *rootOne;
      return 2;
    }
    return 0;
  }

  if (isEqual (c, 0.0))
  {
    /* ax^2+bx=0 */
    *rootTwo = 0.0;
    return solveLinearEquation (a, b, rootOne) + 1;
  }
  /* ax^2+bx+c=0 */
  double discriminant = b * b - 4.0 * a * c;

  if (isEqual (discriminant, 0.0))
  {
    *rootOne = - b / a / 2.0;
    return 1;
  }
  if (discriminant > 0.0)
  {
    double sqrtDiscriminant = sqrt (discriminant);

    *rootOne = (- b - sqrtDiscriminant) / a / 2.0;
    *rootTwo = (- b + sqrtDiscriminant) / a / 2.0;
    return 2;
  }
  return 0;
}

//------------------------------------------------------------------------------
/*!
//  \fn int solveLinearEquation(double a, double b, double *rootLinear)
//
//  \brief Solves the linear equation ax+b=0
//  \param a,b Coefficients a,b of the equation
//  \param rootLinear Pointer to root of the equation
//  \return Number of roots: 1 or INFINITE
*/
//------------------------------------------------------------------------------

int solveLinearEquation(double a, double b, double *rootLinear)
{
  assert (rootLinear);
  assert (!isnan (a));
  assert (!isnan (b));

  if (isEqual (a, 0.0))
  {
    return INFINITE;
  }
  *rootLinear = - b / a;
  return 1;
}

//------------------------------------------------------------------------------
/*!
//  \fn int isEqual(double numberOne, double numberTwo)
//
//  \brief Compare two double numbers with precision EPSILON
//  \param numberOne,numberTwo The numbers we want to compare
//  \return 1 if numbers are equal with precision EPSILON, 0 if not
*/
//------------------------------------------------------------------------------

int isEqual(double numberOne, double numberTwo)
{
  assert (!isnan (numberOne));
  assert (!isnan (numberTwo));

  return fabs (numberOne - numberTwo) < EPSILON;
}

