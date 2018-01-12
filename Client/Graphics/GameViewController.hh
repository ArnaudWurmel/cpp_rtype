#ifndef GAMEVIEWCONTROLLER_HPP
#define GAMEVIEWCONTROLLER_HPP

#include "AViewController.hh"
#include "RootViewController.hh"

namespace rtp {
class GameViewController : public AViewController {
public:
  explicit GameViewController(RootViewController &);
  ~GameViewController() override;
  bool render() override;

private:
  RootViewController &_rootViewController;
  bool _continue;
};
} // namespace rtp

#endif
