// IDisplayable.h
#ifndef IDISPLAYABLE_H
#define IDISPLAYABLE_H

#include <ostream> // For std::ostream

// Abstract interface for classes that can display themselves to an output stream.
class IDisplayable {
  public:
  // Virtual destructor is essential for proper cleanup in inheritance hierarchies.
  virtual ~IDisplayable() = default;

  // Pure virtual function: concrete derived classes MUST implement this method.
  // Defines the contract for how an object should display itself.
  virtual void display(std::ostream& os) const = 0;

  // Friend function overload for the << operator.
  // Allows objects implementing IDisplayable to be directly used with output streams
  // (e.g., std::cout << myDisplayableObject;), invoking the virtual display() method.
  friend std::ostream& operator<<(std::ostream& os, const IDisplayable& disp) {
    disp.display(os); // Polymorphic call to the object's specific display method
    return os;
  }
};

#endif // IDISPLAYABLE_H