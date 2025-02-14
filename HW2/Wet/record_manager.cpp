#include "record_manager.h"
#include "team.h"

int absValue(const int x) {
    return x > 0 ? x : -x;
}

LinkedList<Team*>* RecordManager::lookupPositiveRecords(const int record) const {
    return this->positiveRecords.get(record);
}

LinkedList<Team*>* RecordManager::lookupNegativeRecords(const int record) const {
    return this->negativeRecords.get(record);
}

void RecordManager::addTeam(Team* team) {
    team->setRecordNode(this->neutralRecords.insert(team));
}

void RecordManager::addPoints(Team* team, const int points) {
    if (points == 0) {
        return;
    }

    const auto recordNode = team->getRecordNode();
    const int oldRecord = team->getRecord();
    const int newRecord = oldRecord + points;

    team->setRecord(newRecord);

    HashTable<LinkedList<Team*>>* src = nullptr;
    auto srcList = &this->neutralRecords;
    if (oldRecord != 0) {
        src = oldRecord < 0 ? &this->negativeRecords : &this->positiveRecords;
        srcList = src->get(absValue(oldRecord));;
    }

    HashTable<LinkedList<Team*>>* dest = nullptr;
    auto destList = &this->neutralRecords;
    if (newRecord != 0) {
        dest = newRecord < 0 ? &this->negativeRecords : &this->positiveRecords;
        destList = dest->get(absValue(newRecord));

        // The dest might not exist
        if (destList == nullptr) {
            destList = new LinkedList<Team*>();
            dest->insert(absValue(newRecord), destList);
        }
    }

    srcList->removeSymbolically(recordNode);
    destList->insertSymbolically(recordNode);

    // If after removal the source is empty (and is not neutral), free the memory
    if (srcList->empty() && src != nullptr) {
        src->remove(absValue(oldRecord));
        delete srcList;
    }
}

void RecordManager::removeTeam(const Team* team) {
    const auto recordNode = team->getRecordNode();
    const int record = team->getRecord();
    if (record == 0) {
        this->neutralRecords.remove(recordNode);
        return;
    }

    auto& src = record < 0 ? this->negativeRecords : this->positiveRecords;
    const auto list = src.get(absValue(record));
    list->remove(recordNode);

    // If after removal the source is empty, free the memory
    if (list->empty()) {
        src.remove(absValue(record));
        delete list;
    }
}
