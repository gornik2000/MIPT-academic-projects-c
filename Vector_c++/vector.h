//------------------------------------------------------------------------------
#undef private
//------------------------------------------------------------------------------
#include <iostream>
#include <fstream>
#include <ctime>

using namespace std;
//-----------------------------------------------------------------------------
typedef size_t vector_data;
typedef int    vector_pars;

const vector_data VECTOR_POISON = 0;
//-----------------------------------------------------------------------------
class vector
{
  friend vector operator+ (const vector& a,      const vector& b);
  friend vector operator+ (const vector_data& a, const vector& b);
  friend vector operator+ (const vector& a,      const vector_data& b);

  friend vector operator- (const vector& a, const vector& b);
  friend vector operator- (const vector& a, const vector_data& b);

  friend vector      operator* (const vector_data& a, const vector& b);
  friend vector      operator* (const vector& a,      const vector_data& b);
  friend vector_data operator* (const vector& a,      const vector& b);

  friend vector operator/ (const vector& a, const vector_data& b);

  friend char operator== (const vector& a, const vector& b);

  friend ostream &operator<< (ostream& out, const vector& a);
  friend istream &operator>> (istream& in,  const vector& a);

  private:
    vector_data *data_;
    vector_pars capacity_;
  public:
    vector  ();
    vector  (const vector& a);
    vector  (vector_pars capacity);
    ~vector ();

    vector_pars capacity ();

    char print  ();
    char clear  (vector_data content = 0);
    char resize (vector_pars count);

    vector_data &operator[] (vector_pars index);
    vector      &operator=  (const vector& a);
};
//-----------------------------------------------------------------------------
