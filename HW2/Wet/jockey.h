#pragma once

class Team;

class Jockey {
    Team* team = nullptr;
    int record = 0;

public:
    explicit Jockey(Team* team) : team(team) {}

    Team* getTeam() const {
        return this->team;
    }

    int getRecord() const {
        return this->record;
    }

    void increaseRecord() {
        ++this->record;
    }

    void decreaseRecord() {
        --this->record;
    }
};
