#include <cassert>
#include <iomanip>
#include <iostream>

#include "project4.hpp"
#include "History.hpp"
#include "Transaction.hpp"

////////////////////////////////////////////////////////////////////////////////
// Definitions for Transaction class
////////////////////////////////////////////////////////////////////////////////
//
//

// Constructor
// TASK 1
//
Transaction::Transaction(std::string ticker_symbol,
                         unsigned int day_date,
                         unsigned int month_date,
                         unsigned int year_date,
                         bool buy_sell_trans,
                         unsigned int number_shares,
                         double trans_amount)
{
  symbol = ticker_symbol;
  day = day_date;
  month = month_date;
  year = year_date;
  if (buy_sell_trans == true)
    trans_type = "Buy";
  else
    trans_type = "Sell";
  shares = number_shares;
  amount = trans_amount;
  trans_id = assigned_trans_id;
  ++assigned_trans_id;
}

// Destructor
// TASK 1
//
Transaction::~Transaction()
{
}

// Overloaded < operator.
// TASK 2
//
bool Transaction::operator<(Transaction const &other)
{

  if (this->year < other.year)
    return true;
  else if (this->year == other.year && this->month < other.month)
    return true;
  else if (this->year == other.year && this->month == other.month && this->day < other.day)
    return true;

  return false;
}

// GIVEN
// Member functions to get values.
//
std::string Transaction::get_symbol() const { return symbol; }
unsigned int Transaction::get_day() const { return day; }
unsigned int Transaction::get_month() const { return month; }
unsigned int Transaction::get_year() const { return year; }
unsigned int Transaction::get_shares() const { return shares; }
double Transaction::get_amount() const { return amount; }
double Transaction::get_acb() const { return acb; }
double Transaction::get_acb_per_share() const { return acb_per_share; }
unsigned int Transaction::get_share_balance() const { return share_balance; }
double Transaction::get_cgl() const { return cgl; }
bool Transaction::get_trans_type() const { return (trans_type == "Buy") ? true : false; }
unsigned int Transaction::get_trans_id() const { return trans_id; }
Transaction *Transaction::get_next() { return p_next; }

// GIVEN
// Member functions to set values.
//
void Transaction::set_acb(double acb_value) { acb = acb_value; }
void Transaction::set_acb_per_share(double acb_share_value) { acb_per_share = acb_share_value; }
void Transaction::set_share_balance(unsigned int bal) { share_balance = bal; }
void Transaction::set_cgl(double value) { cgl = value; }
void Transaction::set_next(Transaction *p_new_next) { p_next = p_new_next; }

// GIVEN
// Print the transaction.
//
void Transaction::print()
{
  std::cout << std::fixed << std::setprecision(2);
  std::cout << std::setw(4) << get_trans_id() << " "
            << std::setw(4) << get_symbol() << " "
            << std::setw(4) << get_day() << " "
            << std::setw(4) << get_month() << " "
            << std::setw(4) << get_year() << " ";

  if (get_trans_type())
  {
    std::cout << "  Buy  ";
  }
  else
  {
    std::cout << "  Sell ";
  }

  std::cout << std::setw(4) << get_shares() << " "
            << std::setw(10) << get_amount() << " "
            << std::setw(10) << get_acb() << " " << std::setw(4) << get_share_balance() << " "
            << std::setw(10) << std::setprecision(3) << get_acb_per_share() << " "
            << std::setw(10) << std::setprecision(3) << get_cgl()
            << std::endl;
}

////////////////////////////////////////////////////////////////////////////////
// Definitions for the History class
////////////////////////////////////////////////////////////////////////////////
//
//

// Constructor
// TASK 3
//
History::History()
{
  p_head = nullptr;
}

// Destructor
// TASK 3
//
History::~History()
{
  Transaction *p_tmp{p_head};
  while (p_tmp != nullptr)
  {
    p_tmp = p_tmp->get_next();
    delete p_head;
    p_head = p_tmp;
  }
}

// read_history(...): Read the transaction history from file.
// TASK 4
//
void History::read_history()
{
  ece150::open_file();
  while (ece150::next_trans_entry() == true)
  {
    Transaction *tmp = new Transaction(ece150::get_trans_symbol(), ece150::get_trans_day(), ece150::get_trans_month(),
                                       ece150::get_trans_year(), ece150::get_trans_type(), ece150::get_trans_shares(),
                                       ece150::get_trans_amount());

    insert(tmp);
  }
  ece150::close_file();
}

// insert(...): Insert transaction into linked list.
// TASK 5
//
void History::insert(Transaction *p_new_trans)
{
  if (p_head == nullptr)
  {
    this->p_head = p_new_trans;
  }
  else
  {
    Transaction *p_tr = p_head;

    while (p_tr->get_next() != nullptr)
    {
      p_tr = p_tr->get_next();
    }

    p_tr->set_next(p_new_trans);
  }
}

// sort_by_date(): Sort the linked list by trade date.
// TASK 6
//

void History::sort_by_date()
{
  /*
  if (p_head->get_next() == nullptr || p_head == nullptr)
  {
    return;
  }
  Transaction *p_sorted{p_head};
  p_head = p_head->get_next();
  p_sorted->set_next(nullptr);
  while (p_head != nullptr)
  {
    Transaction *p_insert = p_head;
    p_insert->set_next(nullptr);
    p_head = p_head->get_next();
    // p_insert < all elements in p_sorted, put it at the very start (answered by WEEFTA)
    if (*p_insert < *p_sorted)
    {
      p_insert->set_next(p_sorted);
      p_sorted = p_insert;
    }
    else{
    Transaction *p_temp = p_sorted;
    // Lopp through the p_temp, find the position before the p_insert + 1 (Answered by WEEFTA)
    while (p_temp->get_next() != nullptr && (*(p_temp->get_next())< *p_insert))
    {
      p_temp = p_temp -> get_next();
    }
    p_insert->set_next(p_temp->get_next());
    p_temp->set_next(p_insert);
    }
  }
  p_head = p_sorted;
  */
}

// update_acb_cgl(): Updates the ACB and CGL values.
// TASK 7
//

void History::update_acb_cgl()
{
  Transaction *p_get{p_head};
  Transaction *p_last{p_get};

  while (p_get != nullptr)
  {
    if (p_get == p_last)
    {
      p_get->set_acb(p_get->get_amount());
      p_get->set_share_balance(p_get->get_shares());
      p_get->set_acb_per_share(p_get->get_acb() / p_get->get_share_balance());
      p_get->set_cgl(0.0);
      p_get = p_get->get_next();
    }
    else
    {
      if (p_get->get_trans_type() == true)
      {
        p_get->set_share_balance(p_get->get_shares() + p_last->get_share_balance());
        p_get->set_acb(p_last->get_acb() + p_get->get_amount());
        p_get->set_acb_per_share(p_get->get_acb() / p_get->get_share_balance());
        p_get->set_cgl(0.0);
      }
      else
      {
        p_get->set_share_balance(p_last->get_share_balance() - p_get->get_shares());
        p_get->set_acb(p_last->get_acb() - (p_get->get_shares() * (p_last->get_acb_per_share())));
        p_get->set_acb_per_share(p_last->get_acb_per_share());
        p_get->set_cgl(p_get->get_amount() - (p_get->get_shares() * (p_last->get_acb_per_share())));
      }
      p_last = p_get;
      p_get = p_get->get_next();
    }
  }
  p_get = nullptr;
  p_last = nullptr;
}

// compute_cgl(): )Compute the ACB, and CGL.
// TASK 8
double History::compute_cgl(unsigned int year)
{
  double cgl_year{0};
  for (Transaction *temp{p_head}; temp != nullptr; temp = temp->get_next())
  {
    if (temp->get_year() == year)
    {
      cgl_year += temp->get_cgl();
    }
  }
  return cgl_year;
}

// print() Print the transaction history.
//TASK 9
//
void History::print()
{
  if (p_head != nullptr)
  {
    std::cout << "========== BEGIN TRANSACTION HISTORY ============" << std::endl;
    Transaction *p_tmp = p_head;
    while (p_tmp != nullptr)
    {
      p_tmp->print();
      p_tmp = p_tmp->get_next();
    }
    delete p_tmp;
    p_tmp = nullptr;
    std::cout << "========== END TRANSACTION HISTORY ============" << std::endl;
  }
  else
  {
    std::cout << "========== BEGIN TRANSACTION HISTORY ============" << std::endl;
    std::cout << "========== END TRANSACTION HISTORY ============" << std::endl;
  }
}

// GIVEN
// get_p_head(): Full access to the linked list.
//
Transaction *History::get_p_head() { return p_head; }
