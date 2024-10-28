#include "user_input.hpp"

UserInput::UserInput() : attackTime(0), phone(""), status(std::make_unique<InputStatus>()) { }