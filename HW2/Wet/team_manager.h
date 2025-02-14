#pragma once

#include "hash_table.h"
#include "team.h"

class TeamManager {
public:
    explicit TeamManager(RecordManager& rm);

    Team* lookupRootTeam(int teamId) const;
    Team* createTeam(int teamId);
    Team* findRoot(Team* team);
    Team* uniteTeams(Team* t1, Team* t2);

private:
    RecordManager& recordManager;
    HashTable<Team> data;
};
