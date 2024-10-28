#include "input_processor.hpp"

#include "../format/input_format.hpp"
#include "../validator/input_validator.hpp"

InputProcessor::InputProcessor(std::shared_ptr<UserData> userData, std::shared_ptr<MessageHandler> messageHandler)
    : m_user(userData), m_messageHandler(messageHandler) {

}

void InputProcessor::ProcessPhoneNumber(std::string_view message) {
    if (!InputValidator::IsRussianPhoneNumber(message)) {
        this->m_messageHandler->SendChatMessage("❌ Некорректный формат номера телефона.\n\nУбедитесь в правильности введеного номера и повторите попытку. В случае непредвиденного поведения обратитесь к разработчику: @soamane");
        return;
    }

    this->m_user->input->phone = InputFormat::FormatPhoneNumber(message);
    this->m_user->input->status->phoneEntered = !this->m_user->input->phone.empty();

    if (this->m_user->input->status->phoneEntered) {
        this->m_messageHandler->SendChatMessage("📲 Номер телефона успешно считан.");
        this->m_messageHandler->SendChatMessage("⏳ Введите количество минут для атаки. Учтите, что максимальное количество не должно превышать 100");
    } else {
        this->m_messageHandler->SendChatMessage("❌ Некорректный номер телефона.\n\nУбедитесь в правильности введеного номера и повторите попытку. В случае непредвиденного поведения обратитесь к разработчику: @soamane");
    }
}


void InputProcessor::ProcessAttackTime(std::string_view message) {
    if (!InputValidator::IsDigitOnly(message)) {
        this->m_messageHandler->SendChatMessage("❌ Некорректное значение.\n\nУбедитесь в правильности введеных данных. В случае непредвиденного поведения обратитесь к разработчику: @soamane");
        return;
    }

    try {
        this->m_user->input->attackTime = std::stoi(message.data());
        if (this->m_user->input->attackTime < 1 || this->m_user->input->attackTime > 100) {
            this->m_messageHandler->SendChatMessage("❌ Количество минут должно быть от 1 до 100.\n\nУбедитесь в правильности введеных данных. В случае непредвиденного поведения обратитесь к разработчику: @soamane");
            return;
        }

        this->m_user->input->status->attackTimeEntered = true;

        this->m_messageHandler->SendChatMessage("🎲 Количество минут для проведения атаки: " + std::to_string(m_user->input->attackTime) + "\n\nЧтобы начать спам по указанному номеру, введите команду /execute");
    } catch ([[maybe_unused]] const std::exception& e) {
        this->m_messageHandler->SendChatMessage("❌ Некорректный ввод.\n\nУбедитесь в правильности введеных данных. В случае непредвиденного поведения обратитесь к разработчику: @soamane");
    }
}