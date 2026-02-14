#pragma once

#include <Arduino.h>
#include <BoardProfile.h>

class Esp32Task : public ITaskManager
{
    TaskHandle_t mTaskHandle = nullptr;

public:
    Esp32Task() = default;
    explicit Esp32Task(TaskConfig *config);
    void deleteTask() override;
    int createTask(TaskConfig *config) override;
};
