//
// Created by vsensu on 2020/7/19.
//

#ifndef NODECRAFT_COMMANDS_H
#define NODECRAFT_COMMANDS_H

#include <vector>

#include "entt.hpp"

class Command
{
public:
    virtual ~Command() {}
    virtual void execute() = 0;
    virtual void undo() = 0;
};

class CompressedCommands : public Command
{
public:
    using Command::Command;

    void push(Command *cmd) { cmds_.push_back(cmd); }

    void execute() override
    {
        for (const auto &e : cmds_) {
            e->execute();
        }
    }

    void undo() override
    {
        for (int i = cmds_.size()-1; i >= 0; --i) {
            cmds_[i]->undo();
        }
    }

private:
    std::vector<Command*> cmds_;
};



class Cmd_CreateEntity : public Command
{
public:
    Cmd_CreateEntity() = delete;
    Cmd_CreateEntity(entt::registry &registry) : registry_(registry) {}
    entt::entity entity() const { return entity_; }
    void execute() override { entity_ = registry_.create(); }
    void undo() override { registry_.destroy(entity_); }
private:
    entt::registry &registry_;
    entt::entity entity_;
};

template<typename TComponent>
class Cmd_AddComponent : public Command
{
public:
    Cmd_AddComponent() = delete;
    Cmd_AddComponent(entt::registry &registry, entt::entity entity, const TComponent &comp) : registry_(registry), entity_(entity), comp_(comp) {}

    void execute() override { registry_.emplace<TComponent>(entity_, comp_); }
    void undo() override { registry_.remove<TComponent>(entity_); }
private:
    entt::registry &registry_;
    entt::entity entity_;
    TComponent comp_;
};

//template<typename TComponent>
//class Cmd_ModComponent<> : public Command
//{};

#endif //NODECRAFT_COMMANDS_H
