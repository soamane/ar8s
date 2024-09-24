#include "ServiceExecutor.hpp"

#include <iostream>
#include <thread>

ServiceExecutor::ServiceExecutor() {
    this->SetupSettings();
}

ServiceExecutor::~ServiceExecutor() { }

void ServiceExecutor::Execute() { }

void ServiceExecutor::SetupSettings() { }

std::string ServiceExecutor::ReplacePhonePlaceholder(const std::string& str,
    std::string_view value) { }

void ServiceExecutor::ShowServiceInfo(const Service& service) { }
