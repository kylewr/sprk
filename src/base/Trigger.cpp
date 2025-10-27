#include "Trigger.hpp"

#include <iostream>

Trigger& Trigger::create(std::function<bool()> condition) {
    auto trigger = std::shared_ptr<Trigger>(new Trigger(condition));
    TriggerManager::getInstance().add(trigger);
    return *trigger;
}

TriggerManager::~TriggerManager() {
    // Don't delete triggers - they manage their own lifetime
    triggers.clear();
}

void TriggerManager::process() {
    for (const auto& trigger : triggers) {
        if (!trigger) {
            continue;
        }
        if (trigger->isDisabled()) {
            continue;
        }

        bool lastState = trigger->getLastState();
        bool currentState;

        try {
            currentState = trigger->get();
        } catch (...) {
            // skip if condition throws an error
            continue;
        }

        if (lastState != currentState) {
            std::function<void()> action =
                currentState ? trigger->getOnTrueAction() : trigger->getOnFalseAction();
            if (action) {
                action();
            }
        }

        std::function<void()> whileAction =
            currentState ? trigger->getWhileTrueAction() : trigger->getWhileFalseAction();
        if (whileAction) {
            whileAction();
        }
    }
}
