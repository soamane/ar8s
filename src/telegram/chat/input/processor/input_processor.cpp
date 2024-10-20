#include "input_processor.hpp"

#include "../handler/phone/phone_handler.hpp"
#include "../validator/string/string_validator.hpp"

InputProcessor::InputProcessor(std::shared_ptr<MessageHandler> messageHandler) : m_messageHandler(messageHandler) { }

void InputProcessor::ProcessPhoneNumber(UserData& user, TgBot::Message::Ptr message) {
    if (!StringValidator::IsRussianPhoneNumber(message->text)) {
        this->m_messageHandler->SendErrorMessage(message->chat->id, message->messageId, "❌ Некорректный формат номера телефона.\n\nУбедитесь в правильности введеного номера и повторите попытку. В случае непредвиденного поведения обратитесь к разработчику: @soamane");
        return;
    }

    user.input.phone = PhoneHandler::FormatPhoneNumber(message->text);
    if (user.input.phone.empty()) {
        throw std::runtime_error("Failed to format input phone number");
    }

    user.inputStatus.phoneEntered = !user.input.phone.empty();

    if (user.inputStatus.phoneEntered) {
        this->m_messageHandler->SendChatMessage(message->chat->id, "📲 Номер телефона успешно считан.\n\n⏳ Введите желаемое количество итераций. Учтите, что максимальное количество не должно превышать 100");
    } else {
        this->m_messageHandler->SendErrorMessage(message->chat->id, message->messageId, "❌ Некорректный номер телефона.\n\nУбедитесь в правильности введеного номера и повторите попытку. В случае непредвиденного поведения обратитесь к разработчику: @soamane");
    }

}

void InputProcessor::ProcessAttackCount(UserData& user, TgBot::Message::Ptr message) {
    if (!StringValidator::IsDigitOnly(message->text)) {
        this->m_messageHandler->SendErrorMessage(message->chat->id, message->messageId, "❌ Некорректное значение.\n\nУбедитесь в правильности введеных данных. В случае непредвиденного поведения обратитесь к разработчику: @soamane");
        return;
    }

    try {
        user.input.attacksCount = std::stoi(message->text);
        if (user.input.attacksCount < 1 || user.input.attacksCount > 100) {
            this->m_messageHandler->SendErrorMessage(message->chat->id, message->messageId, "❌ Количество итераций должно быть от 1 до 100.\n\nУбедитесь в правильности введеных данных. В случае непредвиденного поведения обратитесь к разработчику: @soamane");
            return;
        }

        user.inputStatus.attacksEntered = true;
        this->m_messageHandler->SendChatMessage(message->chat->id, "🎲 Количество итераций для проведения атаки: " + std::to_string(user.input.attacksCount) + "\n\nЧтобы начать спам по указанному номеру, введите команду /execute");
    } catch ([[maybe_unused]] const std::exception& e) {
        this->m_messageHandler->SendErrorMessage(message->chat->id, message->messageId, "❌ Некорректный ввод.\n\nУбедитесь в правильности введеных данных. В случае непредвиденного поведения обратитесь к разработчику: @soamane");
    }
}