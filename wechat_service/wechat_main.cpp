//
//  main.cpp
//  wechat_watch_check
//
//  Created by 吴敦为 on 2021/12/30.
//

//以下是微信手表侧sdk会依赖到的头文件
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <string>
#include <cstdlib>
#include <cstdint>
#include <cstddef>
#include <cassert>
#include <climits>
#include <sstream>
#include <cstdio>
#include <pthread.h>
#include <iostream>
#include <memory>
#include <deque>
#include <map>
#include <queue>
#include <list>
#include <vector>
#include <algorithm>
#include <time.h>
#include <limits.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>

//用于文件系统接口测试的目录
#define FILESYSTEM_TEST_DIR  "./testdir"

//如果不支持CLOCK_MONOTONIC的话就需要支持c++11的chrono
#define SUPPORT_CLOCK_MONOTONIC 1
#ifndef SUPPORT_CLOCK_MONOTONIC
#error you should define if support CLOCK_MONOTONIC or not
#else
#if not SUPPORT_CLOCK_MONOTONIC
#include <chrono>
#endif
#endif

#define RUN_MBEDTLS_TEST 1
#if RUN_MBEDTLS_TEST
#include "mbedtls/gcm.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/ecdh.h"
#include "mbedtls/ecdsa.h"
#include "mbedtls/sha256.h"
#include "mbedtls/base64.h"
#include "mbedtls/pk.h"
#include "cJSON.h"

std::string test_lic = "YjBgAgEBBBkApPKc9bvhkKUgRuUYtUHykr11bK6xlBDhoAoGCCqGSM49AwEBoTQDMgAEGql2weaafQC8uQZLIMx/8TwLT5b/bNMjQ3qflw0luW1oqnv7YUPNCSxd3R5326o6SzBJMBMGByqGSM49AgEGCCqGSM49AwEBAzIABMLMt9t0qKgJYQMO510W6U0i3kC8NCQgrb/pKLQigILFebNJj37uqbhUEddJwOE1sg==";

#endif

#define adam_printf printf

typedef bool (*test_func_entry)();
std::list<test_func_entry> g_task_list;
uint32_t g_scuess_tasks=0;


uint64_t __gettickcount() {
#if SUPPORT_CLOCK_MONOTONIC
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
#else
    //use c++11 systemtime
    auto tp = std::chrono::system_clock::now();
    auto tp_msec = std::chrono::duration_cast<std::chrono::milliseconds>(tp.time_since_epoch()).count();
    return tp_msec;
#endif
}

class adam_test_log {
public:
    // Use constructor to write log of entering a scope.
    adam_test_log(const char* funcname) {
        m_scuess_ = true;
        m_funcname.append(funcname);
        s_time_ = __gettickcount();
        adam_printf("[ RUN      ] %s start.\r\n", funcname);
    }
    // Use destructor to write log of exiting a scope normally or with a exception.
    ~adam_test_log() {
        if (m_scuess_) {
            adam_printf("[       OK ] %s end. (%llu ms total)\r\n\r\n", m_funcname.c_str(), __gettickcount() - s_time_);
        } else {
            adam_printf("[  FAILED  ] %s end. (%llu ms total)\r\n\r\n", m_funcname.c_str(), __gettickcount() - s_time_);
        }
    }
    void set_failed(){m_scuess_ = false;}
    bool get_state(){return m_scuess_;}
private:
    bool m_scuess_;
    std::string m_funcname;
    uint64_t s_time_;
};

class ThreadTestVal {
public:
    ThreadTestVal() {
        pthread_mutex_init(&m_mutex, NULL);
    #if SUPPORT_CLOCK_MONOTONIC
        pthread_condattr_t  condattr;
        pthread_condattr_init(&condattr);
        pthread_condattr_setclock(&condattr, CLOCK_MONOTONIC);
        pthread_cond_init(&m_condition, &condattr);
        pthread_condattr_destroy(&condattr);
    #else
        pthread_cond_init(&m_condition, NULL);
    #endif
    }
    ~ThreadTestVal() {
        pthread_mutex_destroy(&m_mutex);
        pthread_cond_destroy(&m_condition);
    }
    void Lock() {
        pthread_mutex_lock(&m_mutex);
    }
    void Unlock() {
        pthread_mutex_unlock(&m_mutex);
    }
    void Wait() {
        pthread_cond_wait(&m_condition, &m_mutex);
    }
    int TimeWait(uint32_t timeout_ms) {
        struct timespec abstime;
#if SUPPORT_CLOCK_MONOTONIC
        clock_gettime(CLOCK_MONOTONIC, &abstime);
        abstime.tv_sec  += timeout_ms / 1000;
        abstime.tv_nsec += (timeout_ms % 1000)*1000000;
        if (abstime.tv_nsec >= 1000000000) {
            abstime.tv_nsec -= 1000000000;
            abstime.tv_sec++;
        }
#else
        auto tp = std::chrono::system_clock::now();
        uint64_t curtime = std::chrono::duration_cast<std::chrono::milliseconds>(tp.time_since_epoch()).count();
        abstime.tv_sec = curtime / 1000;
        abstime.tv_nsec = (curtime % 1000) * 1000000;
        abstime.tv_sec  += timeout_ms / 1000;
        abstime.tv_nsec += (timeout_ms % 1000)*1000000;
        if (abstime.tv_nsec >= 1000000000) {
            abstime.tv_nsec -= 1000000000;
            abstime.tv_sec++;
        }
#endif
        return pthread_cond_timedwait(&m_condition, &m_mutex, &abstime);
    }
    void Signal() {
        pthread_cond_signal(&m_condition);
    }
public:
    pthread_mutex_t m_mutex;
    pthread_cond_t m_condition;
};

class Stltestval {
public:
    Stltestval(){
        i = 0;
    }
    ~Stltestval(){}
    Stltestval& operator=(const Stltestval& tval) {
        if (this != &tval) {
            this->i = tval.i;
        }
        return *this;
    }
    void seti(uint32_t v){i=v;}
    uint32_t geti(){return i;}
private:
    uint32_t i;
};

struct Stlteststruct {
    Stlteststruct():ptrvar(NULL),uintvar(0){}
    
    Stltestval* ptrvar;
    uint64_t uintvar;
};

void adam_remove_dir(const char* pathname) {
    if (0 != access(pathname, 0)) {
        return;
    }
    char cur_dir[] = ".";
    char up_dir[] = "..";
    DIR *dirp;
    struct dirent *dp;
    struct stat dir_stat;
    if ( 0 > stat(pathname, &dir_stat) ) {
        return;
    }
    if (S_ISREG(dir_stat.st_mode) ) {
        remove(pathname);
    } else if (S_ISDIR(dir_stat.st_mode)) {
        dirp = opendir(pathname);
        while ((dp=readdir(dirp)) != NULL) {
            if ((0 == strcmp(cur_dir, dp->d_name)) || (0 == strcmp(up_dir, dp->d_name))) {
                continue;
            }
            char dir_name[128];
            sprintf(dir_name, "%s/%s", pathname, dp->d_name);
            adam_remove_dir(dir_name);
        }
        closedir(dirp);
        rmdir(pathname);
    }
}

std::deque<int> m_queue;
ThreadTestVal *var1;
ThreadTestVal *var2;
bool m_end_thred = false;
uint32_t m_recv_signals = 0;
uint32_t m_recv_queue = 0;

bool Test_stl_impl() {
    adam_test_log log_var(__func__);
    std::map<std::string, std::string> m_str_str_map;
    std::map<uint32_t, std::string> m_uint_str_map;
    std::map<uint32_t, uint64_t> m_uint_uint_map;
    std::map<uint8_t, Stltestval*> m_uint_prt_map;
    std::vector<uint32_t> m_uint_vector;
    std::queue<Stltestval*> m_prt_queue;
    std::deque<Stlteststruct> m_var_deque;
    std::list<Stltestval> m_var_list;
    //test map
    m_str_str_map.insert(std::make_pair("key1", "hello world!"));
    m_str_str_map.insert(std::make_pair("key2", "hello world2!"));
    std::map<std::string, std::string>::iterator str_str_map_iter = m_str_str_map.find("key1");
    if (str_str_map_iter == m_str_str_map.end()) {
        log_var.set_failed();
        adam_printf("[----------] std::map<std::string, std::string> find key failed.\r\n");
    } else if ((str_str_map_iter->second) != "hello world!") {
        log_var.set_failed();
        adam_printf("[----------] std::map<std::string, std::string> get key value failed.\r\n");
    }
    m_str_str_map.erase("key1");
    str_str_map_iter = m_str_str_map.find("key1");
    if (str_str_map_iter != m_str_str_map.end()) {
        log_var.set_failed();
        adam_printf("[----------] std::map<std::string, std::string> erase key failed.\r\n");
    }
    if (m_str_str_map.size() != 1) {
        log_var.set_failed();
        adam_printf("[----------] std::map<std::string, std::string> get size error.\r\n");
    }
    m_str_str_map.clear();
    if (m_str_str_map.size() != 0) {
        log_var.set_failed();
        adam_printf("[----------] std::map<std::string, std::string> clear error.\r\n");
    }
    if (m_uint_str_map.count(1)) {
        log_var.set_failed();
        adam_printf("[----------] std::map<uint32_t, std::string> count error.\r\n");
    }
    m_uint_str_map[1] = "hello world!";
    if (!m_uint_str_map.count(1)) {
        log_var.set_failed();
        adam_printf("[----------] std::map<uint32_t, std::string> key set error.\r\n");
    }
    if (m_uint_str_map[1] != "hello world!") {
        log_var.set_failed();
        adam_printf("[----------] std::map<uint32_t, std::string> key get value error.\r\n");
    }
    m_uint_uint_map[100] = 100;
    m_uint_prt_map[100] = NULL;
    m_uint_uint_map.clear();
    m_uint_prt_map.clear();
    //test vector
    m_uint_vector.push_back(10);
    m_uint_vector.push_back(20);
    m_uint_vector.push_back(30);
    if (m_uint_vector.size() != 3) {
        log_var.set_failed();
        adam_printf("[----------] std::vector<uint32_t> push_back error.\r\n");
    }
    for (uint32_t i=0; i < m_uint_vector.size(); i++) {
        if (20 == m_uint_vector[i]) {
            m_uint_vector.erase(m_uint_vector.begin()+i);
            break;
        }
    }
    if (m_uint_vector.size() != 2) {
        log_var.set_failed();
        adam_printf("[----------] std::vector<uint32_t> erase error.\r\n");
    }
    m_uint_vector.clear();
    if (m_uint_vector.size() != 0) {
        log_var.set_failed();
        adam_printf("[----------] std::vector<uint32_t> clear error.\r\n");
    }
    //test queue
    Stltestval test1, test2;
    uint32_t queuesize = 0;
    test1.seti(10);
    test2.seti(20);
    m_prt_queue.push(&test1);
    m_prt_queue.push(&test2);
    if (m_prt_queue.size() != 2) {
        log_var.set_failed();
        adam_printf("[----------] std::queue<Stltestval*> push error.\r\n");
    }
    while(!m_prt_queue.empty()) {
        Stltestval* testptr = m_prt_queue.front();
        if (queuesize == 0) {
            if (testptr->geti() != 10) {
                log_var.set_failed();
                adam_printf("[----------] std::queue<Stltestval*> front error.\r\n");
            }
        } else {
            if (testptr->geti() != 20) {
                log_var.set_failed();
                adam_printf("[----------] std::queue<Stltestval*> pop error.\r\n");
            }
        }
        m_prt_queue.pop();
        queuesize++;
    }
    if (queuesize != 2) {
        log_var.set_failed();
        adam_printf("[----------] std::queue<Stltestval*> empty error.\r\n");
    }
    //test deque
    Stlteststruct dequetest0, dequetest1, dequetest2;
    dequetest1.uintvar = 1;
    dequetest2.uintvar = 2;
    m_var_deque.push_back(dequetest0);
    m_var_deque.push_back(dequetest1);
    m_var_deque.push_back(dequetest2);
    if (m_var_deque.size() != 3) {
        log_var.set_failed();
        adam_printf("[----------] std::deque<Stlteststruct> push_back error.\r\n");
    }
    for (uint64_t i = 0; i < m_var_deque.size();i++) {
        if (m_var_deque[i].uintvar != i) {
            log_var.set_failed();
            adam_printf("[----------] std::deque<Stlteststruct> get val error req:%llu, acutal:%llu.\r\n", i, m_var_deque[i].uintvar);
        }
    }
    m_var_deque.erase(m_var_deque.begin()+1);
    if ((m_var_deque[1].uintvar != 2) || (m_var_deque.size() != 2)) {
        log_var.set_failed();
        adam_printf("[----------] std::deque<Stlteststruct> erase error.\r\n");
    }
    m_var_deque.clear();
    if (m_var_deque.size() != 0) {
        log_var.set_failed();
        adam_printf("[----------] std::deque<Stlteststruct> clear error.\r\n");
    }
    //test list
    Stltestval listtest1, listtest2;
    listtest1.seti(1);
    listtest2.seti(2);
    m_var_list.push_back(listtest1);
    m_var_list.push_back(listtest2);
    if (m_var_list.size() != 2) {
        log_var.set_failed();
        adam_printf("[----------] std::list<Stltestval> push_back error.\r\n");
    }
    Stltestval listesttmp = m_var_list.front();
    if (listesttmp.geti() != 1) {
        log_var.set_failed();
        adam_printf("[----------] std::list<Stltestval> front error.\r\n");
    }
    m_var_list.pop_front();
    listesttmp = m_var_list.front();
    if (listesttmp.geti() != 2) {
        log_var.set_failed();
        adam_printf("[----------] std::list<Stltestval> pop_front error.\r\n");
    }
    m_var_list.erase(m_var_list.begin());
    if (m_var_list.size() != 0) {
        log_var.set_failed();
        adam_printf("[----------] std::list<Stltestval> erase error.\r\n");
    }
    m_var_list.clear();
    return log_var.get_state();
}

bool Test_time_impl() {
    adam_test_log log_var(__func__);
    var1 = new ThreadTestVal();
    uint64_t starttime = __gettickcount();
    var1->Lock();
    if (ETIMEDOUT != var1->TimeWait(1540)) {
        log_var.set_failed();
        adam_printf("[----------] pthread_cond_timedwait error failed to test time.\r\n");
    }
    uint64_t curtime = __gettickcount();
    if (((curtime - starttime) < 1540) || ((curtime - starttime) > 1550)) {
        log_var.set_failed();
        adam_printf("[   WARN   ] clock_gettime accuracy warnning req:1540ms, actual:%llums.\r\n", curtime - starttime);
    }
    delete var1;
    return log_var.get_state();
}

bool Test_file_impl() {
    adam_test_log log_var(__func__);
    std::string testdir = FILESYSTEM_TEST_DIR;
    if (0 != access(testdir.c_str(), 0)) {
        mkdir(testdir.c_str(), 0755);
    }
    //add one more dir to protect user filesystem
    testdir = testdir + "/wechat";
    //test dir opera
    adam_remove_dir(testdir.c_str());
    if (0 == access(testdir.c_str(), 0)) {
        log_var.set_failed();
        adam_printf("[----------] remove or rmdir failed dir exit!\r\n");
    } else {
        mkdir(testdir.c_str(), 0755);
    }
    if (0 != access(testdir.c_str(), 0)) {
        log_var.set_failed();
        adam_printf("[----------] mkdir failed!\r\n");
    }
    //test file opera
    std::string filefullpath = testdir + "/testfile.txt";
    std::string data = "hello world:";
    std::string appenddata = "zorrowu";
    int file_fd = open(filefullpath.c_str(), O_RDWR|O_CREAT|O_TRUNC, 0777);
    if (file_fd < 0) {
        log_var.set_failed();
        adam_printf("[----------] create file failed:%d.\r\n", file_fd);
    } else {
        ssize_t writesize = write(file_fd, data.c_str(), data.size());
        if (writesize != data.size()) {
            log_var.set_failed();
            adam_printf("[----------] write file failed:%d.\r\n", writesize);
        }
        close(file_fd);
    }
    //test read file
    file_fd = open(filefullpath.c_str(), O_RDONLY, 0777);
    if (file_fd < 0) {
        log_var.set_failed();
        adam_printf("[----------] open read only file failed:%d.\r\n", file_fd);
    } else {
        uint64_t filesize = lseek(file_fd, 0, SEEK_END);
        lseek(file_fd, 0, SEEK_SET);
        char* buffer = (char*)calloc(1, filesize);
        ssize_t readsize = read(file_fd, buffer, filesize);
        if ((filesize != readsize) || (filesize != data.size())) {
            log_var.set_failed();
            adam_printf("[----------] read file data error shouldbe:%zd actual:%llu read:%zd.\r\n", data.size(), filesize, readsize);
        } else if (0 != memcmp(buffer, data.c_str(), data.size())) {
            log_var.set_failed();
            adam_printf("[----------] read file content error.\r\n");
        }
        free(buffer);
        close(file_fd);
    }
    //test append file
    file_fd = open(filefullpath.c_str(), O_RDWR|O_CREAT|O_APPEND, 0777);
    if (file_fd < 0) {
        log_var.set_failed();
        adam_printf("[----------] open append file failed:%d.\r\n", file_fd);
    } else {
        ssize_t writesize = write(file_fd, appenddata.c_str(), appenddata.size());
        if (writesize != appenddata.size()) {
            log_var.set_failed();
            adam_printf("[----------] append write file failed:%d.\r\n", writesize);
        }
        close(file_fd);
    }
    file_fd = open(filefullpath.c_str(), O_RDONLY, 0777);
    if (file_fd < 0) {
        log_var.set_failed();
        adam_printf("[----------] after append open read only file failed:%d.\r\n", file_fd);
    } else {
        uint64_t filesize = lseek(file_fd, 0, SEEK_END);
        lseek(file_fd, 0, SEEK_SET);
        char* buffer = (char*)calloc(1, filesize);
        ssize_t readsize = read(file_fd, buffer, filesize);
        std::string finaldata = data + appenddata;
        if ((filesize != readsize) || (filesize != (data.size() + appenddata.size()))) {
            log_var.set_failed();
            adam_printf("[----------] append read file data error shouldbe:%zd actual:%llu read:%zd.\r\n", data.size() + appenddata.size(), filesize, readsize);
        } else if (0 != memcmp(buffer, finaldata.c_str(), finaldata.size())) {
            log_var.set_failed();
            adam_printf("[----------] append read file content error.\r\n");
        }
        free(buffer);
        close(file_fd);
    }
    return log_var.get_state();
}

static void *pthread_test_wakeup(void *arg) {
    ThreadTestVal* vars = (ThreadTestVal*)arg;
    while(!m_end_thred) {
        var2->Lock();
        var2->Signal();
        var2->Unlock();
        vars->Lock();
        if (m_queue.size() == 0) {
            vars->Wait();
            m_recv_signals++;
        }
        if (m_queue.size() != 0) {
            m_queue.pop_front();
            m_recv_queue++;
        }
        vars->Unlock();
    }
    return NULL;
}

bool Test_thread_impl() {
    adam_test_log log_var(__func__);
    uint64_t starttime = __gettickcount();
    var1 = new ThreadTestVal();
    var2 = new ThreadTestVal();
    pthread_t m_thread;
    uint32_t timeout = 1500;
    uint32_t test_count = 10000;

    //test pthread_cond_timedwait
    var1->Lock();
    if (ETIMEDOUT != var1->TimeWait(timeout)) {
        log_var.set_failed();
        adam_printf("[----------] pthread_cond_timedwait test failed.\r\n");
    } else if ((__gettickcount() - starttime) < timeout) {
        log_var.set_failed();
        adam_printf("[----------] pthread_cond_timedwait test failed time not correct %llu:%u.\r\n", __gettickcount() - starttime, timeout);
    }
    var1->Unlock();
    //condition press test
    starttime = __gettickcount();
    pthread_attr_t thread_attr;
    pthread_attr_init(&thread_attr);
    int ret = pthread_attr_setstacksize(&thread_attr, PTHREAD_STACK_MIN);
    if (ret != 0) {
        log_var.set_failed();
        adam_printf("[----------] pthread_attr_setstacksize failed ret:%d.\r\n", ret);
    }
    pthread_create(&m_thread, &thread_attr, pthread_test_wakeup, var1);
    ret = pthread_attr_destroy(&thread_attr);
    if (ret != 0) {
        log_var.set_failed();
        adam_printf("[----------] pthread_attr_destroy failed ret:%d.\r\n", ret);
    }
    for (uint32_t i=0; i<test_count;) {
        var2->Lock();
        var2->TimeWait(10);
        var2->TimeWait(1);//make sure test thread enter wait
        var2->Unlock();
        var1->Lock();
        if (m_queue.size() == 0) {
            m_queue.push_back(1);
            var1->Signal();
            i++;
        }
        var1->Unlock();
        if ((__gettickcount() - starttime) > 50000) {
            log_var.set_failed();
            adam_printf("[----------] pthread cond press test failed cost too much time:%llu sent signal:%d.\r\n", __gettickcount() - starttime, i);
            break;
        }
    }
    //test thread join
    m_end_thred = true;
    var1->Lock();
    var1->Signal();
    var1->Unlock();
    if (0 != pthread_join(m_thread, NULL)) {
        log_var.set_failed();
        adam_printf("[----------] pthread join failed.\r\n");
    }
    if ((m_recv_queue != test_count) || (m_recv_signals != test_count)) {
        adam_printf("[   INFO   ] pthread cond press test not match queue:%u signals:%u.\r\n", m_recv_queue, m_recv_signals);
    }
    delete var1;
    delete var2;
    return log_var.get_state();
}
#if RUN_MBEDTLS_TEST
bool base64decode(const char* srcdata, uint32_t srcdata_len, std::string& decdata) {
    if ((srcdata == NULL) || ((srcdata_len % 4) !=0) ) {
        return false;
    }
    size_t olen;
    uint32_t tmp = 0;
    for (const char* input_end = srcdata + srcdata_len; input_end != srcdata && *--input_end == '='; ) {
        ++tmp;
    }
    if (tmp > 2) {
        return false;
    }
    tmp =  (srcdata_len / 4) * 3 - tmp;
    unsigned char *dstdata = (unsigned char *)malloc(tmp);
    if (dstdata == NULL) {
        return false;
    }
    int ret = mbedtls_base64_decode(dstdata, tmp, &olen, (const unsigned char *)srcdata, srcdata_len);
    if (ret != 0) {
        free(dstdata);
        return false;
    }
    decdata.append((const char*)dstdata, olen);
    free(dstdata);
    return true;
}
bool ecdh_init(mbedtls_ecp_group& m_grp_, mbedtls_ecp_point& m_self_public_, mbedtls_mpi& m_self_private_) {
    int ret;
    const char *pers = "test_ecdh";
    mbedtls_entropy_context entropy;
    mbedtls_ctr_drbg_context ctr_drbg;
    mbedtls_entropy_init(&entropy);
    mbedtls_ctr_drbg_init(&ctr_drbg);
    ret = mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy, (const unsigned char *) pers, strlen(pers));
    if(ret != 0) {
        mbedtls_ctr_drbg_free(&ctr_drbg);
        mbedtls_entropy_free(&entropy);
        return false;
    }
    ret = mbedtls_ecp_group_load(&m_grp_, MBEDTLS_ECP_DP_SECP256R1);
    if(ret != 0) {
        mbedtls_ctr_drbg_free(&ctr_drbg);
        mbedtls_entropy_free(&entropy);
        return false;
    }
    ret = mbedtls_ecdh_gen_public(&m_grp_, &m_self_private_, &m_self_public_, mbedtls_ctr_drbg_random, &ctr_drbg);
    if(ret != 0) {
        mbedtls_ctr_drbg_free(&ctr_drbg);
        mbedtls_entropy_free(&entropy);
        return false;
    }
    mbedtls_ctr_drbg_free(&ctr_drbg);
    mbedtls_entropy_free(&entropy);
    return true;
}
bool ecdsa_test(mbedtls_ecp_group& m_grp_, mbedtls_ecp_point& m_self_public_, mbedtls_mpi& m_self_private_, std::string& sign_key) {
    uint8_t buf[65];
    size_t olen;
    int ret;
    ret = mbedtls_ecp_point_write_binary(&m_grp_, &m_self_public_, MBEDTLS_ECP_PF_UNCOMPRESSED, &olen, buf, sizeof(buf));
    if (ret != 0) {
        return false;
    }
    std::string body;
    body.append((const char*)buf, olen);
    unsigned char hash[32];
    unsigned char sig[MBEDTLS_ECDSA_MAX_LEN];
    size_t sig_len = 0;
    const char *pers = "test_ecdsa";
    mbedtls_pk_context pripk;
    mbedtls_ecdsa_context ctx_sign;
    mbedtls_entropy_context entropy;
    mbedtls_ctr_drbg_context ctr_drbg;
    ret = mbedtls_sha256_ret((const unsigned char *)body.c_str(), body.size(), hash, 0);
    if (ret != 0 ) {
        return false;
    }
    mbedtls_pk_init(&pripk);
    ret = mbedtls_pk_parse_key(&pripk, (const unsigned char *)sign_key.c_str(), sign_key.size(), NULL, 0);
    if (ret != 0) {
        mbedtls_pk_free(&pripk);
        return false;
    }
    mbedtls_ecdsa_init(&ctx_sign);
    ret = mbedtls_ecdsa_from_keypair(&ctx_sign, (const mbedtls_ecp_keypair *)pripk.pk_ctx);
    if (ret != 0) {
        mbedtls_pk_free(&pripk);
        mbedtls_ecdsa_free(&ctx_sign);
        return false;
    }
    mbedtls_entropy_init(&entropy);
    mbedtls_ctr_drbg_init(&ctr_drbg);
    ret = mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy, (const unsigned char *)pers, strlen(pers));
    if (ret != 0) {
        mbedtls_ctr_drbg_free(&ctr_drbg);
        mbedtls_ecdsa_free(&ctx_sign);
        mbedtls_entropy_free(&entropy);
        mbedtls_pk_free(&pripk);
        return false;
    }
    ret = mbedtls_ecdsa_write_signature(&ctx_sign, MBEDTLS_MD_SHA256, hash, sizeof(hash), sig, &sig_len, mbedtls_ctr_drbg_random, &ctr_drbg);
    mbedtls_ecdsa_free(&ctx_sign);
    mbedtls_ctr_drbg_free(&ctr_drbg);
    mbedtls_entropy_free(&entropy);
    mbedtls_pk_free(&pripk);
    if ((ret != 0) || (sig_len == 0)) {
        return false;
    } else {
        return true;
    }
}
static const int kGcmNonceLength = 12;
static const int kGcmTagLength = 16;
int aes_gcm_encrypt(const std::string& key, const char* input, uint32_t input_size, char** output, uint32_t* output_len) {
    mbedtls_gcm_context aes;
    int ret;
    std::string nonce = "0123456789ab";
    *output_len = 0;

    mbedtls_gcm_init(&aes);
    ret = mbedtls_gcm_setkey(&aes, MBEDTLS_CIPHER_ID_AES, (const unsigned char*)key.c_str(), key.size() * 8);
    if (ret != 0) {
        mbedtls_gcm_free(&aes);
        return ret;
    }
    ret = mbedtls_gcm_starts(&aes, MBEDTLS_GCM_ENCRYPT, (const unsigned char*)nonce.c_str(), nonce.size(), NULL, 0);
    if (ret != 0) {
        mbedtls_gcm_free(&aes);
        return ret;
    }
    *output_len = input_size+kGcmNonceLength+kGcmTagLength;
    *output = (char*)malloc(*output_len);
    if (!(*output)) {
        *output_len = 0;
        mbedtls_gcm_free(&aes);
        return -1;
    }
    ret = mbedtls_gcm_update(&aes, input_size, (const unsigned char*)input, (unsigned char *)*output);
    if (ret != 0) {
        free(*output);
        *output = NULL;
        *output_len = 0;
        mbedtls_gcm_free(&aes);
        return ret;
    }
    memcpy(*output + input_size, nonce.c_str(), nonce.size());
    ret = mbedtls_gcm_finish(&aes, (unsigned char *)(*output + input_size + nonce.size()), kGcmTagLength);
    if (ret != 0) {
        free(*output);
        *output = NULL;
        *output_len = 0;
        mbedtls_gcm_free(&aes);
        return ret;
    }
    mbedtls_gcm_free(&aes);
    return 0;
}
int aes_gcm_decrypt(const std::string& key, const char* input, uint32_t input_size, char** output, uint32_t* output_len) {
    mbedtls_gcm_context aes;
    int ret;
    unsigned char tag[kGcmTagLength];
    *output_len = 0;

    mbedtls_gcm_init(&aes);
    ret = mbedtls_gcm_setkey(&aes, MBEDTLS_CIPHER_ID_AES, (const unsigned char*)key.c_str(), key.size() * 8);
    if (ret != 0) {
        mbedtls_gcm_free(&aes);
        return ret;
    }
    ret = mbedtls_gcm_starts(&aes, MBEDTLS_GCM_DECRYPT, (const unsigned char*)&input[input_size-kGcmTagLength-kGcmNonceLength], kGcmNonceLength, NULL, 0);
    if (ret != 0) {
        mbedtls_gcm_free(&aes);
        return ret;
    }
    *output_len = input_size-kGcmNonceLength-kGcmTagLength;
    *output = (char*)malloc(*output_len);
    if (!(*output)) {
        *output_len = 0;
        mbedtls_gcm_free(&aes);
        return -1;
    }
    ret = mbedtls_gcm_update(&aes, *output_len, (const unsigned char*)input, (unsigned char *)*output);
    if (ret != 0) {
        free(*output);
        *output = NULL;
        *output_len = 0;
        mbedtls_gcm_free(&aes);
        return ret;
    }
    ret = mbedtls_gcm_finish(&aes, tag, kGcmTagLength);
    if (ret != 0) {
        free(*output);
        *output = NULL;
        *output_len = 0;
        mbedtls_gcm_free(&aes);
        return ret;
    }
    mbedtls_gcm_free(&aes);
    //check tag
    for (int i=0; i < kGcmTagLength; i++) {
        if (tag[i] != (unsigned char)input[input_size-kGcmTagLength+i]) {
            free(*output);
            *output = NULL;
            *output_len = 0;
            return -1;
        }
    }
    return 0;
}
bool Test_mbedtls_impl() {
    adam_test_log log_var(__func__);
    std::string sign_key;
    std::string verify_key;
    std::string lic_decode_data;
    //aes-gcm var
    int ret;
    std::string aes_key = "0123456789123456";
    std::string databody = "hello world zorrowu!";
    std::string encdata;
    std::string decdata;
    char* encrypted_data = NULL;
    uint32_t encrypted_data_size;
    char* decrypted_data = NULL;
    uint32_t decrypted_data_size;
    //test base64 decode
    if (false == base64decode(test_lic.c_str(), test_lic.size(), lic_decode_data)) {
        log_var.set_failed();
        adam_printf("[----------] mbedtls_base64_decode failed.\r\n");
        return false;
    }
    unsigned char pri_len = lic_decode_data.at(0);
    sign_key = lic_decode_data.substr(1, pri_len);
    unsigned char pub_len = lic_decode_data.at(1+pri_len);
    verify_key = lic_decode_data.substr(2+pri_len, pub_len);
    if (lic_decode_data.size() != (2 + pri_len + pub_len)) {
        log_var.set_failed();
        adam_printf("[----------] mbedtls_base64_decode data error.\r\n");
        return false;
    }
    //test ecdh functions
    mbedtls_ecp_group m_grp_;
    mbedtls_ecp_point m_self_public_;
    mbedtls_mpi m_self_private_;
    mbedtls_ecp_group_init(&m_grp_);
    mbedtls_ecp_point_init(&m_self_public_);
    mbedtls_mpi_init(&m_self_private_);
    if (false == ecdh_init(m_grp_, m_self_public_, m_self_private_)) {
        log_var.set_failed();
        adam_printf("[----------] ecdh_init failed.\r\n");
        goto test_end;
    }
    //test ecdsa functions
    if (false == ecdsa_test(m_grp_, m_self_public_, m_self_private_, sign_key)) {
        log_var.set_failed();
        adam_printf("[----------] ecdsa_test failed.\r\n");
        goto test_end;
    }
    //test aes-gcm functions
    ret = aes_gcm_encrypt(aes_key, databody.c_str(), databody.size(), &encrypted_data, &encrypted_data_size);
    if (ret != 0) {
        log_var.set_failed();
        adam_printf("[----------] aes_gcm_encrypt test failed.\r\n");
        goto test_end;
    }
    encdata.append(encrypted_data, encrypted_data_size);
    free(encrypted_data);
    ret = aes_gcm_decrypt(aes_key, encdata.c_str(), encdata.size(), &decrypted_data, &decrypted_data_size);
    if (ret != 0) {
        log_var.set_failed();
        adam_printf("[----------] aes_gcm_decrypt test failed.\r\n");
        goto test_end;
    }
    decdata.append(decrypted_data, decrypted_data_size);
    free(decrypted_data);
    if ((databody.size() != decdata.size()) || (0 != memcmp(databody.c_str(), decdata.c_str(), databody.size()))) {
        log_var.set_failed();
        adam_printf("[----------] aes_gcm_process test failed.\r\n");
        goto test_end;
    }
test_end:
    mbedtls_mpi_free(&m_self_private_);
    mbedtls_ecp_point_free(&m_self_public_);
    mbedtls_ecp_group_free(&m_grp_);
    return log_var.get_state();
}
#endif

int wechat_main(int argc, const char * argv[]) {
    adam_printf("-------- START  ---------\r\n");
    //add task to list
    g_task_list.push_back(Test_stl_impl);
    g_task_list.push_back(Test_time_impl);
    g_task_list.push_back(Test_file_impl);
    g_task_list.push_back(Test_thread_impl);
#if RUN_MBEDTLS_TEST
    g_task_list.push_back(Test_mbedtls_impl);
#endif
    uint64_t starttime = __gettickcount();
    
    adam_printf("[==========] Running %lu test units.\r\n\r\n", g_task_list.size());
    std::list<test_func_entry>::iterator itrea;
    for (itrea = g_task_list.begin();itrea != g_task_list.end();itrea++) {
        if ((*itrea)()) {
            g_scuess_tasks++;
        }
    }
    adam_printf("[==========] Finish Running %lu test units. (%llu ms total)\r\n", g_task_list.size(), __gettickcount() - starttime);
    if (g_scuess_tasks == g_task_list.size()) {
        adam_printf("[  PASSED  ] All %lu test units passed.\r\n", g_task_list.size());
    } else {
        adam_printf("[  FAILED  ] %u test units passed and %ld test units failed.\r\n", g_scuess_tasks, g_task_list.size() - g_scuess_tasks);
    }
    adam_printf("-------- FINISH --------\r\n");
    return 0;
}
