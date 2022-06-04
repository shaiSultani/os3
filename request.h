#ifndef __REQUEST_H__

//for part3
//typedef struct struct_headers {
//    struct timeval stat_req_dispatch;
//    struct timeval stat_req_arrival;
//    int stat_thread_id;
//    int stat_thread_count;
//    int stat_thread_static;
//    int stat_thread_dynamic;
//}Headers;

void requestHandle(int fd);

#endif
