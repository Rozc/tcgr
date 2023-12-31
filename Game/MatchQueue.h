//
// Created by UserName on 2023/11/23.
//

#ifndef TCGR_MATCHQUEUE_H
#define TCGR_MATCHQUEUE_H


// 匹配队列，按照玩家的隐藏分进行匹配
/* 考虑将隐藏分分为五段，优先匹配当前段位玩家，在等待一定时间后如果无法匹配，则匹配高一段位的玩家
 * 具体实现计划是使用五个优先队列，优先级是玩家的匹配等待时间，等待时间越长优先级越高，玩家加入是检查对应队列中是否存在玩家，如果存在则进行匹配，否则加入队列
 * 将玩家加入队列的同时设置一个定时器，在定时器超时时，将其实体浅拷贝一份加入到高一级的队列中，
 * 在匹配时采用延迟删除策略，在匹配成功时检测当前对象是否有效（因为可能在其他匹配队列中匹配成功了）
 *
 * 事实：进入匹配队列的玩家，要么匹配成功，要么被置于匹配队列队首。
 * 函数 matchProcess 将清空等待队列，尝试匹配所有玩家
 * 函数 awake 在有新玩家加入匹配队列时，尝试唤醒 matchProcess，以执行匹配。
 *
 * 改成生产者-消费者模式？生产者：由 ServerIO 提供请求匹配的玩家加入队列（还要做取消匹配），消费者：由 matchProcess 消费队列中的玩家，进行匹配
 * 以适用多线程匹配。
 *
 * */

#include <unordered_map>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "../Tools/Logger.h"
#include "../Net/ServerIO.h"


static const int RANK_NUM = 5;
static const int MATCH_TIMEOUT = 8;
static const int MATCH_CANCEL_TIMEOUT = 30;

static const int MIN_SCORE = 0, MAX_SCORE = 5000;
static const int RANK_SCORE[RANK_NUM] = {MIN_SCORE, 1000, 2000, 3000, MAX_SCORE};


struct Player {
    int fd;
    int score;
    time_t joinTime;
    bool deleted;
    Player(int fd, int score, time_t joinTime) {
        this->fd = fd;
        this->score = score;
        this->joinTime = joinTime;
        this->deleted = false;
    }
};


class MatchQueue {
private:
    std::unordered_map<int, Player*> _playerMap; // fd -> player
    Player* _matchingPlayers[RANK_NUM]{};
    std::queue<Player*> _waitingQueue;

    std::thread _matchingThread;
    std::mutex _mx;
    std::condition_variable _cv;

    bool _matching;

    MatchQueue() : _matchingThread(&MatchQueue::matchingThreadFunc, this) {
        _matching = false;
        _matchingThread.detach();

        for (auto & _matchingPlayer : _matchingPlayers) {
            _matchingPlayer = nullptr;
        }
        Tools::logger.Log(LOG_FOCUS, "MatchQueue inited, matching thread running.");
    }


    static int _getRankByScore(int score) {
        for (int i = 0; i < RANK_NUM; ++i) {
            if (score <= RANK_SCORE[i]) {
                return i;
            }
        }
        // should not reach here
        Tools::logger.Log(LOG_ERROR, "MatchQueue::_getRankByScore: score out of range");
        return RANK_NUM - 1;
    }

    void matchingThreadFunc() {
        // consumer
        std::unique_lock<std::mutex> lock(_mx);
        while (true) {

            while (_waitingQueue.empty()) {
                _cv.wait(lock);
            }
            Tools::logger.Log(LOG_DEBUG, "Matching Process Start.");
            _matching = true;
            while (!_waitingQueue.empty()) {
                Player *player = _waitingQueue.front();
                _waitingQueue.pop();
                if (player->deleted) {
                    continue;
                }
                int rank = _getRankByScore(player->score);
                if (_matchingPlayers[rank] == nullptr) {
                    _matchingPlayers[rank] = player;
                } else {
                    // match success
                    Player *otherPlayer = _matchingPlayers[rank];
                    _matchingPlayers[rank] = nullptr;
                    Tools::logger.Log(LOG_INFO, "Match Success: ", player->fd, " vs ", otherPlayer->fd);
                    // TODO: 匹配成功要告诉 ServerIO
                }
            }
            Tools::logger.Log(LOG_DEBUG, "Matching queue empty. Matching thread sleep.");
            _matching = false;
        }

    }

public:
    void enqueue(int fd, int score) {
        // auto player = std::make_shared<Player>(fd, score, time(nullptr));
        auto* player = new Player(fd, score, time(nullptr));
        _playerMap[fd] = player;
        std::unique_lock<std::mutex> lock(_mx);
        Tools::logger.Log(LOG_DEBUG, "Player ", fd, " enqueue.");
        _waitingQueue.push(player);
        lock.unlock();
        _cv.notify_one();
    }

    static MatchQueue& getInstance() {
        static MatchQueue instance;
        return instance;
    }
    MatchQueue(MatchQueue&) = delete;
    MatchQueue(MatchQueue&&) = delete;
    MatchQueue& operator=(MatchQueue&) = delete;
    MatchQueue& operator=(MatchQueue&&) = delete;
};


#endif //TCGR_MATCHQUEUE_H
