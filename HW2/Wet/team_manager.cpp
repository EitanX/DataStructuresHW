#include "team_manager.h"

TeamManager::TeamManager(RecordManager& rm) : recordManager(rm) {}

Team* TeamManager::createTeam(const int teamId) {
    if (this->data.get(teamId) != nullptr) {
        return nullptr;
    }

    const auto team = new Team(teamId);
    this->data.insert(teamId, team);
    this->recordManager.addTeam(team);
    return team;
}

Team* TeamManager::lookupRootTeam(const int teamId) const {
    Team* team = this->data.get(teamId);
    if (team == nullptr) {
        return nullptr;
    }

    if (!team->isRoot()) {
        return nullptr;
    }

    return team;
}

Team* TeamManager::findRoot(Team* team) { // Assuming team isn't null
    if (team->isRoot()) {
        return team;
    }

    Team* root = this->findRoot(team->getParent());
    team->setParent(root); // Path compression
    return root;
}

Team* TeamManager::uniteTeams(Team* t1, Team* t2) { // Assuming t1, t2 aren't null
    Team* smaller = t1->getSize() < t2->getSize() ? t1 : t2;
    Team* bigger = t1->getSize() < t2->getSize() ? t2 : t1;

    const int biggerTeamId = bigger->getTeamId();
    const int betterTeamId = t1->getRecord() < t2->getRecord() ? t2->getTeamId() : t1->getTeamId();

    // Note that the exercise asked for union by record.
    // We actually do union by size, and make a trick for it to work as expected.
    // This is because union by size is required for O(log* m).

    smaller->setParent(bigger);
    bigger->setSize(bigger->getSize() + smaller->getSize());
    this->recordManager.addPoints(bigger, smaller->getRecord());
    this->recordManager.removeTeam(smaller);

    // Inherit the id of the better team (not necessarily the bigger one).
    bigger->setTeamId(betterTeamId);
    if (biggerTeamId != betterTeamId) { // The better team is the smaller one.
        // We changed the team id, so we better update the references.
        this->data.update(biggerTeamId, smaller);
        this->data.update(betterTeamId, bigger);
    }

    // For debugging purposes, don't keep outdated data.
    smaller->resetRootData();

    // The new root is the root of bigger.
    return bigger;
}
