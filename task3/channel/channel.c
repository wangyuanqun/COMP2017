#include "channel.h"
#include <unistd.h>
#include <stdlib.h>
void channel_init(struct receiver* recv, struct sender* sender,
  size_t msg_sz) {
      if (recv && sender && msg_sz > 0){
          sender->s = msg_sz;
          recv->s = msg_sz;

          int fd[2] = {-1};
          pipe(fd);

          sender->se_fd = fd[1];
          recv->re_fd = fd[0];
      }
}

void sender_dup(struct sender* dest, struct sender* src) {
    if(dest && src) {
        dest->se_fd = src->se_fd;
        dest->s = src->s;
    }
}

void channel_get(struct receiver* channel, void* data) {
    if (data && channel) {
        read(channel->re_fd, data, channel->s);
    }
}

void channel_send(struct sender* channel, void* data) {
    if (data && channel){
        write(channel->se_fd, data, channel->s);
    }
}

void channel_destroy(struct receiver* recv, struct sender* sender) {
    if(recv && sender) {return;}
}
