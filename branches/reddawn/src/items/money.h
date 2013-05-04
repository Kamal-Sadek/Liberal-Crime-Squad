#ifndef MONEY_H
#define MONEY_H

#include "includes.h"

class Money : public Item
{
   public:
      explicit Money(long amount) : amount_(amount) {}
      virtual Money* clone() const { return new Money(*this); }
      explicit Money(const char * inputXml);      
      string showXml() const;
      
      virtual string equip_title() const { return ("$"+tostring(amount_)); }
      
      virtual const string& get_name() const { return itemtypename(); }
      virtual long get_fencevalue() const { return amount_ * number_; }
      
      virtual bool is_money() const { return true; }
      
      virtual Money* split(int number);
      virtual bool merge(Item& i);
      virtual bool sort_compare_special(Item* other) const;
      
      long get_amount() const { return amount_; }
      void set_amount(long amount) { amount_ = amount; }
      void add_amount(long amount) { amount_ += amount; }
      void take_all_from(Money& more) { amount_ += more.amount_; more.amount_ = 0; }
      void flatten() { amount_ = amount_ * number_; number_ = 1; }
   
   private:
      long amount_;
};

#endif //MONEY_H
