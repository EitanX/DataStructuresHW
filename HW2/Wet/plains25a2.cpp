// You can edit anything you want in this file.
// However, you need to implement all public Plains function, as provided below as a template

#include "plains25a2.h"
#include "jockey.h"

Plains::Plains() = default;

Plains::~Plains() = default;

StatusType Plains::add_team(const int teamId) {
    if (teamId <= 0) {
        return StatusType::INVALID_INPUT;
    }

    try {
        if (this->teamManager.createTeam(teamId) == nullptr) {
            return StatusType::FAILURE;
        }
    }
    catch (const std::bad_alloc&) {
        return StatusType::ALLOCATION_ERROR;
    }

    return StatusType::SUCCESS;
}

StatusType Plains::add_jockey(const int jockeyId, const int teamId) {
    if (jockeyId <= 0 || teamId <= 0) {
        return StatusType::INVALID_INPUT;
    }

    if (this->jockeys.get(jockeyId) != nullptr) {
        return StatusType::FAILURE;
    }

    Team* team = this->teamManager.lookupRootTeam(teamId);
    if (team == nullptr) {
        return StatusType::FAILURE;
    }

    try {
        this->jockeys.insert(jockeyId, new Jockey(team));
    }
    catch (const std::bad_alloc&) {
        return StatusType::ALLOCATION_ERROR;
    }

    return StatusType::SUCCESS;
}

StatusType Plains::update_match(const int victoriousJockeyId, const int losingJockeyId) {
    if (victoriousJockeyId <= 0 || losingJockeyId <= 0 || victoriousJockeyId == losingJockeyId) {
        return StatusType::INVALID_INPUT;
    }

    Jockey* victoriousJockey = this->jockeys.get(victoriousJockeyId);
    Jockey* losingJockey = this->jockeys.get(losingJockeyId);
    if (victoriousJockey == nullptr || losingJockey == nullptr) {
        return StatusType::FAILURE;
    }

    Team* victoriousTeam = this->teamManager.findRoot(victoriousJockey->getTeam());
    Team* losingTeam = this->teamManager.findRoot(losingJockey->getTeam());
    if (victoriousTeam == losingTeam) {
        return StatusType::FAILURE;
    }

    try {
        this->recordManager.addPoints(victoriousTeam, 1);
        this->recordManager.addPoints(losingTeam, -1);

        victoriousJockey->increaseRecord();
        losingJockey->decreaseRecord();
    }
    catch (const std::bad_alloc&) {
        return StatusType::ALLOCATION_ERROR;
    }

    return StatusType::SUCCESS;
}

StatusType Plains::merge_teams(const int teamId1, const int teamId2) {
    if (teamId1 <= 0 || teamId2 <= 0 || teamId1 == teamId2) {
        return StatusType::INVALID_INPUT;
    }

    Team* team1 = this->teamManager.lookupRootTeam(teamId1);
    Team* team2 = this->teamManager.lookupRootTeam(teamId2);
    if (team1 == nullptr || team2 == nullptr) {
        return StatusType::FAILURE;
    }

    try {
        if (this->teamManager.uniteTeams(team1, team2) == nullptr) {
            return StatusType::FAILURE;
        }
    }
    catch (const std::bad_alloc&) {
        return StatusType::ALLOCATION_ERROR;
    }

    return StatusType::SUCCESS;
}

StatusType Plains::unite_by_record(const int record) {
    if (record <= 0) {
        return StatusType::INVALID_INPUT;
    }

    const auto positive = this->recordManager.lookupPositiveRecords(record);
    const auto negative = this->recordManager.lookupNegativeRecords(record);
    if (positive == nullptr || negative == nullptr) {
        return StatusType::FAILURE;
    }

    if (positive->getSize() != 1 || negative->getSize() != 1) {
        return StatusType::FAILURE;
    }

    Team* better = *positive->begin();
    Team* worse = *negative->begin();

    try {
        if (this->teamManager.uniteTeams(better, worse) == nullptr) {
            return StatusType::FAILURE;
        }
    }
    catch (const std::bad_alloc&) {
        return StatusType::ALLOCATION_ERROR;
    }

    return StatusType::SUCCESS;
}

output_t<int> Plains::get_jockey_record(const int jockeyId) {
    if (jockeyId <= 0) {
        return StatusType::INVALID_INPUT;
    }

    const Jockey* jockey = this->jockeys.get(jockeyId);
    if (jockey == nullptr) {
        return StatusType::FAILURE;
    }

    return jockey->getRecord();
}

output_t<int> Plains::get_team_record(const int teamId) {
    if (teamId <= 0) {
        return StatusType::INVALID_INPUT;
    }

    const Team* team = this->teamManager.lookupRootTeam(teamId);
    if (team == nullptr) {
        return StatusType::FAILURE;
    }

    return team->getRecord();
}
