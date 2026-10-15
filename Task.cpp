#include "Task.h"
#include <ostream>
#include <stdexcept>
#include <utility>
#include <cmath>

std::string toString(TaskState state) {
    switch (state) {
    case TaskState::Pending:    return "очікує виконання";
    case TaskState::Assigned:   return "призначене";
    case TaskState::InProgress: return "виконується";
    case TaskState::Completed:  return "завершене";
    case TaskState::Cancelled:  return "скасоване";
    }
    return "невідомий стан";
}
Task::Task(int id, std::string name, double x, double y, int priority, double duration, double resourceRequired)
    : id_(id), name_(std::move(name)), x_(x), y_(y), priority_(priority), duration_(duration), resourceRequired_(resourceRequired), state_(TaskState::Pending)
{
        if (id < 0)
            throw std::invalid_argument("Ідентифікатор не може бути від'ємним");
        if (name_.find_first_not_of(" \t\n\r") == std::string::npos)
            throw std::invalid_argument("Назва не може бути порожньою");
        if (!std::isfinite(x) || !std::isfinite(y))
            throw std::invalid_argument("Координати мають бути скінченними числами");
        if (priority < MIN_PRIORITY || priority > MAX_PRIORITY)
            throw std::invalid_argument("Пріоритет має бути в межах від 1 до 5");
        if (!std::isfinite(duration))
            throw std::invalid_argument("Тривалість має бути скінченним числом");
        if (duration < 0.0)
            throw std::invalid_argument("Тривалість не може бути від'ємною");
        if (!std::isfinite(resourceRequired))
            throw std::invalid_argument("Потреба в ресурсі має бути скінченним числом");
        if (resourceRequired < 0.0)
            throw std::invalid_argument("Потреба в ресурсі не може бути від'ємною");
}
int Task::getId() const { return id_; }
const std::string& Task::getName() const { return name_; }
double Task::getX() const { return x_; }
double Task::getY() const { return y_; }
int Task::getPriority() const { return priority_; }
double Task::getDuration() const { return duration_; }
double Task::getResourceRequired() const { return resourceRequired_; }
TaskState Task::getState() const { return state_; }
bool Task::isPending() const { return state_ == TaskState::Pending; }
bool Task::isFinished() const {
    return state_ == TaskState::Completed || state_ == TaskState::Cancelled;
}
bool Task::transitionTo(TaskState next) {
    bool ok = false;
    switch (next) {
    case TaskState::Pending:
        ok = (state_ == TaskState::Assigned || state_ == TaskState::InProgress);
        break;
    case TaskState::Assigned:
        ok = (state_ == TaskState::Pending);
        break;
    case TaskState::InProgress:
        ok = (state_ == TaskState::Assigned);
        break;
    case TaskState::Completed:
        ok = (state_ == TaskState::InProgress);
        break;
    case TaskState::Cancelled:
        ok = !isFinished();
        break;
    }
    if (ok) state_ = next;
    return ok;
}
bool Task::markAssigned() { return transitionTo(TaskState::Assigned); }
bool Task::markInProgress() { return transitionTo(TaskState::InProgress); }
bool Task::markCompleted() { return transitionTo(TaskState::Completed); }
bool Task::markCancelled() { return transitionTo(TaskState::Cancelled); }
bool Task::resetToPending() { return transitionTo(TaskState::Pending); }
void Task::print(std::ostream& os) const {
    os << "Завдання №" << id_ << " «" << name_ << "»"
        << ", координати: (" << x_ << "; " << y_ << ")"
        << ", пріоритет: " << priority_
        << ", тривалість: " << duration_
        << ", потреба в ресурсі: " << resourceRequired_
        << ", стан: " << toString(state_);
}
std::ostream& operator<<(std::ostream& os, const Task& task) {
    task.print(os);
    return os;
}