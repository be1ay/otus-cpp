#pragma once
#include <memory>
#include <algorithm>
#include "Observer.h"

using ObserverPtr = std::shared_ptr<Observer>;
class BulkNotifier {
public:
    void addObserver(ObserverPtr obs) {
        observers_.push_back(obs);
    }

    void removeObserver(ObserverPtr obs) {
        observers_.erase(std::remove(observers_.begin(), observers_.end(), obs), observers_.end());
    }

    void notify(const VectorCommands& commands) const {
        for (auto &obs : observers_) {
            if (obs) obs->onBulk(commands);
        }
    }

private:
    std::vector<ObserverPtr> observers_;
};