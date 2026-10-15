#include "Robot.h"
#include "Task.h"
#include <algorithm>
#include <cmath>
#include <ostream>
#include <stdexcept>
#include <utility>

std::string toString(RobotState state) {
    switch (state) {
    case RobotState::Available:   return "доступний";
    case RobotState::Busy:        return "зайнятий";
    case RobotState::Faulty:      return "несправний";
    case RobotState::Unavailable: return "недоступний";
    }
    return "невідомий стан";
}

Robot::Robot(int id, std::string name, double x, double y, double resource)
    : id_(id), name_(std::move(name)), x_(x), y_(y), resource_(resource),
    state_(RobotState::Available), currentTaskId_(NO_TASK) {
    if (resource < 0.0)
        throw std::invalid_argument("Робот: ресурс не може бути від'ємним");
}

int Robot::getId() const { return id_; }
const std::string& Robot::getName() const { return name_; }
double Robot::getX() const { return x_; }
double Robot::getY() const { return y_; }
double Robot::getResource() const { return resource_; }
RobotState Robot::getState() const { return state_; }
bool Robot::hasCurrentTask() const { return currentTaskId_ != NO_TASK; }
int Robot::getCurrentTaskId() const { return currentTaskId_; }

const std::vector<int>& Robot::getAssignedTaskIds() const {
    return assignedTaskIds_;
}

bool Robot::isAvailable() const { return state_ == RobotState::Available; }

void Robot::setState(RobotState state) { state_ = state; }
void Robot::markAvailable() { state_ = RobotState::Available; }
void Robot::markBusy() { state_ = RobotState::Busy; }
void Robot::markFaulty() { state_ = RobotState::Faulty; }
void Robot::markUnavailable() { state_ = RobotState::Unavailable; }

bool Robot::hasEnoughResource(const Task& task) const {
    return resource_ >= task.getResourceRequired();
}

bool Robot::canExecute(const Task& task) const {
    return isAvailable() && hasEnoughResource(task);
}

double Robot::distanceTo(double x, double y) const {
    return std::hypot(x - x_, y - y_);
}

double Robot::distanceTo(const Task& task) const {
    return distanceTo(task.getX(), task.getY());
}

bool Robot::assignTask(const Task& task) {
    if (!canExecute(task)) return false;
    assignedTaskIds_.push_back(task.getId());
    currentTaskId_ = task.getId();
    state_ = RobotState::Busy;
    return true;
}

bool Robot::unassignTask(int taskId) {
    auto it = std::find(assignedTaskIds_.begin(), assignedTaskIds_.end(), taskId);
    if (it == assignedTaskIds_.end()) return false;
    assignedTaskIds_.erase(it);
    if (currentTaskId_ == taskId) {
        currentTaskId_ = NO_TASK;
        if (state_ == RobotState::Busy) state_ = RobotState::Available;
    }
    return true;
}

bool Robot::completeTask(const Task& task) {
    if (currentTaskId_ != task.getId()) return false;
    if (!hasEnoughResource(task)) return false;
    x_ = task.getX();
    y_ = task.getY();
    resource_ -= task.getResourceRequired();
    assignedTaskIds_.erase(
        std::remove(assignedTaskIds_.begin(), assignedTaskIds_.end(), task.getId()),
        assignedTaskIds_.end());
    currentTaskId_ = NO_TASK;
    if (state_ == RobotState::Busy) state_ = RobotState::Available;
    return true;
}

void Robot::moveTo(double x, double y) {
    x_ = x;
    y_ = y;
}

void Robot::consumeResource(double amount) {
    if (amount < 0.0)
        throw std::invalid_argument("Робот: кількість не може бути від'ємною");
    if (amount > resource_)
        throw std::invalid_argument("Робот: недостатньо ресурсу");
    resource_ -= amount;
}

void Robot::recharge(double amount) {
    if (amount < 0.0)
        throw std::invalid_argument("Робот: кількість не може бути від'ємною");
    resource_ += amount;
}

void Robot::print(std::ostream& os) const {
    os << "Робот №" << id_ << " «" << name_ << "»"
        << ", координати: (" << x_ << "; " << y_ << ")"
        << ", ресурс: " << resource_
        << ", стан: " << toString(state_)
        << ", поточне завдання: ";
    if (hasCurrentTask()) os << currentTaskId_; else os << "немає";
    os << ", призначені завдання: [";
    for (size_t i = 0; i < assignedTaskIds_.size(); ++i) {
        if (i) os << ", ";
        os << assignedTaskIds_[i];
    }
    os << "]";
}

std::ostream& operator<<(std::ostream& os, const Robot& robot) {
    robot.print(os);
    return os;
}