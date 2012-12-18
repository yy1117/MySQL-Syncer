
#include <rs_config.h>
#include <rs_core.h>

int rs_create_temp_buf(rs_buf_t *b, uint32_t size)
{
    if(b == NULL) {
        rs_log_err(0, "rs_create_temp_buf() failed, b must not be null");
        return RS_ERR;
    }

    b->start = malloc(size);
    if (b->start == NULL) {
        rs_log_err(rs_errno, "malloc() failed, %u", size);
        return RS_ERR;
    }

    b->size = size;
    b->pos = b->start;
    b->last = b->start;
    b->end = b->start + size;
    
    return RS_OK;
}

int rs_send_temp_buf(int fd, rs_buf_t *b)
{
    uint32_t    size;
    ssize_t     n;

    while((size = b->last - b->pos) > 0) {

        rs_log_debug(0, "rs_send_temp_buf send size : %u", size);

        n = rs_write(fd, b->pos, size);

        if(n <= 0) {
            return RS_ERR;
        }

        b->pos += n;
    }

    b->pos = b->start;
    b->last = b->start;

    return RS_OK;
}

void rs_free_temp_buf(rs_buf_t *b)
{
    if(b != NULL && b->start != NULL) {
        free(b->start); 
    }
}
