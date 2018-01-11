#include "ViewController.hpp"

void ViewController::pushView(IView &View) { _stackView.push(View); }

void ViewController::popView() { _stackView.pop(); }

IView &ViewController::topView() { return _stackView.top(); }
