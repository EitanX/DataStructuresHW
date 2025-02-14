#pragma once

#include "record_manager.h"

class Jockey;

class Team {
    struct RootData {
        RecordNode recordNode {};
        int size = 1;
        int record = 0;
        int teamId = 0;
    };

    struct NonRootData {
        Team* parent = nullptr;
    };

    RootData rootData;
    NonRootData nonRootData;

public:
    explicit Team(const int id) {
        this->rootData.teamId = id;
    }

    bool isRoot() const {
        return this->nonRootData.parent == nullptr;
    }

    void resetRootData() {
        this->rootData = RootData();
    }

    Team* getParent() const {
        return this->nonRootData.parent;
    }

    void setParent(Team* parent) {
        this->nonRootData.parent = parent;
    }

    int getSize() const {
        return this->rootData.size;
    }

    void setSize(const int size) {
        this->rootData.size = size;
    }

    int getRecord() const {
        return this->rootData.record;
    }

    void setRecord(const int record) {
        this->rootData.record = record;
    }

    RecordNode getRecordNode() const {
        return this->rootData.recordNode;
    }

    void setRecordNode(const RecordNode recordNode) {
        this->rootData.recordNode = recordNode;
    }

    int getTeamId() const {
        return this->rootData.teamId;
    }

    void setTeamId(const int teamId) {
        this->rootData.teamId = teamId;
    }
};
