#include "input_processor.hpp"

#include "../format/input_format.hpp"
#include "../validator/input_validator.hpp"

InputProcessor::InputProcessor(std::shared_ptr<UserData> userData) : m_user(userData) { }

bool InputProcessor::ProcessPhoneNumber(std::string_view message) {
    if (!InputValidator::IsRussianPhoneNumber(message)) {
        return false;
    }

    this->m_user->input->phone = InputFormat::FormatPhoneNumber(message);
    this->m_user->input->status->phoneEntered = !this->m_user->input->phone.empty();

    return this->m_user->input->status->phoneEntered;
}


bool InputProcessor::ProcessAttackTime(std::string_view message) {
    if (!InputValidator::IsDigitOnly(message)) {
        return false;
    }

    try {
        int attackTime = std::stoi(message.data());

        if (attackTime < 1 || attackTime > 100) {
            return false;
        }

        this->m_user->input->attackTime = attackTime;
        this->m_user->input->status->attackTimeEntered = true;
        return true;

    } catch (const std::exception&) {
        return false;
    }
}
