#pragma once

#include <algorithm>
#include <functional>
#include <memory>
#include <vector>

class Trigger : public std::enable_shared_from_this<Trigger> {
    public:
        static Trigger& create(std::function<bool()> condition);
        ~Trigger() = default;

        inline bool get() {
            lastState = condition();
            return lastState;
        }

        inline bool getLastState() const {
            return lastState;
        }

        inline bool isDisabled() const {
            return disabled;
        }

        void setDisabled(bool isDisabled) {
            disabled = isDisabled;
        }

        inline std::function<void()> getOnTrueAction() const {
            return onTrueAction;
        }

        inline std::function<void()> getOnFalseAction() const {
            return onFalseAction;
        }

        inline std::function<void()> getWhileTrueAction() const {
            return whileTrueAction;
        }

        inline std::function<void()> getWhileFalseAction() const {
            return whileFalseAction;
        }

        Trigger& onTrue(std::function<void()> action) {
            onTrueAction = action;
            return *this;
        }

        Trigger& onFalse(std::function<void()> action) {
            onFalseAction = action;
            return *this;
        }

        Trigger& whileTrue(std::function<void()> action) {
            whileTrueAction = action;
            return *this;
        }

        Trigger& whileFalse(std::function<void()> action) {
            whileFalseAction = action;
            return *this;
        }

    private:
        Trigger(std::function<bool()> condition) : condition(condition) {};

        std::function<bool()> condition;
        bool disabled {false};

        bool lastState {false};

        std::function<void()> onTrueAction;
        std::function<void()> onFalseAction;
        std::function<void()> whileTrueAction;
        std::function<void()> whileFalseAction;
};

class TriggerManager {
    public:
        static TriggerManager& getInstance() {
            // whatever "Meyere's singleton" is lol
            static TriggerManager instance;
            return instance;
        }

        ~TriggerManager();

        void add(std::shared_ptr<Trigger> trigger) {
            triggers.push_back(trigger);
        }

        void remove(std::shared_ptr<Trigger> trigger) {
            triggers.erase(std::remove(triggers.begin(), triggers.end(), trigger), triggers.end());
        }

        void process();

    private:
        TriggerManager() {}
        TriggerManager(const TriggerManager&) = delete;
        TriggerManager& operator=(const TriggerManager&) = delete;
        TriggerManager(TriggerManager&&) = delete;
        TriggerManager& operator=(TriggerManager&&) = delete;

        std::vector<std::shared_ptr<Trigger>> triggers;
};
