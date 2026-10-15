#include "Assignment.h"
#include <ostream>
#include <stdexcept>

std::string toString(AssignmentState state) {
    switch (state) {
    case AssignmentState::Active:    return "активне";
    case AssignmentState::Completed: return "виконане";
    case AssignmentState::Cancelled: return "скасоване";
    }
    return "невідомий стан";
}
Assignment::Assignment(int robotId, int taskId, double distance, double expectedTime, double cost)
    : robotId_(robotId), taskId_(taskId), distance_(distance), expectedTime_(expectedTime), cost_(cost), state_(AssignmentState::Active) {
    if (distance < 0.0 || expectedTime < 0.0 || cost < 0.0)
        throw std::invalid_argument("Відстань, час і вартість не можуть бути від'ємними");
}
int Assignment::getRobotId() const { return robotId_; }
int Assignment::getTaskId() const { return taskId_; }
double Assignment::getDistance() const { return distance_; }
double Assignment::getExpectedTime() const { return expectedTime_; }
double Assignment::getCost() const { return cost_; }
AssignmentState Assignment::getState() const { return state_; }
bool Assignment::isActive() const { return state_ == AssignmentState::Active; }
bool Assignment::markCompleted() {
    if (!isActive()) return false;
    state_ = AssignmentState::Completed;
    return true;
}
bool Assignment::markCancelled() {
    if (!isActive()) return false;
    state_ = AssignmentState::Cancelled;
    return true;
}
void Assignment::print(std::ostream& os) const {
    os << "Призначення: робот №" << robotId_ << ", завдання №" << taskId_
        << ", відстань: " << distance_
        << ", очікуваний час: " << expectedTime_
        << ", вартість: " << cost_
        << ", стан: " << toString(state_);
}
std::ostream& operator<<(std::ostream& os, const Assignment& a) {
    a.print(os);
    return os;
}