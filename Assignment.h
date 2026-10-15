#pragma once
#include <iosfwd>
#include <string>

enum class AssignmentState {Active, Completed, Cancelled};
std::string toString(AssignmentState state);

class Assignment {
public:
    Assignment(int robotId, int taskId, double distance, double expectedTime, double cost);
    int getRobotId() const;
    int getTaskId() const;
    double getDistance() const;
    double getExpectedTime() const;
    double getCost() const;
    AssignmentState getState() const;
    bool isActive() const;
    bool markCompleted();
    bool markCancelled();
    void print(std::ostream& os) const;

private:
    int robotId_;
    int taskId_;
    double distance_;
    double expectedTime_;
    double cost_;
    AssignmentState state_;
};
std::ostream& operator<<(std::ostream& os, const Assignment& a);