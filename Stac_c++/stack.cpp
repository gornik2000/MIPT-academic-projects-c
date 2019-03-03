//------------------------------------------------------------------------------
#include "stack.h"
//-----------------------------------------------------------------------------
stack::stack ():
  data_     (new stack_data [MIN_STACK_CAPACITY + 2]{0}),
  capacity_ (MIN_STACK_CAPACITY),
  size_     (0),
  log_file_ (0)
  {
    log_file_.open ("stack_log_file.txt");

    data_[0]                      = STACK_CANARY;
    data_[MIN_STACK_CAPACITY + 1] = STACK_CANARY;

    log_file_ << time (NULL) << ": stack creation\n";
    cout << "\n Hello! I was born!\n";
  }
//-----------------------------------------------------------------------------
stack::~stack ()
{
  print_data (log_file_);
  delete[] data_;
  data_ = NULL;

  log_file_ << time(NULL) << ": stack destruction\n";
  log_file_.close ();

  cout << "\n Mr user, i don't feel so good\n";
}
//-----------------------------------------------------------------------------
char stack::push (stack_data element)
{
  char error = check();
  if (error != 0)
  {
    return error;
  }

  if (capacity_ >= size_ + 1)
  {
    size_++;
    data_[size_] = element;
    log_file_ << time(NULL) << ": pushed " << element << endl;

    return 0;
  }

  change_capacity(INC_CAPACITY_COEFF);
  size_ ++;
  data_[size_] = element;

  return 0;
}
//-----------------------------------------------------------------------------
stack_data stack::pop ()
{
  char error = check();
  if (error != 0)
  {
    return error;
  }

  if (size_ > 0)
  {
    stack_data element = data_[size_];
    data_[size_] = STACK_POISON;
    size_--;

    log_file_ << time(NULL) << ": popped " << element << endl;
    return element;
  }

  return STACK_POISON;
}
//-----------------------------------------------------------------------------
char stack::change_capacity (float change_value)
{
  char error = check();
  if (error != 0)
  {
    return error;
  }

  if ((change_value > 1) || (capacity_ >= 1 / change_value))
  {
    stack_pars old_capacity = capacity_;
    capacity_               = stack_pars (capacity_ * change_value);
    stack_data *new_data    = new stack_data [capacity_ + 2]{0};

    for (int i = 0; i < old_capacity + 1; i++)
    {
      new_data[i] = data_[i];
    }
    new_data[capacity_ + 1] = STACK_CANARY;

    data_ = new_data;
    log_file_ << time(NULL) << ": changed capacity to " << capacity_ << endl;
  }

  return 0;
}
//-----------------------------------------------------------------------------
stack_pars stack::return_capacity ()
{
  return capacity_;
}
//-----------------------------------------------------------------------------
stack_pars stack::return_size ()
{
  return size_;
}
//-----------------------------------------------------------------------------
char stack::print_data (ofstream &stream)
{
  stream << time (NULL) << ": stack size: " << size_
                        << ", capacity: "   << capacity_
                        << ", canaries: "   << data_[0]
                        << " and "          << data_[capacity_ + 1]
                        << ", data:\n";

  for (int i = 1; i < size_ + 1 ; i ++)
  {
    stream << data_[i] << " ";
  }

  stream << endl;
  return 0;
}
//-----------------------------------------------------------------------------
char stack::check ()
{
  char error = 0;

  if (log_file_ == NULL)
  {
    cout << "ERROR! Log find was not found!\n";
    return 1;
  }
  if (data_ == NULL)
  {
    log_file_ << "ERROR! Null data pointer!\n";
    print_data (log_file_);
    return 2;
  }
  if (size_ > capacity_)
  {
    log_file_ << "ERROR! Current size > capacity!\n";
    print_data (log_file_);
    error = 3;
  }

  if (data_[0] != STACK_CANARY)
  {
    log_file_ << "ERROR! First canary was broken!\n";
    print_data (log_file_);
    error = 4;
  }
  if (data_[capacity_ + 1] != STACK_CANARY)
  {
    log_file_ << "ERROR! Last canary was broken!\n";
    print_data (log_file_);
    error = 5;
  }
  if (capacity_ == 0)
  {
    log_file_ << "ERROR! Zero capacity!\n";
    print_data (log_file_);
    error = 6;
  }

  return error;
}
//------------------------------------------------------------------------------
