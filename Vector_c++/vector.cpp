//-----------------------------------------------------------------------------
#include "vector.h"
//-----------------------------------------------------------------------------
vector::vector ():
  data_     (NULL),
  capacity_ (0)
  {}
//-----------------------------------------------------------------------------
vector::vector (const vector& a):
  data_     (new vector_data [a.capacity_]{0}),
  capacity_ (a.capacity_)
  {
    for (int i = 0; i < capacity_; i++)
    {
      data_[i] = a.data_[i];
    }
  }
//-----------------------------------------------------------------------------
vector::vector (vector_pars capacity):
  data_     (new vector_data [capacity]{0}),
  capacity_ (capacity)
  {}
//-----------------------------------------------------------------------------
vector::~vector ()
{
  clear (VECTOR_POISON);
  delete[] data_;

  capacity_ = -1;
  data_     = NULL;
}
//-----------------------------------------------------------------------------
vector_pars vector::capacity ()
{
  return capacity_;
}
//-----------------------------------------------------------------------------
char vector::clear (vector_data content)
{
  for (int i = 0; i < capacity_; i++)
  {
    data_[i] = content;
  }

  return 0;
}
//-----------------------------------------------------------------------------
char vector::resize (vector_pars count)
{
  if (capacity_ <  count)
  {
    vector_data *new_data = new vector_data [count]{0};
    for (int i = 0; i < capacity_; i++)
    {
      new_data[i] = data_[i];
    }
    delete[] data_;
    data_ = new_data;

    return 0;
  }
  if (capacity_ >  count)
  {
    vector_data *new_data = new vector_data [count]{0};
    for (int i = 0; i < count; i++)
    {
      new_data[i] = data_[i];
    }
    delete[] data_;
    data_ = new_data;

    return 0;
  }
  return 0;
}
//-----------------------------------------------------------------------------
char vector::print ()
{
  for (int i = 0; i < capacity_; i++)
  {
    cout << data_[i] << " ";
  }
  cout << endl;

  return 0;
}
//-----------------------------------------------------------------------------
vector_data &vector::operator[] (vector_pars index)
{
  return data_[index];
}
//-----------------------------------------------------------------------------
vector &vector::operator= (const vector& a)
{
  clear (VECTOR_POISON);
  delete[] data_;

  vector_data *copy_data = new vector_data[a.capacity_];
  data_     = copy_data;
  capacity_ = a.capacity_;

  for (int i = 0; i < capacity_; i++)
  {
    data_[i] = a.data_[i];
  }

  return *this;
}
//-----------------------------------------------------------------------------
vector operator+ (const vector& a, const vector& b)
{
  vector result (b.capacity_);

  for (int i = 0; i < b.capacity_; i++)
  {
    result[i] = a.data_[i] + b.data_[i];
  }

  return result;
}
//-----------------------------------------------------------------------------
vector  operator+ (const vector_data& a, const vector& b)
{
  vector result (b.capacity_);

  for (int i = 0; i < b.capacity_; i++)
  {
    result[i] = a + b.data_[i];
  }

  return result;
}
//-----------------------------------------------------------------------------
vector  operator+ (const vector& a, const vector_data& b)
{
  vector result (a.capacity_);

  for (int i = 0; i < a.capacity_; i++)
  {
    result[i] = b + a.data_[i];
  }

  return result;
}
//-----------------------------------------------------------------------------
vector  operator- (const vector& a, const vector& b)
{
  vector result (a.capacity_);

  for (int i = 0; i < a.capacity_; i++)
  {
    result[i] = a.data_[i] - b.data_[i];
  }

  return result;
}
//-----------------------------------------------------------------------------
vector  operator- (const vector& a, const vector_data& b)
{
  vector result (a.capacity_);

  for (int i = 0; i < a.capacity_; i++)
  {
    result[i] = a.data_[i] - b;
  }

  return result;
}
//-----------------------------------------------------------------------------
vector  operator* (const vector_data& a, const vector& b)
{
  vector result (b.capacity_);

  for (int i = 0; i < b.capacity_; i++)
  {
    result[i] = a * b.data_[i];
  }

  return result;
}
//-----------------------------------------------------------------------------
vector  operator* (const vector& a, const vector_data& b)
{
  vector result (a.capacity_);

  for (int i = 0; i < a.capacity_; i++)
  {
    result[i] = a.data_[i] * b;
  }

  return result;
}
//-----------------------------------------------------------------------------
vector_data  operator* (const vector& a, const vector& b)
{
  vector_data result (0);

  for (int i = 0; i < b.capacity_; i++)
  {
    result  = result + a.data_[i] * b.data_[i];
  }

  return result;
}
//-----------------------------------------------------------------------------
vector operator/ (const vector& a, const vector_data& b)
{
  vector result (a.capacity_);

  for (int i = 0; i < a.capacity_; i++)
  {
    result[i] = a.data_[i] / b;
  }

  return result;
}
//-----------------------------------------------------------------------------
char operator== (const vector& a, const vector& b)
{
  vector_pars capacity = min (a.capacity_, b.capacity_);
  for (int i = 0; i < capacity; i++)
  {
    if (a.data_[i] != b.data_[i]) return 0;
  }

  return 1;
}
//-----------------------------------------------------------------------------
ostream &operator<< (ostream& out, const vector& a)
{
  for (int i = 0; i < a.capacity_; i++)
  {
    out << a.data_[i] << " ";
  }
}
//-----------------------------------------------------------------------------
istream &operator>> (istream& in, const vector& a)
{
  for (int i = 0; i < a.capacity_; i++)
  {
    in >> a.data_[i];
  }
}
//-----------------------------------------------------------------------------
