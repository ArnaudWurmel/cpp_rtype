#ifndef VIEWCONTROLLER_HPP
#define VIEWCONTROLLER_HPP

#include "IView.hpp"
#include <stack>

class ViewController {
public:
  ViewController(){};
  ~ViewController(){};
  void pushView(IView &);
  void popView();
  IView &topView();

private:
  std::stack<IView> _stackView;
};

#endif
