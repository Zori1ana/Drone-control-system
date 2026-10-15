#pragma once
#include <iosfwd>
#include <string>

enum class TaskState {Pending, Assigned, InProgress, Completed, Cancelled};
std::string toString(TaskState state);

class Task {
public:
    static constexpr int MIN_PRIORITY = 1;
    static constexpr int MAX_PRIORITY = 5;
    Task(int id, std::string name, double x, double y, int priority, double duration, double resourceRequired);
    int getId() const;
    const std::string& getName() const;
    double getX() const;
    double getY() const;
    int getPriority() const;
    double getDuration() const;
    double getResourceRequired() const;
    TaskState getState() const;
    bool markAssigned();
    bool markInProgress();
    bool markCompleted();
    bool markCancelled();
    bool resetToPending();
    bool isPending() const;
    bool isFinished() const;
    void print(std::ostream& os) const;

private:
    bool transitionTo(TaskState next);
    int id_;
    std::string name_;
    double x_, y_;
    int priority_;
    double duration_;
    double resourceRequired_;
    TaskState state_;
};

std::ostream& operator<<(std::ostream& os, const Task& task);