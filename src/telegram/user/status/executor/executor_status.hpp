#ifndef EXECUTOR_STATUS_HPP
#define EXECUTOR_STATUS_HPP

struct ExecutorStatus {
    bool attackInProgress;
    bool attackStopped;

    void clear();
};

#endif // !EXECUTOR_STATUS_HPP