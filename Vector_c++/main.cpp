//-----------------------------------------------------------------------------
#include "vector.cpp"
//-----------------------------------------------------------------------------
int main()
{
  int size = 5;
  vector a (size);
  for (int i = 0; i < size; i++)
  {
    a[i] = i*i;
  }
  vector b(5);
  cin >> b;
  cout << b;
  //b.print ();
  vector c;
  c = a + b;
  cout << c;
  cout << int (a == b) << endl << a * c << endl;
}
//-----------------------------------------------------------------------------