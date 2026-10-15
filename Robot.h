#pragma once
#include <iosfwd>
#include <string>
#include <vector>

class Task;

enum class RobotState {
    Available,
    Busy,
    Faulty,
    Unavailable
};

std::string toString(RobotState state);

class Robot {
public:
    Robot(int id, std::string name, double x, double y, double resource);

    int getId() const;
    const std::string& getName() const;
    double getX() const;
    double getY() const;
    double getResource() const;
    RobotState getState() const;
    bool hasCurrentTask() const;
    int getCurrentTaskId() const;
    const std::vector<int>& getAssignedTaskIds() const;

    bool isAvailable() const;
    bool hasEnoughResource(const Task& task) const;
    bool canExecute(const Task& task) const;

    double distanceTo(double x, double y) const;
    double distanceTo(const Task& task) const;

    void setState(RobotState state);
    void markAvailable();
    void markBusy();
    void markFaulty();
    void markUnavailable();

    bool assignTask(const Task& task);
    bool unassignTask(int taskId);
    bool completeTask(const Task& task);

    void moveTo(double x, double y);
    void consumeResource(double amount);
    void recharge(double amount);

    void print(std::ostream& os) const;

private:
    static constexpr int NO_TASK = -1;

    int id_;
    std::string name_;
    double x_, y_;
    double resource_;
    RobotState state_;
    int currentTaskId_;
    std::vector<int> assignedTaskIds_;
};

std::ostream& operator<<(std::ostream& os, const Robot& robot);