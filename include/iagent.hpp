#pragma once
enum class AgentType { Algae, Herbivore, Carnivore };
class IAgent{
    public:
    virtual int getLife() = 0;
    virtual void setLife(int life) = 0;
    virtual void addAge() = 0;
    virtual int getAge() = 0;
    virtual bool isDead() = 0;
    virtual AgentType getType() const = 0;
    virtual ~IAgent() = default;
};