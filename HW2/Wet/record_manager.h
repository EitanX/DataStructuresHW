#pragma once

#include "hash_table.h"
#include "linked_list.h"

class Team;

using RecordNode = LinkedList<Team*>::Iterator;

class RecordManager {
    LinkedList<Team*> neutralRecords;
    HashTable<LinkedList<Team*>> positiveRecords;
    HashTable<LinkedList<Team*>> negativeRecords;

public:
    LinkedList<Team*>* lookupPositiveRecords(int record) const;
    LinkedList<Team*>* lookupNegativeRecords(int record) const;

    void addTeam(Team* team);
    void addPoints(Team* team, int points);
    void removeTeam(const Team* team);
};
