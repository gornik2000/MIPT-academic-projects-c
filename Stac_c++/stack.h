//------------------------------------------------------------------------------
#include <iostream>
#include <fstream>
#include <ctime>

using namespace std;
//-----------------------------------------------------------------------------
typedef size_t stack_data;
typedef int    stack_pars;

const stack_data STACK_CANARY       = 184876268;
const stack_data STACK_POISON       = 0;
const stack_data MIN_STACK_CAPACITY = 16;

const float INC_CAPACITY_COEFF = 2;
const float DEC_CAPACITY_COEFF = 0.5;
//-----------------------------------------------------------------------------
class stack
{
  private:
    stack_data *data_;
    stack_pars size_;
    stack_pars capacity_;
    ofstream   log_file_;

    char change_capacity (float change_value);
    char check ();

  public:
    stack  ();
    ~stack ();

    char       push (stack_data element);
    stack_data pop  ();

    stack_pars return_capacity ();
    stack_pars return_size     ();

    char print_data (ofstream &stream);
};
//------------------------------------------------------------------------------

