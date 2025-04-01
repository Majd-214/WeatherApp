// IDisplayable.h
#ifndef IDISPLAYABLE_H
#define IDISPLAYABLE_H

#include <ostream>

// Interface for objects that can be displayed to an output stream
class IDisplayable {
  public:
  // Virtual destructor is crucial for base classes with virtual functions
  virtual ~IDisplayable() = default;

  // Pure virtual function makes this class abstract (an interface)
  virtual void display(std::ostream& os) const = 0;

  // Overload operator<< to use the display method polymorphically
  friend std::ostream& operator<<(std::ostream& os, const IDisplayable& disp) {
    disp.display(os);
    return os;
  }
};

#endif // IDISPLAYABLE_H